// 32InputComponent.cpp
#include "00pch.h"
#include "32InputComponent.h"
#include <windows.h> // Windows API for input

InputComponent::InputComponent()
{
    for (auto key : magic_enum::enum_values<InputKey>())
    {
        m_keyStates[key] = KeyState::None;
        m_prevKeyStates[key] = false;
    }
}

void InputComponent::Initialize()
{
    Component::Initialize();
    // Movement 컴포넌트 찾기 (있다면)
    m_movement = GetOwner()->GetComponent<Movement>();


    RegisterKeyCallback(InputKey::Left, KeyState::Hold, [this](InputKey){
        if (m_movement) m_movement->SetVelocity(-m_moveSpeed,0);
        });

    RegisterKeyCallback(InputKey::Right, KeyState::Hold, [this](InputKey) {
        if (m_movement) m_movement->SetVelocity(m_moveSpeed, 0);
        });

    RegisterKeyCallback(InputKey::Left, KeyState::Released, [this](InputKey) {
        if (m_movement && !IsKeyHold(InputKey::Right)) 
            m_movement->SetVelocity(0);
        });

    RegisterKeyCallback(InputKey::Right, KeyState::Released, [this](InputKey) {
        if (m_movement && !IsKeyHold(InputKey::Left))
            m_movement->SetVelocity(0);
        });

}

void InputComponent::Update(float deltaTime)
{
    UpdateKeyStates();
    ProcessCallbacks();

    ////debug
    //static float tempTime = 0;
    //tempTime += deltaTime;
    //if (tempTime > 0.5f)
    //{
    //    //std::cout << debugInput->GetAllKeyStatesAsString()<< std::endl;
    //    std::cout << GetAllKeyStatesAsString() << std::endl;

    //    tempTime = 0;
    //}
}

void InputComponent::Release()
{
    Component::Initialize();
}

KeyState InputComponent::GetKeyState(InputKey key) const
{
    auto it = m_keyStates.find(key);
    return it != m_keyStates.end() ? it->second : KeyState::None;
}

KeyState InputComponent::GetKeyState(std::string_view keyName) const
{
    auto key = ParseInputKey(keyName);
    return key ? GetKeyState(*key) : KeyState::None;
}

bool InputComponent::IsKeyPressed(std::string_view keyName) const
{
    return GetKeyState(keyName) == KeyState::Pressed;
}

bool InputComponent::AreKeysPressed(std::string_view keyFlags) const
{
    // "Left|Right|Jump" 형태의 문자열을 파싱
    auto parsedKeys = magic_enum::enum_flags_cast<InputKey>(keyFlags);
    if (!parsedKeys) return false;

    // 각 키가 모두 눌렸는지 확인
    for (auto key : magic_enum::enum_values<InputKey>()) {
        if (magic_enum::enum_flags_test(*parsedKeys, key)) {
            if (!IsKeyPressed(key) && !IsKeyHold(key)) {
                return false;
            }
        }
    }
    return true;
}

InputKey InputComponent::GetPressedKeys() const
{
    InputKey result = static_cast<InputKey>(0);

    for (const auto& [key, state] : m_keyStates) {
        if (state == KeyState::Pressed || state == KeyState::Hold) {
            result = static_cast<InputKey>(
                static_cast<std::underlying_type_t<InputKey>>(result) |
                static_cast<std::underlying_type_t<InputKey>>(key)
                );
        }
    }
    return result;
}

void InputComponent::RegisterKeyCallback(InputKey key, KeyState state, KeyCallback callback)
{
    m_callbacks.push_back({ key, state, callback });
}

void InputComponent::RegisterKeyCallback(std::string_view keyName, std::string_view stateName, KeyCallback callback)
{
    auto key = ParseInputKey(keyName);
    auto state = ParseKeyState(stateName);

    if (key && state) {
        RegisterKeyCallback(*key, *state, callback);
    }
}

void InputComponent::SetupWithMovement(float moveSpeed)
{
    m_moveSpeed = moveSpeed;
}

std::string InputComponent::GetAllKeyStatesAsString() const
{
    std::string result = "Key States: ";

    for (const auto& [key, state] : m_keyStates) {
        if (state != KeyState::None) {
            result += std::string(magic_enum::enum_name(key)) + "=" +
                std::string(magic_enum::enum_name(state)) + " ";
        }
    }

    return result;
}

void InputComponent::UpdateKeyStates()
{
    for (auto key : magic_enum::enum_values<InputKey>()) {
        bool currentDown = IsVirtualKeyDown(key);
        bool prevDown = m_prevKeyStates[key];

        if (currentDown && !prevDown) {
            m_keyStates[key] = KeyState::Pressed;
        }
        else if (currentDown && prevDown) {
            m_keyStates[key] = KeyState::Hold;
        }
        else if (!currentDown && prevDown) {
            m_keyStates[key] = KeyState::Released;
        }
        else {
            m_keyStates[key] = KeyState::None;
        }

        m_prevKeyStates[key] = currentDown;
    }
}

void InputComponent::ProcessCallbacks()
{
    for (const auto& callbackInfo : m_callbacks) {
        
        if (GetKeyState(callbackInfo.key) == callbackInfo.triggerState) {
            callbackInfo.callback(callbackInfo.key);
        }
    }

    // 상태 변화 콜백 호출
    if (m_onStateChanged) {
        
        for (const auto& [key, state] : m_keyStates) {
            if (state != KeyState::None) {
                m_onStateChanged(key, state);
            }
        }
    }
}

std::optional<InputKey> InputComponent::ParseInputKey(std::string_view keyName) const
{
    return magic_enum::enum_cast<InputKey>(keyName);
}

std::optional<KeyState> InputComponent::ParseKeyState(std::string_view stateName) const
{
    return magic_enum::enum_cast<KeyState>(stateName);
}

int InputComponent::GetVirtualKeyCode(InputKey key) const
{
    // 실제 가상 키 코드 매핑
    switch (key) {
    case InputKey::Left: return VK_LEFT;
    case InputKey::Right: return VK_RIGHT;
    case InputKey::Jump: return VK_SPACE;
    case InputKey::Action: return 'Z';
    case InputKey::Menu: return VK_ESCAPE;
    case InputKey::Pause: return 'P';
    default: return 0;
    }
}

bool InputComponent::IsVirtualKeyDown(InputKey key) const
{
    return GetAsyncKeyState(GetVirtualKeyCode(key)) & 0x8000;
}
