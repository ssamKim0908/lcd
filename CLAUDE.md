# LCD Framework Project

## 목표
목표: 객체 지향 및 디자인 패턴 학습

## 내용
미니 스마트폰: 런처 화면에서 앱(Game, MP3)을 선택해 실행하고, 종료 시 런처로 복귀.

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
---

## 이름
- class: Camel

## 전체 구조

┌─ Layer 2 : Framework ──────────────────────────────────┐
│  framework/  (launcher 프로세스에 링크)                 │
│   - RenderServer  (ILcd 사용 → IPC로 그림 명령 수신)     | 
|      -Rasterizer -> RenderServer에서 실제 그리는 객체    │
│   - InputRouter   (IKeys 사용 → IPC로 키 송신)          │
│   - AppManager    (fork/exec — 여기서만, 앱은 모름)     │
│  sdk/  (game/mp3 프로세스에 링크)                       │
│   - Canvas        (IPC 클라이언트 → "그려줘" 송신)      │
│   - Scene, App    (앱 측 베이스 클래스)                 │
│   - 이벤트 루프                                          │
│  shared/  (양쪽 binary에 다 들어감)                     │
│   - IPC 메시지 타입, Color, time, font              │
└────────────────────────────────────────────────────────┘
           ↑ 의존
┌─ Layer 1 : Platform ───────────────────────────────────┐
│  hal/       Spi, Gpio (Linux C API 감싸기)              │
│  display/   ILcd      + St7789Lcd                        │
│  input/     IKeys     + GpioKeys                         │
│  ipc/       Socket wrapper + send/recv 헬퍼             │
│  util/      color, time (인터페이스 없음 — 값/함수뿐)   │
└────────────────────────────────────────────────────────┘

[Apps — layer 아님, 그냥 main() 가진 binary]
  launcher/main.cpp    → Layer 2 framework + Layer 1 직접 사용
  apps/game/main.cpp   → Layer 2 sdk 만 사용 (LCD/GPIO 직접 X)
  apps/mp3/main.cpp    → Layer 2 sdk 만 사용 (LCD/GPIO 직접 X)

의존 규칙:
 - Layer 2 → Layer 1 (가능)
 - Layer 1 → Layer 2 (절대 금지)
 - launcher binary 만 Layer 1의 LCD/GPIO 직접 사용
 - app binary 는 Layer 2 sdk 통해서만 그림/입력 접근

---
## framework 구조
┌─ launcher process ──────────────────────────────────────────┐
│                                                              │
│  Main thread        InputThread          RenderThread(s)    │
│  ─────────────      ─────────────        ─────────────      │
│  AppManager         GpioRead              Rasterizer         │
│  (fork/exec,        .wait_event() ──┐    + decode loop       │
│   focus 관리)                        │                       │
│                                      ▼                       │
│                                 InputRouter                  │
│                                      │                       │
│                                      ▼                       │
│                            focused_app->ch.send(KeyEvent)    │
│                                                              │
│                                          ▲                   │
│                                          │ ch.recv()         │
│                                       (per app channel)      │
└──────────────────────────────────────────────────────────────┘

## 설계 원칙

- 각 레이어는 인터페이스(I-prefix) + 구체 구현으로 분리
- `.hpp`에 `#include` 최소화 — forward declaration 선호
- 레이어는 최소로 유지, 불필요한 중간 레이어 추가 금지
- `main()`이 Composition Root — 구체 타입을 아는 유일한 지점