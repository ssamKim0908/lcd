# LCD Framework Project

## 목표

미니 스마트폰: 런처 화면에서 앱(Game, MP3)을 선택해 실행하고, 종료 시 런처로 복귀.
Android와 유사한 멀티 프로세스 구조.

## 언어/환경

- C++17
- Raspberry Pi + 1.3인치 ST7789 LCD (240x240)
- 물리 키 입력 (GPIO)

---

## 전체 구조

```
┌─────────────────────────────────────────────┐
│            Framework Process                 │
│  (항상 실행, 하드웨어 독점 소유)                │
│                                              │
│  ┌──────────┐  ┌────────────┐  ┌──────────┐ │
│  │AppManager│  │RenderServer│  │InputRouter│ │
│  │(앱 실행/ │  │(그리기 명령 │  │(키 이벤트 │ │
│  │ 종료관리)│  │ 받아 LCD출력│  │ 앱에 전달)│ │
│  └──────────┘  └────────────┘  └──────────┘ │
│                                              │
│  ┌─────────────────────────────┐            │
│  │  Lcd (실제 SPI/GPIO)         │            │
│  │  Keys (실제 GPIO)            │            │
│  └─────────────────────────────┘            │
└───────────────┬─────────────────────────────┘
                │  Unix Domain Socket (IPC)
        ┌───────┴───────┐
        ↓               ↓
┌──────────────┐  ┌──────────────┐
│ Game Process │  │ MP3 Process  │
│  GameScene   │  │  Mp3Scene    │
│  Client SDK  │  │  Client SDK  │
│  (IPC 숨김)  │  │  (IPC 숨김)  │
└──────────────┘  └──────────────┘
```

---

## 레이어 구조

### Layer 1 — HAL (Hardware Abstraction)
Linux C API를 C++로 감싸는 레이어.
Framework Process만 이 레이어에 접근.

```
hal/       — Gpio, Spi
interface/ — IRead, IWrite, IFactory
factory/   — LcdFactoryCommunication
service/   — LcdReadService, LcdWriteService
transport/ — LcdTransport
```

### Layer 2 — 구체 기본
LCD 그리기 + 키 입력 + 유틸리티. Framework Process 내부.

```
display/   — ILcd, St7789Lcd
input/     — IKeys, GpioKeys
util/      — Color, time
```

### Layer 3 — Framework
앱 생명주기, IPC, 렌더링 서버, 런처.

```
framework/
├── RenderServer   — 소켓으로 그리기 명령 수신 → Lcd 실행
├── InputRouter    — Keys 폴링 → 앱 소켓으로 키 이벤트 전송
├── AppManager     — fork/exec, 프로세스 생명주기
└── LauncherScene  — 런처 UI

sdk/               — 앱 개발자용 Client SDK
├── ClientScene    — 앱이 상속할 베이스 클래스
├── Canvas         — ILcd처럼 보이지만 내부는 IPC
└── ClientApp      — 소켓 연결 + 클라이언트 이벤트 루프
```

### App Layer
별도 실행파일. SDK만 사용, 하드웨어 직접 접근 불가.

```
apps/
├── game/   — game 실행파일
└── mp3/    — mp3 실행파일
```

## 설계 원칙

- **OOP/디자인 패턴 학습이 1순위** — 실용성보다 패턴 적용 연습 우선
- 각 레이어는 인터페이스(I-prefix) + 구체 구현으로 분리
- `.hpp`에 `#include` 최소화 — forward declaration 선호
- 레이어는 최소로 유지, 불필요한 중간 레이어 추가 금지
- `main()`이 Composition Root — 구체 타입을 아는 유일한 지점

