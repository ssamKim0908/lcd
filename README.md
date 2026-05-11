# LCD Framework

이 프로젝트는 Raspberry Pi Zero 2W와 1.3인치 TFT LCD 디스플레이를 위한 C++ 기반의 LCD 애플리케이션 프레임워크입니다. 

기존 제조사([Seengreat](https://github.com/seengreat/1.3inch-LCD-Display))에서 제공하는 C 언어 기반의 드라이버 코드를 C++ 구조로 재설계하여, 더 쉽고 확장성 있는 LCD 애플리케이션 개발 환경을 제공합니다.

---

## 개요

여러 앱이 독립적으로 동작하고 LCD 자원을 효율적으로 관리할 수 있는 **프레임워크**를 지향합니다. Client-Server 구조를 채택하여 LCD 제어 로직(Server)과 실제 애플리케이션(Client/SDK)을 분리하였습니다.

- **원형 프로젝트**: [Seengreat 1.3inch LCD C Driver](https://github.com/seengreat/1.3inch-LCD-Display)
- **개발 환경**: Raspberry Pi Zero 2W, Raspbian OS (Linux)
- **사용 AI**: Claude Code

---

## 주요 특징

1.  **Client-Server 아키텍처**:
    - **Server**: LCD 하드웨어 제어, 래스터라이징(Rasterizing), 입력(Key) 이벤트를 관리합니다.
    - **Client (SDK)**: 프레임워크가 제공하는 SDK를 사용하여 하드웨어에 대한 직접적인 지식 없이도 LCD 앱을 개발할 수 있습니다.
2.  **OSAL (OS Abstraction Layer)**: SPI, GPIO, Epoll 등 리눅스 시스템 호출을 추상화하여 하드웨어 제어 로직을 깔끔하게 분리했습니다.

---

## 하드웨어 및 소프트웨어

### Hardware
- **Main Board**: Raspberry Pi Zero 2W (또는 호환 모델)
- **Display**: 1.3inch TFT LCD (ST7789 Driver, 240x240 resolution)
- **Interface**: SPI

### Software
- **OS**: Raspbian OS (Linux)
- **Compiler**: g++ (C++17 이상 권장)
- **Lanugage**: C++17

---

## 프로젝트 구조

```text
.
├── C_LCD_Driver/        # 원본 C 언어 드라이버 (Reference)
├── C++_LCD_Driver/      # 핵심 프레임워크 소스 코드
│   ├── app_manager/     # 앱 실행 및 관리 매니저
│   ├── display/         # ST7789 LCD 제어 및 드라이버 로직
│   ├── framework/       # 서버 핵심 로직 및 명령어 처리
│   ├── hal/             # Hardware Abstraction Layer (SPI, GPIO)
│   ├── sdk/             # 앱 개발자를 위한 Client SDK
│   ├── ipc/             # UDS 기반 통신 모듈
│   ├── util/            # Font, Color, Span 등 유틸리티
│   └── main.cpp         # 서버 실행 엔트리 포인트
└── example/             # SDK 활용 예제 앱
    ├── hello/           # "Hello World" 출력 예제
    └── counter/         # 버튼 입력을 활용한 카운터 예제
```

---

## 빌드 및 실행 방법

### 1. 전체 프로젝트 빌드
```bash
cd C++_LCD_Driver
make
```

### 2. 드라이버 서버 실행
서버가 먼저 실행되어 LCD를 초기화하고 IPC 채널을 열어야 합니다.
```bash
./build/lcd_driver
```

### 3. 예제 앱 빌드 및 실행
```bash
cd example/counter
make
./build/counter_app
```

---

## 앱 개발 예시 (SDK 사용)

`sdk::App`을 상속받아 간단하게 LCD 앱을 만들 수 있습니다.

```cpp
#include <sdk/App.hpp>
#include <sdk/Draw.hpp>

class MyApp : public sdk::App {
protected:
    void on_render() override {
        draw().clear(0x0000); // Black
        draw().draw_text(10, 10, "Hello LCD!", util::font::TextSize::Small, 0xFFFF);
        draw().render();
    }

    void on_key(const KeyEvent& ev) override {
        if (ev.state == KeyState::Pressed && ev.key == Key::K1) {
            // 버튼 클릭 이벤트 처리
        }
    }
};

int main() {
    MyApp app;
    app.run();
    return 0;
}
```
