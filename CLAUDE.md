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

**의존 규칙**
- Layer 2 → Layer 1 (가능)
- Layer 1 → Layer 2 (절대 금지)
- LCD/GPIO 직접 접근은 **server 프로세스만**. launcher/app binary 에 `display/`, `input/` 링크 금지 — 빌드 단계에서 강제.
- launcher/app 은 `sdk/` 통해서만 그림/입력 접근.

---

## 설계 원칙

- 각 레이어는 인터페이스(I-prefix) + 구체 구현으로 분리
- `.hpp` 에 `#include` 최소화 — forward declaration 선호
- 레이어는 최소로 유지, 불필요한 중간 레이어 추가 금지
- `main()` 이 Composition Root — 구체 타입을 아는 유일한 지점
