// 32PhysicsComponent.h
#pragma once
#include "00pch.h"
#include "31Component.h"
//#include "30GameObject.h"

//class Component;
class GameObject;
class Transform;

struct Vector2
{
    float x, y;
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

class PhysicsComponent : public Component
{
public:
    PhysicsComponent();
    virtual ~PhysicsComponent() = default;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Release() override;

    // 점프 기능
    bool Jump();

    // 물리 속성 설정
    void SetGravity(float gravity) { m_gravity = gravity; }
    void SetJumpPower(float jumpPower) { m_jumpPower = jumpPower; }
    void SetMaxFallSpeed(float maxFallSpeed) { m_maxFallSpeed = maxFallSpeed; }
    void SetGroundDrag(float drag) { m_groundDrag = drag; }
    void SetAirDrag(float drag) { m_airDrag = drag; }

    // 상태 설정/조회
    void SetGrounded(bool grounded) { m_isGrounded = grounded; }
    bool IsGrounded() const { return m_isGrounded; }

    Vector2 GetVelocity() const { return m_velocity; }
    void SetVelocity(const Vector2& velocity) { m_velocity = velocity; }
    void SetVelocity(float x, float y) { m_velocity.x = x; m_velocity.y = y; }
    void AddVelocity(float x, float y) { m_velocity.x += x; m_velocity.y += y; }

private:
    Transform* m_transform = nullptr;

    Vector2 m_velocity;

    // 물리 속성
    float m_gravity = 800.0f;
    float m_jumpPower = 350.0f;
    float m_maxFallSpeed = 500.0f;
    float m_groundDrag = 0.8f;
    float m_airDrag = 0.2f;

    bool m_isGrounded = false;

    void ApplyGravity(float deltaTime);
    void ApplyDrag(float deltaTime);
    void ClampVelocity();
};


