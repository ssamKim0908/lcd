# 2. Framework

이 문서는 Server 와 App Manager 를 소스 코드 중심으로 설명합니다.

---

## 1. Server

소스: [framework/Server.hpp](C++_LCD_Driver/framework/Server.hpp), [framework/Server.cpp](C++_LCD_Driver/framework/Server.cpp)

`Server` 는 의존성을 생성자 주입으로 받습니다. 구체 타입을 만들어 넘기는 곳은 [main.cpp](C++_LCD_Driver/main.cpp) 뿐입니다.

```cpp
Server(std::unique_ptr<IServer>    server,    // UDS 리슨 소켓
       std::unique_ptr<IPoller>    poller,    // epoll
       std::unique_ptr<IKeys>      keys,      // GPIO 키
       std::shared_ptr<Rasterizer> rasterizer);
```

Server 자신은 인터페이스(`IServer`, `IPoller`, `IKeys`)만 알 뿐, epoll·UDS·GPIO 구현은 모릅니다.

### 1.1 `run()` — 이벤트 루프

```cpp
void Server::run()
{
    poller_->add(server_->fd(), In);   // ① 리슨 소켓 (새 클라이언트 접속)
    poller_->add(keys_->fd(),   In);   // ② GPIO 키 입력

    while (running_)
    {
        PollResult ev = poller_->wait();          // 이벤트 1개 대기
        if (ev.fd == IPoller::ERROR.fd) continue;
        if ((ev.events & In) == 0)      continue;

        if      (ev.fd == server_->fd()) on_accept();   // 새 연결
        else if (ev.fd == keys_->fd())   on_key();      // 키 입력
        else                             on_recv(ev.fd); // 클라이언트 패킷
    }
}
```

Server가 처리하는 것은 다음과 같습니다.

| fd | 핸들러 | 하는 일 |
|---|---|---|
| Listen Socket | `on_accept()` | 새 클라이언트를 받아들임 |
| GPIO 키 | `on_key()` | 키 이벤트를 현재 활성 클라이언트로 전송 |
| 클라이언트 소켓 | `on_recv()` | 받은 draw 패킷을 실행 |


epoll을 이용해 하나의 루프에서 감시하다가, 깨어난 fd 종류에 따라 분기합니다.

#### epoll 구조

epoll은 OSAL의 [osal/epoll.cpp](C++_LCD_Driver/osal/epoll.cpp) 에서 `IPoller` 인터페이스로 감싸 둡니다.

```cpp
class IPoller {
    virtual void       add (int fd, uint32_t events) = 0;  // epoll_ctl ADD
    virtual void       del (int fd)                  = 0;  // epoll_ctl DEL
    virtual PollResult wait()                        = 0;  // epoll_wait
};
```

- `Epoll` 생성자에서 `epoll_create1`, `add/del` 은 `epoll_ctl`, `wait` 은 `epoll_wait` 을 호출합니다.
- `wait()` 는 `epoll_wait(.., maxevents=1, timeout=-1)` 이라 이벤트 하나를 무한 대기합니다.
- Level-Triggered 방식으로 구현했습니다. 수정을 원하시면 epoll 관련 파일에서 수정하면 됩니다.

### 1.2 키 입력 포커싱 (FocusStack)

여러 클라이언트(App Manager, App)가 동시에 연결될 수 있지만, 키와 화면을 갖는 건 항상 하나입니다. Server는 연결된 채널을 `FocusStack`(스택)으로 관리하고, 맨 위(top) 만 활성으로 봅니다.

```cpp
class FocusStack {
    std::stack<std::shared_ptr<IChannel>> stack_;
    // push / pop / top
};
```

포커싱의 핵심은 epoll에는 항상 `리슨 소켓 + 키 fd + top 클라이언트 fd` 만 등록한다는 점입니다.

- **`on_accept()`** — 새 클라이언트가 접속하면, 기존 top을 epoll에서 `del` 하고 새 채널을 push한 뒤 새 fd를 `add`합니다. → 새 클라이언트가 top(활성)이 됩니다.

- **`on_key()`** — 키 이벤트는 `top` 에게만 보냅니다. top이 없으면 버립니다.


- **`on_recv(fd)`** — top이 보낸 패킷만 처리합니다. top이 아닌 fd는 무시합니다.

- **`disconnect_top()`** — top이 끊기면 pop하고, 바로 아래 클라이언트를 다시 epoll에 `add` 하여 top으로 복귀시킵니다.

이 동작이 App 종료 → App Manager 복귀 를 자연스럽게 만듭니다.

```
접속 순서             FocusStack (top = 활성)
App Manager 접속  →   [App Manager]
App 접속          →   [App Manager, App]   ← App 이 활성, App Manager 는 epoll 에서 빠짐
App 종료          →   [App Manager]         ← App Manager 가 다시 활성
```

---

## 2. App Manager

소스: [app_manager/](C++_LCD_Driver/app_manager/)

App Manager는 그 자체로 SDK로 만든 클라이언트입니다. [AppManager.hpp](C++_LCD_Driver/app_manager/AppManager.hpp) 를 보면 `sdk::App` 을 상속하고 `on_draw()`(메뉴 그리기) / `on_key()`(메뉴 이동·선택)만 구현합니다. 즉 일반 App과 동일한 방식으로 동작합니다.

### 2.1 어떻게 App을 실행시키는지

#### (1) 파일 등록 — apps.d

App Manager는 시작할 때 `apps.d/` 디렉토리의 설정 파일을 읽어 메뉴를 만듭니다. ([main.cpp](C++_LCD_Driver/app_manager/main.cpp))

```cpp
Manifest manifest(std::make_unique<LocalFileReader>());
apps = manifest.load_dir(manifest_dir);   // apps.d/*.conf 전부 로드
```

- [Manifest.cpp](C++_LCD_Driver/app_manager/Manifest.cpp) 는 디렉토리의 `*.conf` 파일을 모두 읽어, 각 줄을 `AppEntry{ name, path }` 로 파싱합니다.
- 파일 형식은 한 줄에 하나, `이름 | 실행파일_절대경로` 입니다. `#` 으로 시작하는 줄은 주석입니다.

  ```
  Counter | /home/.../example/counter/build/counter
  ```

- 이 등록 파일은 보통 직접 만들지 않고, 예제 Makefile의 `make register` 로 자동 생성합니다(자세한 건 [how_to_make_app](03_how_to_make_app.md)).

#### (2) fork — 프로세스 실행

메뉴에서 `Center` 키를 누르면 선택된 앱을 실행합니다.

- `fork` 로 자식을 만들고 `execl` 로 App 바이너리로 교체합니다.
- **Blocking**: 부모(App Manager)는 `waitpid` 에서 App이 끝날 때까지 멈춰 있습니다. App 실행 동안 App Manager는 키 루프를 돌지 않고, 키는 App이 가져갑니다(위 FocusStack 참고).
- App이 종료되면 `waitpid` 가 반환되고, App Manager의 이벤트 루프가 재개되어 메뉴를 다시 그립니다.

---

[← 1. Architecture](01_architecture.md) | [3. How to Make App →](03_how_to_make_app.md)
