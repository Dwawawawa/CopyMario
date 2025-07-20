// 32InputComponent.cpp
#include "00pch.h"
#include "32InputComponent.h"
#include <windows.h> // Windows API for input

InputComponent::InputComponent()
{
}

void InputComponent::Initialize()
{
    Component::Initialize();
    // Movement 컴포넌트 찾기 (있다면)
    m_movement = GetOwner()->GetComponent<Movement>();
}

void InputComponent::Update(float deltaTime)
{
    if (!IsActive()) return;
    Component::Update(deltaTime);

    UpdateKeyState();

    // 좌우 키 처리
    if (m_leftState == KeyState::Hold && m_onLeftCallback)
    {
        m_onLeftCallback();
    }
    if (m_rightState == KeyState::Hold && m_onRightCallback)
    {
        m_onRightCallback();
    }
    if (m_leftState == KeyState::Released && m_onLeftReleasedCallback)
    {
        m_onLeftReleasedCallback();
    }
    if (m_rightState == KeyState::Released && m_onRightReleasedCallback)
    {
        m_onRightReleasedCallback();
    }

    // 점프 키 처리 (Pressed일 때만 점프 - 연속 점프 방지)
    if (m_jumpState == KeyState::Pressed && m_onJumpCallback)
    {
        m_onJumpCallback();
    }
    if (m_jumpState == KeyState::Released && m_onJumpReleasedCallback)
    {
        m_onJumpReleasedCallback();
    }
}

void InputComponent::Release()
{
    m_onLeftCallback = nullptr;
    m_onRightCallback = nullptr;
    m_onLeftReleasedCallback = nullptr;
    m_onRightReleasedCallback = nullptr;
    m_onJumpCallback = nullptr;
    m_onJumpReleasedCallback = nullptr;
    Component::Release();
}

void InputComponent::UpdateKeyState()
{
    bool leftDown = IsKeyDown(VK_LEFT);
    bool rightDown = IsKeyDown(VK_RIGHT);
    bool jumpDown = IsKeyDown(VK_SPACE);  // 스페이스바를 점프 키로 사용

    // 왼쪽 키 상태 업데이트
    if (leftDown && !m_prevLeftKey)
        m_leftState = KeyState::Pressed;
    else if (leftDown && m_prevLeftKey)
        m_leftState = KeyState::Hold;
    else if (!leftDown && m_prevLeftKey)
        m_leftState = KeyState::Released;
    else
        m_leftState = KeyState::None;

    // 오른쪽 키 상태 업데이트
    if (rightDown && !m_prevRightKey)
        m_rightState = KeyState::Pressed;
    else if (rightDown && m_prevRightKey)
        m_rightState = KeyState::Hold;
    else if (!rightDown && m_prevRightKey)
        m_rightState = KeyState::Released;
    else
        m_rightState = KeyState::None;

    // 점프 키 상태 업데이트
    if (jumpDown && !m_prevJumpKey)
        m_jumpState = KeyState::Pressed;
    else if (jumpDown && m_prevJumpKey)
        m_jumpState = KeyState::Hold;
    else if (!jumpDown && m_prevJumpKey)
        m_jumpState = KeyState::Released;
    else
        m_jumpState = KeyState::None;

    m_prevLeftKey = leftDown;
    m_prevRightKey = rightDown;
    m_prevJumpKey = jumpDown;
}

bool InputComponent::IsKeyDown(int virtualKey) const
{
    return (GetAsyncKeyState(virtualKey) & 0x8000) != 0;
}

void InputComponent::SetupWithMovement(float moveSpeed)
{
    m_moveSpeed = moveSpeed;

    // Movement 컴포넌트와 자동 연동
    SetOnMoveLeft([this]() {
        if (m_movement) {
            m_movement->SetVelocity(-1, 0);  // 왼쪽으로 이동
        }
        });

    SetOnMoveRight([this]() {
        if (m_movement) {
            m_movement->SetVelocity(1, 0);   // 오른쪽으로 이동
        }
        });

    // 키를 뗐을 때 정지
    SetOnLeftReleased([this]() {
        if (m_movement && !IsRightHold()) {
            m_movement->SetVelocity(0, 0);   // 정지
        }
        });

    SetOnRightReleased([this]() {
        if (m_movement && !IsLeftHold()) {
            m_movement->SetVelocity(0, 0);   // 정지
        }
        });
}

/*
사용 예제 (점프 기능 포함):

// GameObject에 InputComponent 추가
auto inputComp = gameObject->AddComponent<InputComponent>();

// 람다로 입력 처리 등록
inputComp->SetOnMoveLeft([this]() {
    // 마리오 왼쪽 이동 로직
    if (physicsComponent) {
        physicsComponent->AddVelocity(-moveSpeed, 0);
    }
    SetDirection(-1);
});

inputComp->SetOnMoveRight([this]() {
    // 마리오 오른쪽 이동 로직
    if (physicsComponent) {
        physicsComponent->AddVelocity(moveSpeed, 0);
    }
    SetDirection(1);
});

// 점프 처리
inputComp->SetOnJump([this]() {
    if (physicsComponent) {
        physicsComponent->Jump();  // PhysicsComponent의 Jump() 호출
    }
});

// 점프 키를 뗐을 때의 처리 (짧은 점프 구현 등)
inputComp->SetOnJumpReleased([this]() {
    if (physicsComponent && physicsComponent->GetVelocity().y < 0) {
        // 점프 키를 빨리 떼면 점프 높이를 줄임
        auto vel = physicsComponent->GetVelocity();
        vel.y *= 0.5f;  // 점프 속도 반감
        physicsComponent->SetVelocity(vel);
    }
});

// 직접 상태 체크도 가능
if (inputComp->IsJumpPressed()) {
    // 점프 키가 눌렸을 때의 처리
}
*/
