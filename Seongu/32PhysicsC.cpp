// 32PhysicsComponent.cpp
#include "00pch.h"
#include "32PhysicsComponent.h"
#include "31Component.h"
#include "32Transform.h"
#include "30GameObject.h"



class ColliderComponent;

PhysicsComponent::PhysicsComponent()
    : m_velocity(0, 0)
{

}

void PhysicsComponent::Initialize()
{
    Component::Initialize();
    m_transform = GetOwner()->GetComponent<Transform>();
}

void PhysicsComponent::Update(float deltaTime)
{
    if (!IsActive() || !m_transform) return;
    Component::Update(deltaTime);

    ApplyGravity(deltaTime);
    ApplyDrag(deltaTime);
    ClampVelocity();

    // 위치 업데이트
    auto currentPos = m_transform->GetPosition();
    currentPos.x += m_velocity.x * deltaTime;
    currentPos.y += m_velocity.y * deltaTime;
    m_transform->SetPosition(currentPos);
}

void PhysicsComponent::Release()
{
    m_transform = nullptr;

    Component::Release();

}

bool PhysicsComponent::Jump()
{
    if (!m_isGrounded) return false;

    m_velocity.y = -m_jumpPower;  // Y축 위쪽이 -라고 가정
    m_isGrounded = false;
    return true;
}

void PhysicsComponent::ApplyGravity(float deltaTime)
{
    if (!m_isGrounded)
    {
        m_velocity.y += m_gravity * deltaTime;
    }
}

void PhysicsComponent::ApplyDrag(float deltaTime)
{
    float drag = m_isGrounded ? m_groundDrag : m_airDrag;

    // Windows.h에서 정의된 매크로와 충돌 방지

#undef max
#undef min


    // X축 드래그 (마찰력)
    if (m_velocity.x > 0)
    {
        m_velocity.x = std::max(0.0f, m_velocity.x - drag * deltaTime * 100.0f);
    }
    else if (m_velocity.x < 0)
    {
        m_velocity.x = std::min(0.0f, m_velocity.x + drag * deltaTime * 100.0f);
    }
}

void PhysicsComponent::ClampVelocity()
{
    // 최대 낙하 속도 제한
    if (m_velocity.y > m_maxFallSpeed)
    {
        m_velocity.y = m_maxFallSpeed;
    }
}