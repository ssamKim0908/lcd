# LCD Framework

이 프로젝트는 Raspberry Pi Zero 2W와 1.3인치 TFT LCD 디스플레이를 위한 C++ 기반의 LCD 애플리케이션 프레임워크입니다. 

기존 제조사([Seengreat](https://github.com/seengreat/1.3inch-LCD-Display))에서 제공하는 C 언어 기반의 드라이버 코드를 C++ 구조로 재설계하여, 더 쉽고 확장성 있는 LCD 애플리케이션 개발 환경을 제공합니다.

---

## 주요 특징

1.  **Client-Server**
    - **Server**: LCD 하드웨어 제어, 입력(Key) 이벤트를 관리합니다.
    - **Client (SDK)**: 프레임워크가 제공하는 SDK를 사용하여 하드웨어에 대한 직접적인 지식 없이도 LCD 앱을 개발할 수 있습니다.
2.  **Layer 아키텍쳐**
    - **목적**: 리눅스 시스템콜은 C언어 -> C언어 코드와 C++코드 격리 목표
    - **OS Abstract Layer (OSAL)**: raw C 시스템콜을 이용하는 C++ Layer.
    - **Application Layer** (server, app_manager, SDK, app): 순수 C++.

---

## 하드웨어 및 소프트웨어

### Hardware
- **Main Board**: Raspberry Pi Zero 2W (또는 호환 모델)
- **Display**: 1.3inch TFT LCD (ST7789 Driver, 240x240 resolution)
- **Interface**: SPI, GPIO

### Software
- **OS**: Raspbian OS (Linux)
- **Compiler**: g++ (C++17 이상 권장)
- **Lanugage**: C++17

---

## 프로젝트 디렉토리

```
lcd/
├── C_LCD_Driver/        # 원본 C 드라이버 (참고용)
├── C++_LCD_Driver/      # 프레임워크 본체
│   ├── main.cpp             서버 엔트리
│   ├── app_manager/         메뉴 + 앱 실행
│   ├── osal/                OS 추상화 (SPI, GPIO, Epoll, Process, IPC)
│   ├── osal/facade/
│   │   ├── display/         LCD 제어
│   │   └── input/           GPIO 키 입력
│   ├── framework/           서버
│   ├── sdk/                 클라이언트 SDK (공개 + internal/)
│   ├── shared/              서버·클라이언트 공통 타입
│   ├── interface/           내부 인터페이스 (I-prefix)
│   └── util/                Font, Span 등
├── example/             # SDK 사용 예제 앱
│   ├── hello/
│   ├── counter/
│   ├── sdk/                 (sync-sdk 로 채워지는 SDK 스냅샷)
│   └── Makefile             sync-sdk 등 상위 타깃
└── Document/            # 문서
```
---