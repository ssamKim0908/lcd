# LCD Framework Project

## 목표
목표: 객체 지향 및 디자인 패턴 학습

## 내용
미니 스마트폰: 런처 화면에서 앱(Game, MP3)을 선택해 실행하고, 종료 시 런처로 복귀.
Android와 유사한 멀티 프로세스 구조.

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

┌─ Layer 3 : Framework + SDK + Apps ─────────────────────┐
│  framework/                                              │
│   - Scene, App                     (추상화)              │
│   - RenderServer (ILcd→IPC)                             │
│   - InputRouter  (IKeys→IPC)                            │
│   - FileServer   (IFileStore→IPC)                       │
│   - AppManager   (fork/exec — 여기서만, 앱은 모름)      │
│   - LauncherScene                                        │
│  sdk/                                                    │
│   - Canvas, Assets, ClientScene, ClientApp              │
│  apps/ game, mp3                                         │
└────────────────────────────────────────────────────────┘
           ↑ 의존
┌─ Layer 2 : 플랫폼 서비스 (3개 기능 = 3개 디렉토리) ─────┐
│  display/   ILcd      + St7789Lcd                        │
│  input/     IKeys     + GpioKeys                         │
│  storage/   IFileStore + LocalFileStore                  │
│  util/      color, time (인터페이스 없음 — 값/함수뿐)   │
└────────────────────────────────────────────────────────┘
           ↑ 의존
┌─ Layer 1 : HAL (Linux C API 감싸기) ───────────────────┐
│  hal/                                                    │
│   - Spi     (SPI ioctl/read/write)                      │
│   - Gpio    (GPIO)                                       │
│   - Socket? (Unix Domain Socket — 아래 논의)            │
└────────────────────────────────────────────────────────┘

---

## 설계 원칙

- **OOP/디자인 패턴 학습이 1순위** — 실용성보다 패턴 적용 연습 우선
- 각 레이어는 인터페이스(I-prefix) + 구체 구현으로 분리
- `.hpp`에 `#include` 최소화 — forward declaration 선호
- 레이어는 최소로 유지, 불필요한 중간 레이어 추가 금지
- `main()`이 Composition Root — 구체 타입을 아는 유일한 지점