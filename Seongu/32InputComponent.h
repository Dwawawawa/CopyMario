// 32InputComponent.h
#include "00pch.h"
#include "31Component.h"
#include "32Movement.h"
#pragma once

enum class KeyState
{
    None,       // Ű�� ������ ���� ����
    Pressed,    // �̹� �����ӿ� ����
    Hold,       // ���������� ������ ����
    Released    // �̹� �����ӿ� ��
};

class InputComponent : public Component
{
public:
    // ���� �ݹ� Ÿ�� ����
    using KeyCallback = std::function<void()>;

    InputComponent();
    ~InputComponent() override = default;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Release() override;

    // �¿� Ű ���� üũ
    bool IsLeftPressed() const { return m_leftState == KeyState::Pressed; }
    bool IsLeftHold() const { return m_leftState == KeyState::Hold; }
    bool IsLeftReleased() const { return m_leftState == KeyState::Released; }
    bool IsRightPressed() const { return m_rightState == KeyState::Pressed; }
    bool IsRightHold() const { return m_rightState == KeyState::Hold; }
    bool IsRightReleased() const { return m_rightState == KeyState::Released; }

    // ���� Ű ���� üũ
    bool IsJumpPressed() const { return m_jumpState == KeyState::Pressed; }
    bool IsJumpHold() const { return m_jumpState == KeyState::Hold; }
    bool IsJumpReleased() const { return m_jumpState == KeyState::Released; }

    // ���� �ݹ� ���
    void SetOnMoveLeft(KeyCallback callback) { m_onLeftCallback = callback; }
    void SetOnMoveRight(KeyCallback callback) { m_onRightCallback = callback; }
    void SetOnLeftReleased(KeyCallback callback) { m_onLeftReleasedCallback = callback; }
    void SetOnRightReleased(KeyCallback callback) { m_onRightReleasedCallback = callback; }

    // ���� �ݹ� ���
    void SetOnJump(KeyCallback callback) { m_onJumpCallback = callback; }
    void SetOnJumpReleased(KeyCallback callback) { m_onJumpReleasedCallback = callback; }

    // Movement ������Ʈ�� �����ϴ� ���� �Լ�
    void SetupWithMovement(float moveSpeed = 200.0f);

private:
    // Ű ����
    KeyState m_leftState = KeyState::None;
    KeyState m_rightState = KeyState::None;
    KeyState m_jumpState = KeyState::None;  // ���� Ű ���� �߰�

    bool m_prevLeftKey = false;
    bool m_prevRightKey = false;
    bool m_prevJumpKey = false;  // ���� Ű ���� ���� �߰�

    // �ݹ��
    KeyCallback m_onLeftCallback;
    KeyCallback m_onRightCallback;
    KeyCallback m_onLeftReleasedCallback;
    KeyCallback m_onRightReleasedCallback;
    KeyCallback m_onJumpCallback;  // ���� �ݹ� �߰�
    KeyCallback m_onJumpReleasedCallback;  // ���� ������ �ݹ� �߰�

    // Movement ������Ʈ ������
    Movement* m_movement = nullptr;
    float m_moveSpeed = 200.0f;

    void UpdateKeyState();
    bool IsKeyDown(int virtualKey) const;
};