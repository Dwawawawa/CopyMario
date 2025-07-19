#include "00pch.h"
#include "32Movement.h"
#include "30GameObject.h"
#include "32Transform.h"
#include <algorithm>

void Movement::Initialize()
{
    m_transform = GetOwner()->GetComponent<Transform>();
    if (m_transform && m_movementType == MovementType::BackAndForth) {
        m_startPosition = m_transform->GetPosition();
    }
}

void Movement::Update(float deltaTime)
{
    if (!m_transform || !IsActive()) return;

    m_time += deltaTime;

    switch (m_movementType) {
    case MovementType::Linear:
        UpdateLinearMovement(deltaTime);
        break;
    case MovementType::Bounce:
        UpdateBounceMovement(deltaTime);
        break;
    case MovementType::Circular:
        UpdateCircularMovement(deltaTime);
        break;
    case MovementType::BackAndForth:
        UpdateBackAndForthMovement(deltaTime);
        break;
    }

}

void Movement::UpdateLinearMovement(float deltaTime)
{
    Transform::Vector2 movement = m_velocity * (m_speed * deltaTime);
    m_transform->Translate(movement);
}

void Movement::UpdateBounceMovement(float deltaTime)
{
    auto pos = m_transform->GetPosition();

    // 경계 체크 및 속도 반전
    if (pos.x <= m_minBounds.x || pos.x >= m_maxBounds.x) {
        m_velocity.x = -m_velocity.x;
    }
    if (pos.y <= m_minBounds.y || pos.y >= m_maxBounds.y) {
        m_velocity.y = -m_velocity.y;
    }

#undef max
#undef min

    // 경계 내부로 위치 보정
    pos.x = std::max(m_minBounds.x, std::min(m_maxBounds.x, pos.x));
    pos.y = std::max(m_minBounds.y, std::min(m_maxBounds.y, pos.y));
    m_transform->SetPosition(pos);

    // 이동
    Transform::Vector2 movement = m_velocity * (m_speed * deltaTime);
    m_transform->Translate(movement);
}

void Movement::UpdateCircularMovement(float deltaTime)
{
    float angle = m_speed * m_time * 0.01f; // 속도를 각속도로 변환
    float x = m_circularCenter.x + cos(angle) * m_circularRadius;
    float y = m_circularCenter.y + sin(angle) * m_circularRadius;
    m_transform->SetPosition(x, y);
}

void Movement::UpdateBackAndForthMovement(float deltaTime)
{
    auto currentPos = m_transform->GetPosition();
    Transform::Vector2 direction;

    if (m_backAndForthHorizontal) {
        direction = Transform::Vector2(m_backAndForthForward ? 1 : -1, 0);
        float distanceFromStart = abs(currentPos.x - m_startPosition.x);
        if (distanceFromStart >= m_backAndForthDistance) {
            m_backAndForthForward = !m_backAndForthForward;
        }
    }
    else {
        direction = Transform::Vector2(0, m_backAndForthForward ? 1 : -1);
        float distanceFromStart = abs(currentPos.y - m_startPosition.y);
        if (distanceFromStart >= m_backAndForthDistance) {
            m_backAndForthForward = !m_backAndForthForward;
        }
    }

    Transform::Vector2 movement = direction * (m_speed * deltaTime);
    m_transform->Translate(movement);
}