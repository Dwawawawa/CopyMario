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
    None,       // Ű�� ������ ���� ����
    Pressed,    // �̹� �����ӿ� ����
    Hold,       // ���������� ������ ����
    Released    // �̹� �����ӿ� ��
};

// �Է� Ű Ÿ���� enum���� ����
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
    // ���� �ݹ� Ÿ�� ����
    using KeyCallback = std::function<void(InputKey)>;
    using StateCallback = std::function<void(InputKey,KeyState)>;

    InputComponent();
    ~InputComponent() override = default;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Release() override;

    // ��� 1: Magic Enum�� �̿��� ���� Ű ���� üũ
    KeyState GetKeyState(InputKey key) const;
    bool IsKeyPressed(InputKey key) const { return GetKeyState(key) == KeyState::Pressed; };
    bool IsKeyHold(InputKey key) const { return GetKeyState(key) == KeyState::Hold; };
    bool IsKeyReleased(InputKey key) const { return GetKeyState(key) == KeyState::Released;};

    // ��� 2: ���ڿ��� Ű ���� ��ȸ (�����/���� ���Ͽ�)
    KeyState GetKeyState(std::string_view keyName) const;
    bool IsKeyPressed(std::string_view keyName) const;

    // ��� 3: ���� Ű ���� üũ
    bool AreKeysPressed(std::string_view keyFlags) const;
    InputKey GetPressedKeys() const;
    
    // �ݹ� ��� = Magic Enum Ȱ��
    void RegisterKeyCallback(InputKey key, KeyState state, KeyCallback callback);
    void RegisterKeyCallback( std::string_view keyName, std::string_view stateName, KeyCallback callback);
    
    // ���� ���� ��ȭ �ݹ�
    void SetOnStateChanged(StateCallback callback){ }

    // Movement ����
    void SetupWithMovement(float moveSpeed = 200.0f);

    // ������ - ���� ��� Ű ���¸� ���ڿ��� ��ȯ
    std::string GetAllKeyStatesAsString() const;

private:
    // Ű�� ���� ����
    std::unordered_map<InputKey, KeyState> m_keyStates;
    std::unordered_map<InputKey, bool> m_prevKeyStates;

    // �ݹ� ���� ����
    struct CallbackInfo
    {
        InputKey key;
        KeyState triggerState;
        KeyCallback callback;
    };
    std::vector<CallbackInfo> m_callbacks;
    StateCallback m_onStateChanged;

    // Movement ������Ʈ ������
    Movement* m_movement = nullptr;
    float m_moveSpeed = 200.0f;

    // ���� �޼����
    void UpdateKeyStates();
    bool IsVirtualKeyDown(InputKey key) const;
    int  GetVirtualKeyCode(InputKey key) const;
    void ProcessCallbacks();

    // Magic Enum ���� �޼����
    std::optional<InputKey> ParseInputKey(std::string_view keyName) const;
    std::optional<KeyState> ParseKeyState(std::string_view stateName) const;
};