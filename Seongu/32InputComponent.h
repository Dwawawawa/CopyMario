// 32InputComponent.h
#include "00pch.h"
#include "31Component.h"
#include "32Movement.h"
#pragma once

enum class KeyState
{
    None,       // 키가 눌리지 않은 상태
    Pressed,    // 이번 프레임에 눌림
    Hold,       // 지속적으로 눌리고 있음
    Released    // 이번 프레임에 뗌
};

class InputComponent : public Component
{
public:
    // 람다 콜백 타입 정의
    using KeyCallback = std::function<void()>;

    InputComponent();
    ~InputComponent() override = default;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Release() override;

    // 좌우 키 상태 체크
    bool IsLeftPressed() const { return m_leftState == KeyState::Pressed; }
    bool IsLeftHold() const { return m_leftState == KeyState::Hold; }
    bool IsLeftReleased() const { return m_leftState == KeyState::Released; }
    bool IsRightPressed() const { return m_rightState == KeyState::Pressed; }
    bool IsRightHold() const { return m_rightState == KeyState::Hold; }
    bool IsRightReleased() const { return m_rightState == KeyState::Released; }

    // 점프 키 상태 체크
    bool IsJumpPressed() const { return m_jumpState == KeyState::Pressed; }
    bool IsJumpHold() const { return m_jumpState == KeyState::Hold; }
    bool IsJumpReleased() const { return m_jumpState == KeyState::Released; }

    // 람다 콜백 등록
    void SetOnMoveLeft(KeyCallback callback) { m_onLeftCallback = callback; }
    void SetOnMoveRight(KeyCallback callback) { m_onRightCallback = callback; }
    void SetOnLeftReleased(KeyCallback callback) { m_onLeftReleasedCallback = callback; }
    void SetOnRightReleased(KeyCallback callback) { m_onRightReleasedCallback = callback; }

    // 점프 콜백 등록
    void SetOnJump(KeyCallback callback) { m_onJumpCallback = callback; }
    void SetOnJumpReleased(KeyCallback callback) { m_onJumpReleasedCallback = callback; }

    // Movement 컴포넌트와 연동하는 편의 함수
    void SetupWithMovement(float moveSpeed = 200.0f);

private:
    // 키 상태
    KeyState m_leftState = KeyState::None;
    KeyState m_rightState = KeyState::None;
    KeyState m_jumpState = KeyState::None;  // 점프 키 상태 추가

    bool m_prevLeftKey = false;
    bool m_prevRightKey = false;
    bool m_prevJumpKey = false;  // 점프 키 이전 상태 추가

    // 콜백들
    KeyCallback m_onLeftCallback;
    KeyCallback m_onRightCallback;
    KeyCallback m_onLeftReleasedCallback;
    KeyCallback m_onRightReleasedCallback;
    KeyCallback m_onJumpCallback;  // 점프 콜백 추가
    KeyCallback m_onJumpReleasedCallback;  // 점프 릴리즈 콜백 추가

    // Movement 컴포넌트 연동용
    Movement* m_movement = nullptr;
    float m_moveSpeed = 200.0f;

    void UpdateKeyState();
    bool IsKeyDown(int virtualKey) const;
};