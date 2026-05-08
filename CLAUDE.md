# LCD Framework Project

## 목표
목표: 객체 지향 및 디자인 패턴 학습

## 언어/환경

- C++17
- Raspberry Pi + 1.3인치 ST7789 LCD (240x240)
- 물리 키 입력 (GPIO)

## 개발 원칙
- RAII
- Smart pointer
- OOP
- SOLID
- STL Container
- Class 맴버 변수 참조자, raw pointer 금지
---

## 이름
- class: Camel

---

## 프로세스 구조

세 종류의 프로세스로 구성. **Server 가 시스템의 init 역할** — 부팅 시 server 가 먼저 뜨고 launcher 를 fork+exec 한다. Game/MP3 는 launcher 가 fork 한다.

```
server  (init, 하드웨어 독점)
  └─ launcher          ← server 가 fork
       └─ game | mp3   ← launcher 가 fork (한 번에 하나)
```

- **server**: LCD/GPIO 를 독점 소유. UDS 로 client 연결을 받고, focus stack 을 관리하며 키를 top 에게 라우팅한다. client 의 정체(launcher 인지 app 인지)를 모른다 — fd 만 보고 동작.
- **launcher**: server 의 client. 자체 UI 를 그리고 사용자 선택을 받아 app 을 fork+exec. `waitpid()` 로 잠들어 있다가 app 종료 시 깨어남.
- **app (game/mp3)**: server 의 client. 실행되는 동안만 살아있다 종료. focus 개념을 모른다 — 존재 = focus.

---

## 전체 구조

```
┌─ Layer 2 : Framework ──────────────────────────────────┐
│  framework/  (server 프로세스에 링크)                   │
│   - IpcServer       (epoll reactor: accept / recv /    │
│                      input 을 단일 스레드에서 처리)     │
│   - Rasterizer      (실제 그리기)                       │
│   - IRenderCommand  (Command 패턴 base)                │
│   - decode_one()    (recv → IRenderCommand)            │
│   - FocusStack      (fd 기반 stack — push / pop / top) │
│                                                         │
│  sdk/  (launcher / game / mp3 모두에 링크)              │
│   - Canvas       (Client Stub: 그리기 호출 → 직렬화 →  │
│                   send, 키 recv)                       │
│   - Scene, App   (client 측 베이스 클래스)             │
│   - 이벤트 루프                                          │
│                                                         │
│  shared/  (server, sdk 양쪽에 들어감)                   │
│   - IPC 메시지 타입, 직렬화 포맷, color, time, font     │
└─────────────────────────────────────────────────────────┘
           ↑ 의존
┌─ Layer 1 : Platform ───────────────────────────────────┐
│  hal/       Spi, Gpio (Linux C API 감싸기)              │
│  display/   ILcd      + St7789Lcd  (server 전용)        │
│  input/     IKeys     + GpioKeys   (server 전용,        │
│                                     fd 노출)           │
│  ipc/       UdsServer, UdsClient, UdsChannel           │
│  util/      color, time, font (인터페이스 없음 — 값/함수)│
└─────────────────────────────────────────────────────────┘

[Binaries]
  server/main.cpp     → Layer 2 framework + Layer 1 (LCD/GPIO/UDS)
  launcher/main.cpp   → Layer 2 sdk + ipc client + fork/exec
  apps/game/main.cpp  → Layer 2 sdk 만 사용
  apps/mp3/main.cpp   → Layer 2 sdk 만 사용
```

**의존 규칙**
- Layer 2 → Layer 1 (가능)
- Layer 1 → Layer 2 (절대 금지)
- LCD/GPIO 직접 접근은 **server 프로세스만**. launcher/app binary 에 `display/`, `input/` 링크 금지 — 빌드 단계에서 강제.
- launcher/app 은 `sdk/` 통해서만 그림/입력 접근.

---

## IPC 구조

### 단일 channel, full-duplex

client 하나당 UDS channel 하나. 동일 fd 로 양방향 메시지가 흐른다.

```
Server → Client : KeyEvent
Client → Server : RenderCommand
종료              : client 가 close (또는 shutdown(SHUT_WR)) → server 가 EOF 감지
```

### Focus stack

- 새 client connect → `push` → top 이 자동으로 focus
- top 이 close → `recv()==0` → `pop` → 새 top 이 focus
- 비정상 종료(crash) 는 현재 범위 밖. 모든 종료는 graceful close 가정 — EOF 는 항상 top 에서만 발생.

### RPC + Command 패턴

```
[Client]                          [Server]
Canvas.fill_rect(...)
                                   decode_one(fd)
  → channel.send  ───────────►     channel.recv                                   
                                   → unique_ptr<IRenderCommand>
                                   → cmd->execute(Rasterizer&)
```

Rasterizer 는 server 프로세스의 단일 자원. 단일 스레드에서만 호출되므로 race 없음.

### Launcher 프로세스 (단일 스레드)
```
main loop:
    KeyEvent k = canvas.recv_key();          // server 가 보냄 (focus = launcher 일 때만)
    AppKind picked = scene.handle_key(k);
    if (picked) {
        pid_t pid = fork_exec(picked);
        ::waitpid(pid, ...);                  // 잠듦. 이 사이 server 는 app 으로 키 라우팅
        scene.draw();                          // app 종료 → 깨어남 → 화면 복구
    }
```

focus 상태 변수 없음. `recv_key()` 자리에 있으면 = 내가 focus, `waitpid()` 자리에 있으면 = 자식이 focus.

---

## 설계 원칙

- 각 레이어는 인터페이스(I-prefix) + 구체 구현으로 분리
- `.hpp` 에 `#include` 최소화 — forward declaration 선호
- 레이어는 최소로 유지, 불필요한 중간 레이어 추가 금지
- `main()` 이 Composition Root — 구체 타입을 아는 유일한 지점
