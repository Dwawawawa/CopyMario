// 32InputComponent.h
#include "00pch.h"
#include "31Component.h"
#include "32Movement.h"
#include <string_view>
#include <optional>
#include "../SS_D2DEngine/magic_enum/magic_enum_all.hpp"
#pragma once

enum class KeyState
{
    None,       // 키가 눌리지 않은 상태
    Pressed,    // 이번 프레임에 눌림
    Hold,       // 지속적으로 눌리고 있음
    Released    // 이번 프레임에 뗌
};

// 입력 키 타입을 enum으로 정의
enum class InputKey : std::uint32_t
{
    Left = 1,
    Right = 2,
    Jump = 4,
    Action = 8,
    Menu = 16,
    Pause = 32
};

template<>
struct magic_enum::customize::enum_range<InputKey>
{
    static constexpr bool is_flags = true;
};

class InputComponent : public Component
{
public:
    // 람다 콜백 타입 정의
    using KeyCallback = std::function<void(InputKey)>;
    using StateCallback = std::function<void(InputKey,KeyState)>;

    InputComponent();
    ~InputComponent() override = default;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Release() override;

    // 방법 1: Magic Enum을 이용한 범용 키 상태 체크
    KeyState GetKeyState(InputKey key) const;
    bool IsKeyPressed(InputKey key) const { return GetKeyState(key) == KeyState::Pressed; };
    bool IsKeyHold(InputKey key) const { return GetKeyState(key) == KeyState::Hold; };
    bool IsKeyReleased(InputKey key) const { return GetKeyState(key) == KeyState::Released;};

    // 방법 2: 문자열로 키 상태 조회 (디버깅/설정 파일용)
    KeyState GetKeyState(std::string_view keyName) const;
    bool IsKeyPressed(std::string_view keyName) const;

    // 방법 3: 여러 키 조합 체크
    bool AreKeysPressed(std::string_view keyFlags) const;
    InputKey GetPressedKeys() const;
    
    // 콜백 등록 = Magic Enum 활용
    void RegisterKeyCallback(InputKey key, KeyState state, KeyCallback callback);
    void RegisterKeyCallback( std::string_view keyName, std::string_view stateName, KeyCallback callback);
    
    // 범용 상태 변화 콜백
    void SetOnStateChanged(StateCallback callback){ }

    // Movement 연동
    void SetupWithMovement(float moveSpeed = 200.0f);

    // 디버깅용 - 현재 모든 키 상태를 문자열로 반환
    std::string GetAllKeyStatesAsString() const;

private:
    // 키별 상태 저장
    std::unordered_map<InputKey, KeyState> m_keyStates;
    std::unordered_map<InputKey, bool> m_prevKeyStates;

    // 콜백 저장 구조
    struct CallbackInfo
    {
        InputKey key;
        KeyState triggerState;
        KeyCallback callback;
    };
    std::vector<CallbackInfo> m_callbacks;
    StateCallback m_onStateChanged;

    // Movement 컴포넌트 연동용
    Movement* m_movement = nullptr;
    float m_moveSpeed = 200.0f;

    // 내부 메서드들
    void UpdateKeyStates();
    bool IsVirtualKeyDown(InputKey key) const;
    int  GetVirtualKeyCode(InputKey key) const;
    void ProcessCallbacks();

    // Magic Enum 헬퍼 메서드들
    std::optional<InputKey> ParseInputKey(std::string_view keyName) const;
    std::optional<KeyState> ParseKeyState(std::string_view stateName) const;
};