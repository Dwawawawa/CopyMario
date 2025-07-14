# SeongU(게임)와 SSEngine(엔진) : "슈퍼마리오 브라더스" 모작
2Q 미니 프로젝트 대비 및 엔진 제작 숙련도 높이기 위한 모작 프로젝트입니다.

## 📋 프로젝트 개요
**개발 기간:** 2025.07.11 ~ 2025.07.15 (5일)  
**개발 목적:** 미니 프로젝트 대비 및 엔진 제작 숙련도 향상  
**장르:** 2D 플랫포머 액션 게임  

## 🌿 브랜치 관리 전략
개발 진행 단계별로 브랜치를 분리하여 학습 과정을 체계적으로 관리합니다.

### 주요 브랜치 구조
| 브랜치명 | 설명 | 상태 |
|---------|------|------|
| `main` | 최종 완성 버전 | 🎯 완료 |
| `OnlySeongu` | 엔진 분리 없이 게임로직만 구현 | 🎯 완료  |
| `BasicEngine` | SSEngine 기본 구조 분리 | 🔄 진행중 |
| `RenderFocus` | 렌더링 시스템 집중 개발 | 📋 예정 |
| `AnimationMaster` | 애니메이션 시스템 완성 | 📋 예정 |
| `ShaderMario` | 쉐이더 적용된 마리오 구현 | 📋 예정 |
| `PhysicsComplete` | 물리 엔진 완성 | 📋 예정 |
| `ThreeScenes` | 3개 씬 구현 완료 | 📋 예정 |
| `PolishedGame` | 최종 다듬기 및 최적화 | 📋 예정 |

### 브랜치 전환 가이드
```bash
# 현재 브랜치 확인
git branch

# 특정 브랜치로 전환
git checkout OnlySeongu
git checkout BasicEngine
git checkout RenderFocus

# 새 브랜치 생성하면서 전환
git checkout -b [브랜치명]
```

## 🎯 개발 목표
### 구현 범위
- ✅ 사운드 제외 (시각적 요소에 집중)
- ✅ 3개 씬 구현
  1. **인트로 씬** - 타이틀 화면
  2. **스테이지 1-1** - 메인 게임플레이
  3. **아웃트로 씬** - 엔딩 화면

### 핵심 기능
2Q에서 맡은 파트가 랜더이기 때문에 게임로직과 엔진 파트는 "바이브 코딩"하고자 합니다. 
애니메이션을 정확히 만들어서 움직임이 부드럽게 만들며, 쉐이더를 이용하여 이쁜 마리오를 목표로 하겠습니다. 
- [ ] 애니메이션 시스템
- [ ] 스타 먹으면 반짝이는 마리오
      
- [ ] 플레이어 캐릭터 (마리오) 조작
- [ ] 물리 엔진 (점프, 중력, 충돌)
- [ ] 적 캐릭터 AI (굼바 등)
- [ ] 아이템 시스템 (코인, 버섯, 꽃)
- [ ] 맵 시스템 (타일 기반)
- [ ] 씬 전환 시스템

## 🛠️ 기술 스택
**엔진:** 자체 제작 SSEngine (Direct2D 기반)  
**언어:** C++17  
**그래픽스:** Direct2D, DirectWrite  
**플랫폼:** Windows 10/11  
**IDE:** Visual Studio 2022  

## 📁 프로젝트 구조
```
// 여기는 수정될 수 있음
📦 SuperMarioBros
├── 📂 SeongU (게임 프로젝트)
│   ├── 📄 main.cpp
│   ├── 📄 GameProcess.h/cpp
│   ├── 📂 Scenes
│   │   ├── 📄 IntroScene.h/cpp
│   │   ├── 📄 Stage1_1Scene.h/cpp
│   │   └── 📄 OutroScene.h/cpp
│   └── 📂 GameObjects
│       ├── 📄 Player.h/cpp
│       ├── 📄 Enemy.h/cpp
│       └── 📄 Item.h/cpp
├── 📂 SSEngine (엔진 프로젝트)
│   ├── 📄 10EngineD2D.h/cpp
│   ├── 📂 Core
│   ├── 📂 Render
│   ├── 📂 Input
│   ├── 📂 Physics
│   └── 📂 Utils
├──📂 Resources
│   ├── 📂 Images
│   └── 📂 Maps
└── 📄 README.md
```

## 🎮 게임 조작법
| 키 | 기능 |
|---|---|
| `A` / `←` | 왼쪽 이동 |
| `D` / `→` | 오른쪽 이동 |
| `Space` / `↑` | 점프 |
| `Enter` | 게임 시작 / 일시정지 |
| `ESC` | 게임 종료 |

## 📝 개발 일지
### Day 1 (2025.07.11)
- [x] 프로젝트 셋업 및 구조 설계
- [x] SSEngine 기본 구조 구현
- [x] Direct2D 초기화 및 기본 렌더링
- [x] `OnlySeongu` 브랜치 생성 및 기본 게임로직 구현
- [x] 게임타이머 갖고 옴


### Day 2 (2025.07.12)
- [x] 휴식

### Day 3 (2025.07.13)
- [x] 휴식
- [ ] NzWndBase를 넣고, 랜더러 구조 바꾸고 등등

### Day 4 (2025.07.14)
- [ ] 시간이 지나면 씬 넘어감 그렇게 인트로 -> 메인 -> 아웃트로까지 가능함
- [ ] 플레이어 캐릭터 기본 이동 구현
- [ ] 입력 시스템 구현
- [ ] 기본 물리 시스템 구현
- [ ] `BasicEngine` 브랜치 생성 및 엔진 분리 작업
- [ ] 맵 시스템 및 충돌 검사
- [ ] 적 캐릭터 AI 구현
- [ ] 아이템 시스템 구현
- [ ] `RenderFocus` 브랜치에서 렌더링 시스템 집중 개발- [ ] 3개 씬 구현 및 전환
- [ ] 애니메이션 시스템 구현
- [ ] 게임 로직 완성
- [ ] `AnimationMaster` 및 `ShaderMario` 브랜치 작업

### Day 5 (2025.07.15)
- [ ] 버그 수정 및 최적화
- [ ] 코드 리팩토링
- [ ] 프로젝트 완료 및 문서 정리
- [ ] `PolishedGame` 브랜치에서 최종 마무리

## 🎯 학습 목표
### 엔진 개발 관련
- Direct2D를 활용한 2D 렌더링 시스템 구축
- 게임 오브젝트 관리 시스템 설계
- 씬 매니저 구현
- 입력 처리 시스템 구현

### 게임 개발 관련
- 2D 플랫포머 게임 메커니즘 구현
- 물리 엔진 기초 구현
- 상태 머신 패턴 활용
- 게임 루프 최적화

## 🔧 주요 도전 과제
- [ ] 픽셀 퍼펙트 충돌 검사 구현
- [ ] 부드러운 애니메이션 시스템 구현
- [ ] 효율적인 타일 기반 맵 렌더링
- [ ] 게임 오브젝트 풀링 시스템

## 📚 참고 자료
- [Direct2D 공식 문서](https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-portal)
- [슈퍼마리오 브라더스 게임 분석](https://www.youtube.com/watch?v=zRGRJRUWafY)
- [2D 게임 물리 엔진 이론](https://www.toptal.com/game/video-game-physics-part-i-an-introduction-to-rigid-body-dynamics)

- [당신의 안녕을 위하여](https://github.com/hajineys/For-Your-Tranquility)

---
**⭐ 이 프로젝트가 도움이 되었다면 스타를 눌러주세요!**
