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
    // Movement ������Ʈ ã�� (�ִٸ�)
    m_movement = GetOwner()->GetComponent<Movement>();
}

void InputComponent::Update(float deltaTime)
{
    if (!IsActive()) return;
    Component::Update(deltaTime);

    UpdateKeyState();

    // �¿� Ű ó��
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

    // ���� Ű ó�� (Pressed�� ���� ���� - ���� ���� ����)
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
    bool jumpDown = IsKeyDown(VK_SPACE);  // �����̽��ٸ� ���� Ű�� ���

    // ���� Ű ���� ������Ʈ
    if (leftDown && !m_prevLeftKey)
        m_leftState = KeyState::Pressed;
    else if (leftDown && m_prevLeftKey)
        m_leftState = KeyState::Hold;
    else if (!leftDown && m_prevLeftKey)
        m_leftState = KeyState::Released;
    else
        m_leftState = KeyState::None;

    // ������ Ű ���� ������Ʈ
    if (rightDown && !m_prevRightKey)
        m_rightState = KeyState::Pressed;
    else if (rightDown && m_prevRightKey)
        m_rightState = KeyState::Hold;
    else if (!rightDown && m_prevRightKey)
        m_rightState = KeyState::Released;
    else
        m_rightState = KeyState::None;

    // ���� Ű ���� ������Ʈ
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

    // Movement ������Ʈ�� �ڵ� ����
    SetOnMoveLeft([this]() {
        if (m_movement) {
            m_movement->SetVelocity(-1, 0);  // �������� �̵�
        }
        });

    SetOnMoveRight([this]() {
        if (m_movement) {
            m_movement->SetVelocity(1, 0);   // ���������� �̵�
        }
        });

    // Ű�� ���� �� ����
    SetOnLeftReleased([this]() {
        if (m_movement && !IsRightHold()) {
            m_movement->SetVelocity(0, 0);   // ����
        }
        });

    SetOnRightReleased([this]() {
        if (m_movement && !IsLeftHold()) {
            m_movement->SetVelocity(0, 0);   // ����
        }
        });
}

/*
��� ���� (���� ��� ����):

// GameObject�� InputComponent �߰�
auto inputComp = gameObject->AddComponent<InputComponent>();

// ���ٷ� �Է� ó�� ���
inputComp->SetOnMoveLeft([this]() {
    // ������ ���� �̵� ����
    if (physicsComponent) {
        physicsComponent->AddVelocity(-moveSpeed, 0);
    }
    SetDirection(-1);
});

inputComp->SetOnMoveRight([this]() {
    // ������ ������ �̵� ����
    if (physicsComponent) {
        physicsComponent->AddVelocity(moveSpeed, 0);
    }
    SetDirection(1);
});

// ���� ó��
inputComp->SetOnJump([this]() {
    if (physicsComponent) {
        physicsComponent->Jump();  // PhysicsComponent�� Jump() ȣ��
    }
});

// ���� Ű�� ���� ���� ó�� (ª�� ���� ���� ��)
inputComp->SetOnJumpReleased([this]() {
    if (physicsComponent && physicsComponent->GetVelocity().y < 0) {
        // ���� Ű�� ���� ���� ���� ���̸� ����
        auto vel = physicsComponent->GetVelocity();
        vel.y *= 0.5f;  // ���� �ӵ� �ݰ�
        physicsComponent->SetVelocity(vel);
    }
});

// ���� ���� üũ�� ����
if (inputComp->IsJumpPressed()) {
    // ���� Ű�� ������ ���� ó��
}
*/
