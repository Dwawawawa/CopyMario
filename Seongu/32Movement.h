//32Movement.h
#pragma once
#include "31Component.h"
#include "32Transform.h"



class Movement : public Component
{
public:
    enum class MovementType {
        Linear,          // 직선 이동
        Bounce,          // 경계에서 튕김
        Circular,        // 원형 이동
        BackAndForth,    // 앞뒤 이동
        Custom           // 사용자 정의
    };

    Movement() : m_speed(100.0f), m_velocity(0, 0), m_movementType(MovementType::Linear),
        m_minBounds(-400, -300), m_maxBounds(400, 300), m_time(0.0f) {
    }

    void Initialize() override;
    void Update(float deltaTime) override;


    // 기본 설정
    void SetSpeed(float speed) { m_speed = speed; }
    void SetVelocity(float x, float y) { m_velocity = Transform::Vector2(x, y); }
    void SetVelocity(const Transform::Vector2& vel) { m_velocity = vel; }
    void SetMovementType(MovementType type) { m_movementType = type; }

    // 경계 설정
    void SetBounds(float minX, float maxX, float minY, float maxY) {
        m_minBounds = Transform::Vector2(minX, minY);
        m_maxBounds = Transform::Vector2(maxX, maxY);
    }
    void SetBounds(const Transform::Vector2& min, const Transform::Vector2& max) {
        m_minBounds = min; m_maxBounds = max;
    }

    // 원형 이동 설정
    void SetCircularMovement(float radius, float centerX = 0, float centerY = 0) {
        m_circularRadius = radius;
        m_circularCenter = Transform::Vector2(centerX, centerY);
        m_movementType = MovementType::Circular;
    }

    // 앞뒤 이동 설정 (특정 축에서만 이동)
    void SetBackAndForthMovement(bool horizontal = true, float distance = 200.0f) {
        m_backAndForthHorizontal = horizontal;
        m_backAndForthDistance = distance;
        m_movementType = MovementType::BackAndForth;
        if (!m_transform) return; // Initialize 전이면 나중에 설정
        m_startPosition = m_transform->GetPosition();
    }

    // Getter 함수들
    float GetSpeed() const { return m_speed; }
    Transform::Vector2 GetVelocity() const { return m_velocity; }
    MovementType GetMovementType() const { return m_movementType; }

private:
    void UpdateLinearMovement(float deltaTime);
    void UpdateBounceMovement(float deltaTime);
    void UpdateCircularMovement(float deltaTime);
    void UpdateBackAndForthMovement(float deltaTime);

    
    Transform* m_transform = nullptr;

    float m_speed;
    Transform::Vector2 m_velocity;
    MovementType m_movementType;

    // 경계
    Transform::Vector2 m_minBounds, m_maxBounds;

    // 원형 이동용
    float m_circularRadius = 100.0f;
    Transform::Vector2 m_circularCenter;

    // 앞뒤 이동용
    bool m_backAndForthHorizontal = true;
    float m_backAndForthDistance = 200.0f;
    Transform::Vector2 m_startPosition;
    bool m_backAndForthForward = true;

    // 시간 누적
    float m_time;
};
