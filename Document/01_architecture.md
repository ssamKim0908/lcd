# 1. Architecture

이 프레임워크는 **멀티 프로세스 Client-Server** 구조입니다.


## 1. 구성 요소


### 시퀀스 다이어 그램
여기 그림 필요

### Server
LCD 렌더링, GPIO 키 입력, 클라이언트 연결을 관리하는 프로세스.

LCD를 제어하는 유일한 프로세스이며, 시스템의 시작점(init 역할)입니다.

### App Manager
등록된 앱 목록을 메뉴로 보여주고, 선택한 앱을 실행하는 프로세스.

LCD를 모르며, 일반 App과 동일한 구조이고, SDK로 만든 클라이언트입니다.

다만, 라즈베리파이와는 파일 읽기, App 실행의 목적으로 RPI와는 소통을 합니다.

### App
사용자가 SDK로 직접 개발하여 실행하는 프로세스(예: example/counter). 

빌드 후 [실행 파일 이름]과 [실행 파일 위치]를 App manager의 apps.d에 등록해야 App manager를 통해 실행할 수 있습니다. Example의 Makefile을 복사하여 개발한다면, ```make register```, ```make unregister``` 를 이용해 자동 등록/해지 할 수 있습니다.

Server에 Draw를 요청하고 키 이벤트를 받아 동작합니다.

---

## 2. Layer

코드는 **OSAL** 과 **순수 C++ layer** 로 나뉩니다.

### 여기 그림 필요

```
┌──────────────────────────────────────────────┐
│  C++ Layer (순수 C++)                         │
│  framework(Server) · app_manager · sdk · app  │
└─────────────────────┬────────────────────────┘
                      │  의존은 ↓ 한 방향만
┌─────────────────────▼────────────────────────┐
│  OSAL  (리눅스 syscall 을 감싼 C++)           │
│  spi · gpio · epoll · ipc(uds) · process      │
└─────────────────────┬────────────────────────┘
                      ▼
                Linux Kernel
```

- C++ 와 C syscall 코드를 격리하는 것이 목적입니다.
- 리눅스 또는 C언어 관련 코드는 전부 OSAL(`osal/`)에서 볼 수 있습니다.
- 상위 layer는 OSAL을 인터페이스로(`interface/`)만 호출합니다.

---

## 3. 특징

### IPC + UDS

App / App Manager 는 **각자 Server와 UDS(Unix Domain Socket)로 연결**됩니다.
모두 같은 소켓 경로(`/tmp/lcd.sock`)로 접근하지만, 연결(채널)은 클라이언트마다 별개입니다.

```
  App Manager ──┐
                ├── 각자 별도의 connection ──> [ Server ]
  App         ──┘
```

- 소켓 타입은 `SOCK_SEQPACKET` (`recv` 1회 = 패킷 1개).
- 클라이언트가 연결/종료되어도 Server는 살아있고, 다른 클라이언트 연결에 영향을 주지 않습니다.

### Draw call RPC

sdk를 사용하여 LCD에 그리려면 Draw class의 맴버 함수들을 이용해야 합니다. 이 맴버 함수들, 가령 `draw().fill_rect(...)`은 모두 **원격 호출(Remote Procedure Call, 이하 RPC)** 입니다. 실제 그림은 Server가 그립다.


RPC를 위해 **호출을 패킷으로 직렬화해 보내고, Server가 그것을 다시 명령 객체로 복원** 합니다.

```
[Client]                                  [Server]
draw().fill_rect(x,y,w,h,c)
   │ 직렬화 (인자 → 바이트)
   ▼
 [패킷] ───────── UDS ─────────────────────> 수신
                                          CommandFactory: 패킷 역직렬화 → FillRectCommand Return
                                          command.execute()  →  Rasterizer.fill_rect()
```

- 클라이언트는 함수 인자를 패킷으로 바꿔 보냅니다. 다만, 이 모든 IPC 관련 코드는 pImpl Idiom을 이용해 캡슐화되어 있습니다. 즉, sdk를 사용해 app을 개발하는 사용자는 자신의 app이 ipc를 하는지 어떤지 몰라도 개발할 수 있습니다.

Server가 패킷을 받은 뒤의 처리는 **역직렬화 → Command 객체 생성 → 실행** 순서입니다.

**① 역직렬화 (패킷 → 인자)**

맨 앞 1 byte로 *어떤 그리기인지(종류)* 를 판별하고, 그 종류에 맞는 인자들을 보낸 순서·크기 그대로 다시 꺼냅니다. 보낼 때와 읽을 때가 같은 약속을 공유하므로 원래 호출의 인자가 그대로 복원됩니다.

**② Command 패턴 (인자 → 객체)**

복원한 인자로 **그 호출 하나를 나타내는 객체**(`FillRectCommand` 등)를 만듭니다. 각 객체는 `ICommand` 인터페이스(`execute()` 하나)를 구현하고, 내부에 *무엇을 그릴지(인자)* 와 *어디에 그릴지(receiver = Rasterizer)* 를 담습니다.

```cpp
class ICommand { public: virtual void execute() = 0; };

class FillRectCommand : public ICommand {
    std::shared_ptr<Rasterizer> receiver;     // 실제 그리는 대상
    int x, y, w, h; uint16_t color;           // 복원된 인자
public:
    void execute() override { receiver->fill_rect(x, y, w, h, color); }
};
```

종류 바이트로 분기해 알맞은 Command를 만드는 일은 Simple Factory가 담당합니다.

```cpp
switch (type) {
case C2S::FillRect:
    return std::make_unique<FillRectCommand>(receiver, x, y, w, h, color);
// case C2S::Clear, DrawText ...
}
```

**③ 실행**

Server는 만들어진 객체가 무엇인지 몰라도 `execute()` 만 호출합니다(다형성). 실제 그리기는 각 Command가 `Rasterizer` 에 위임합니다.

```cpp
auto command = factory.create(packet);   // 패킷 → ICommand
command->execute();                       // 종류를 몰라도 실행
```

- 이렇게 **그릴 대상을 결정(Factory)** 하는 부분과 **그리는 실행(execute)** 을 분리하면, 그리기 종류가 늘어나도 Server의 수신 루프는 바뀌지 않습니다. 새 Command 클래스와 Factory 분기만 추가하면 됩니다.

### SPI draw

기본적으로 LCD 화면은 240 * 240 화면입니다. 따라서, 우리는 240 * 240 *2(RGB16) 사이즈의 Array(이하 framebuffer)를 가지고 있습니다. 이 framebuffer에 그림을 그리고, LCD로 전송합니다.

이떄, Server가 그림을 화면에 내보내는 과정은 **2단계 + SPI 전송용 스레드**로 동작합니다.

```
draw 명령들 ──> framebuffer 에 누적 (메인 스레드)
render 명령 ──> SPI 전송용 스레드가 framebuffer을 복사 ──> SPI 전송
```

- draw 명령(`fill_rect` 등)은 Server의 **framebuffer**에 그려질 뿐, 아직 화면에 나가지 않습니다.
- SPI 전송용 스레드에는 framebuffer와 같은 크기의 buffer를 가지고 있습니다. `render` 명령을 받으면 그 시점의 framebuffer를 buffer로 복사해 **SPI로 LCD에 전송**합니다.
- 만약 아직 spi 전송이 끝나지 않은 상황에서 render 요청이 오면 어떻게 될까요? 우리는 이 요청을 무시하지 않습니다. 즉, spi 전용 스레드가 spi 전송을 끝낼 때까지 기다립니다.

---

[← 0. Introduction](00_introduction.md) | [2. Framework →](02_framwork.md)
