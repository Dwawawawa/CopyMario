# SeongU(게임)와 SSEngine(엔진) : "슈퍼마리오 브라더스" 모작
**2Q 미니 프로젝트 대비 및 엔진 제작 숙련도 높이기 위한 모작 프로젝트**

---
**GitHub Repository:** https://github.com/Dwawawawa/CopyMario

## 📋 프로젝트 개요
**개발 기간:** 2025.07.11 ~ 2025.07.22 (11일)  
**개발 목적:** 게임 엔진 아키텍처 이해 및 컴포넌트 기반 설계 학습  
**장르:** 2D 플랫포머 액션 게임  
**기술 스택:** C++17, Direct2D, Visual Studio 2022

## 🎯 과제 요구사항 구현 현황

### ✅ 구현 완료된 항목

#### 1. 게임 루프 ✅
- Direct2D 기반 메인 게임 루프 구현
- 60FPS 타겟의 안정적인 프레임 관리
- Update-Render 분리된 게임 사이클

#### 2. 씬 - 씬 매니저 ✅
- 씬 매니저를 통한 씬 전환 시스템
- IntroScene, MainGameScene, OutroScene 구현
- 시간 기반 자동 씬 전환 기능

#### 5. 트랜스폼 계층구조 ✅
- 부모: TransformComponent와 자식: PhysicsComponent, ColliderComponent, MovementComponent
- 유니티에서 리지드 바디 2D로 하나의 컴포넌트로 구성하는 이유를 학습


#### 7. 씬->게임오브젝트->컴포넌트 구조 ✅
- 완전한 컴포넌트 기반 아키텍처 구현
- GameObject를 통한 컴포넌트 관리
- 씬 내에서 GameObject 생명주기 관리


**물리 컴포넌트 (PhysicsComponent)**
- 중력, 속도, 가속도 시뮬레이션
- 점프 메커니즘 구현

**콜라이더 컴포넌트 (ColliderComponent)**  
- AABB 기반 충돌 검사
- 게임오브젝트 간 상호작용 처리

**무브먼트 컴포넌트 (MovementComponent)**
- 플레이어 입력 기반 이동 처리
- 물리 시스템과 연동된 움직임

**트랜스폼 컴포넌트 (TransformComponent)**
- 위치, 크기, 회전 정보 관리
- 모든 컴포넌트의 기반이 되는 핵심 컴포넌트

### ⚠️ 미완성된 항목 및 사유

#### 3. 리소스 관리 ❌
**미완성 사유:** 컴포넌트 시스템 구현에 집중하느라 시간 부족
- 향후 AssetManager 패턴으로 리팩토링 예정

#### 4. 애니메이션 ❌  
**미완성 사유:** 물리/충돌 시스템 구현이 예상보다 복잡했음
- 애니메이션 프레임워크는 설계 완료, 구현 대기 중

#### 6. 입력 처리 FSM ❌
**미완성 사유:** 기본 입력 처리에 집중, FSM까지는 시간 부족
- 기본적인 키보드 입력 처리는 구현
- 상태 머신 패턴은 설계만 완료


## 🏗️ 아키텍처 설계

### 프로젝트 구조
```
📦 CopyMario
├── 📂 SeongU (게임 프로젝트)
│   ├── 📄 main.cpp
│   ├── 📄 GameProcess.h/cpp
│   ├── 📂 Scenes/
│   └── 📂 GameObjects/
├── 📂 SSEngine (엔진 프로젝트)
│   └── 📄 Engine.h/cpp
└── 📂 Resources/
```

### 컴포넌트 시스템 설계
```cpp
// 예시: GameObject-Component 관계
class GameObject {
private:
    std::vector<std::unique_ptr<Component>> m_components;
    
public:
    template<typename T>
    T* AddComponent();
    
    template<typename T>
    T* GetComponent();
    
    void Update(float deltaTime);
    void Render(ID2D1RenderTarget* renderTarget);
};
```

## 🎮 현재 구현된 기능
- ✅ 씬 매니저를 통한 씬 전환
- ✅ 컴포넌트 기반 게임오브젝트 시스템
- ✅ 기본 물리 시뮬레이션 (중력, 점프)
- ✅ AABB 충돌 검사 시스템
- ✅ 플레이어 캐릭터 기본 조작
- ✅ Direct2D 기반 렌더링

## 🔧 기술적 도전과 해결
### 1. LNK2019 링킹 에러 해결
**문제:** 컴포넌트 클래스들의 링킹 에러 빈발
**해결:** 파일명 규칙 정립 및 헤더-구현 분리 체계화

### 2. 컴포넌트 간 의존성 관리
**문제:** PhysicsComponent와 MovementComponent 간 순환 참조
**해결:** 컴포넌트 간 통신을 GameObject를 통해 중재하는 방식으로 설계

### 3. Direct2D 좌표계 이해
**문제:** 게임 좌표계와 Direct2D 좌표계 간 차이로 인한 렌더링 문제
**해결:** TransformComponent에서 좌표 변환 로직 구현

## 📝 개발 과정에서 배운 점
1. **컴포넌트 기반 아키텍처의 장단점**
   - 확장성과 모듈성이 뛰어나지만 초기 설계의 중요성을 깨달음
   - 컴포넌트 간 통신 방식에 대한 깊은 고민 필요

2. **게임 엔진 설계의 복잡성**
   - 단순해 보이는 기능도 엔진 레벨에서는 많은 고려사항 존재
   - 성능과 유지보수성 사이의 균형점 찾기

3. **실무와 이론의 차이**
   - 이론적으로는 간단한 시스템도 실제 구현에서는 예상치 못한 복잡성 발생

## 🚀 향후 개발 계획
2Q 미니 프로젝트를 완료하고 방학동안 마무리하겠습니다. 

## 📊 결론
비록 모든 요구사항을 완성하지는 못했지만, 게임 엔진의 핵심인 **컴포넌트 기반 아키텍처**를 깊이 있게 구현하면서 게임 엔진 설계에 대한 실질적인 이해를 얻을 수 있었습니다. 특히 GameObject-Component 시스템을 통해 확장 가능하고 유지보수가 용이한 게임 구조를 만들어낸 것이 이번 프로젝트의 가장 큰 성과라고 생각합니다.

