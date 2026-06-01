# 3. How to Make App

이 문서는 SDK 의 구조와, 그 SDK로 App을 만드는 방법을 설명합니다.

---

## 1. SDK: 공개 / 비공개

SDK는 공개(public) 와 비공개(internal) 로 나뉩니다.

| 구분 | 내용 | 사용자에게 |
|---|---|---|
| **공개** | `App`, `Draw`, `Key` + 보조 타입(`KeyEvent`, `color`, `Font`, `Screen`) | 헤더로 제공 (`dist/include/`) |
| **비공개** | pImpl 구현(`AppImpl` 등), IPC 직렬화, UDS 클라이언트 | `libsdk.a` 안에만 (헤더 미공개) |

```
dist/
├── include/          ← 공개 헤더 (이것만 #include)
│   ├── sdk/   App.hpp · Draw.hpp · Key.hpp
│   ├── util/  color.hpp · Font.hpp · span.hpp
│   └── shared/ KeyEvent.hpp · Screen.hpp
└── lib/
    └── libsdk.a      ← 구현 (공개 + 비공개 전부 컴파일된 결과)
```

비공개 코드(`sdk/internal/`)는 `libsdk.a` 안에 컴파일되어 있지만, 헤더는 공개되지 않습니다.

---

## 2. 공개 헤더 + libsdk.a 만으로 개발 가능

App을 만들 때 필요한 건 공개 헤더(`include/`) 와 라이브러리 파일(`libsdk.a`) 뿐입니다.
IPC·UDS·직렬화 같은 내부 비공개를 전혀 몰라도 App을 개발할 수 있습니다.

- 만든 App은 App Manager에 등록해서 메뉴로 실행하는 것이 기본입니다.
- 단, App은 Server에 연결되는 독립 프로세스일 뿐입니다. 실행 구조를 바꾸면(예: Server가 직접 실행) App Manager 없이 단독으로도 동작시킬 수 있습니다. 다만, 이는 framework의 코드를 직접 이해하고, 수정해야 합니다.

---

## 3. 어떻게 앱을 만드는지

### 3.1 공개 SDK 세 가지

| 클래스 | 역할 |
|---|---|
| `sdk::App` | 앱의 베이스 클래스. 상속해서 `on_draw()` / `on_key()` 를 구현 |
| `sdk::Draw` | 그리기 명령(`App::draw()` 로 접근) |
| `sdk::Key` | 키 입력(보통 직접 안 쓰고 `on_key()` 로 받음) |


### 3.2 만드는 절차 (빌드 · 등록 · 실행)
#### 개발

App 개발은 **`sdk::App` 을 상속**해서 정해진 메소드 두 개만 구현하는 방식입니다.
전체 실행 흐름(루프)은 `App::run()` 이 이미 정해 놓았고(**Template Method 패턴**), 사용자는 그 안에서 호출되는 빈칸만 채웁니다.

```cpp
// sdk/App.cpp — 골격은 고정, on_draw()/on_key() 만 사용자 것이 호출됨
void App::run()
{
    running_ = true;
    on_draw();              // ① 최초 1회 화면 그림
    on_render();            //    화면 전송 (SDK 내부, 자동)
    while (running_)
    {
        KeyEvent ev = key().get_key();   // ② 키 입력 대기 (블로킹)
        on_key(ev);                      // ③ 키 처리 (상태 변경)
        if (!running_) break;            //    exit() 호출됐으면 종료
        on_draw();                       // ④ 바뀐 상태로 다시 그림
        on_render();                     //    화면 전송 (자동)
    }
}
```


핵심은 그리기는 `on_draw()` 한 곳에 모으고, `on_key()` 는 상태만 바꾼다는 점입니다.
키가 들어오면 `run()` 이 알아서 `on_draw()` 를 다시 불러 화면을 갱신하므로, `on_key()` 안에서 직접 그릴 필요가 없습니다. 화면 전송(`render`)도 `run()` 이 자동으로 처리합니다.

**사용자가 구현해야 하는 메소드 (순수 가상):**

| 메소드 | 여기서 할 일 |
|---|---|
| `on_draw()` | 현재 상태를 화면에 그립니다. 보통 `draw().clear(...)` 로 지우고 처음부터 다시 그립니다. 그리기 코드만 둡니다. |
| `on_key(const KeyEvent& ev)` | 눌린 키에 따라 내부 상태(변수)를 바꿉니다. 앱을 끝내려면 `exit()` 를 호출합니다. 그리기는 하지 않습니다. |

최소 예제:

```cpp
#include <sdk/App.hpp>
#include <sdk/Draw.hpp>

class CounterApp : public sdk::App {
protected:
    void on_draw() override {                       // 상태 → 화면
        draw().clear(util::color::BLACK);
        draw().draw_text(60, 100, std::to_string(value_),
                         util::font::TextSize::Small, util::color::WHITE);
    }
    void on_key(const KeyEvent& ev) override {      // 입력 → 상태
        if (ev.state != KeyState::Pressed) return;
        if      (ev.key == Key::Up)   ++value_;
        else if (ev.key == Key::Down) --value_;
        else if (ev.key == Key::K4)   exit();       // 종료
    }
private:
    int value_ = 0;
};

int main() { CounterApp app; app.run(); return 0; }
```



#### 빌드 및 등록/해지

예제([example/](example/))의 Makefile을 복사해 쓰는 것을 권장합니다.

```bash
# ① 최초 1회 — SDK 패키지 동기화 (드라이버 빌드 + 헤더/라이브러리 복사)
cd C++_LCD_Driver && make PLATFORM=device
cd ../example && make sync-sdk          # → example/sdk/{include,lib} 생성

# ② 앱 빌드
cd myapp
make                                    # -I sdk/include -L sdk/lib -lsdk

# ③ App Manager 에 등록
make register                           # apps.d/<APP_NAME>.conf 생성

# ④ 실행 — Server 를 켜면 메뉴에 등록된 앱이 보임
cd ../../C++_LCD_Driver && ./build/lcd_driver

# App Manager에서 해지하고 싶다면
make unregister

```

- 빌드는 공개 헤더(`-I sdk/include`)와 라이브러리(`-lsdk`)만 링크합니다. `display/`, `input/` 같은 HW 코드는 링크하지 않습니다.
- `make register` 는 `apps.d/<APP_NAME>.conf` 에 `이름 | 실행파일_절대경로` 한 줄을 기록합니다(2장 [framework](02_framwork.md) 의 파일 등록 참고).

---

## 4. App 메소드

소스: [sdk/App.hpp](C++_LCD_Driver/sdk/App.hpp)

| 메소드 | 접근 | 설명 |
|---|---|---|
| `run()` | public | 이벤트 루프 시작. `main()` 에서 한 번 호출 |
| `on_draw()` | protected, **순수가상** | 화면을 그림. 매 프레임 호출됨. |
| `on_key(const KeyEvent&)` | protected, **순수가상** | 키 이벤트 처리. |
| `draw()` → `Draw&` | protected | `Draw` 객체 접근 |
| `key()` → `Key&` | protected | `Key` 객체 접근 (보통 직접 안 씀) |
| `exit()` | protected | 이벤트 루프를 끝내 앱을 종료 |




---

## 5. Draw 메소드

소스: [sdk/Draw.hpp](C++_LCD_Driver/sdk/Draw.hpp). `App::draw()` 로 얻어서 사용합니다.

| 메소드 | 설명 |
|---|---|
| `clear(color)` | 화면 전체를 `color` 로 채움 |
| `fill_rect(x, y, w, h, color)` | 속이 찬 사각형 |
| `draw_rect(x, y, w, h, color)` | 테두리만 있는 사각형 |
| `fill_circle(cx, cy, r, color)` | 속이 찬 원 |
| `draw_circle(cx, cy, r, color)` | 테두리만 있는 원 |
| `draw_text(x, y, text, size, color)` | 문자열 출력 |

- `color` 는 **RGB565**(`uint16_t`). `util::color` 의 상수(`BLACK`, `WHITE`, `RED` …)를 쓰면 편합니다.
- 좌표계는 **240 × 240** (`screen::WIDTH`, `screen::HEIGHT`). 좌상단이 (0, 0).
- `size` 는 `util::font::TextSize` (현재 `Small` 만 제공).
- `draw_text` 의 문자열 길이는 255Byte 입니다. Text를 표현하는 Length를 1byte로 설정하여 그렇습니다. 수정을 원한다면, IPC, 직렬화/역직렬화 관련 코드에서 Length를 수정해야 합니다.

---

## 6. Key 메소드

소스: [sdk/Key.hpp](C++_LCD_Driver/sdk/Key.hpp)

| 메소드 | 설명 |
|---|---|
| `get_key()` → `KeyEvent` | 키 이벤트 하나를 **블로킹**으로 받음 |

보통 직접 호출하지 않습니다. `App::run()` 이 내부적으로 `get_key()` 를 호출해 `on_key()` 로 넘겨주기 때문입니다.


### KeyEvent

`on_key()` 가 받는 구조체입니다. 소스: [shared/KeyEvent.hpp](C++_LCD_Driver/shared/KeyEvent.hpp)

해당 키 값에 대한 동작을 구현하시면 되겠습니다.

```cpp
struct KeyEvent { Key key; KeyState state; };

enum class Key      { K1, K2, K3, K4, R, L, Up, Down, Left, Right, Center };
enum class KeyState { Pressed, Released };
```

---

## 7. pImpl

`App`, `Draw`, `Key` 는 모두 pImpl(pointer to implementation) 패턴입니다.
공개 헤더에는 구현 클래스의 전방 선언 + `unique_ptr` 멤버만 두고, 실제 구현은 `libsdk.a` 안에 숨깁니다.

```cpp
// App.hpp (공개)
namespace sdk {
class AppImpl;                       // 전방 선언만
class App {
    ...
private:
    std::unique_ptr<AppImpl> impl_;  // 구현은 비공개
};
}
```

---

[← 2. Framework](02_framwork.md)
