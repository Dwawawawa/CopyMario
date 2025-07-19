// 32Transform.h
#pragma once
#include "31Component.h"

class Transform : public Component
{
public:    
    // math 유틸리티로 빼야 할 듯
    struct Vector2 {
        float x, y;
        Vector2(float x = 0, float y = 0) : x(x), y(y) {}

        // 벡터 연산 추가
        Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
        Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
        Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
        Vector2& operator+=(const Vector2& other) { x += other.x; y += other.y; return *this; }

        // 유틸리티 함수들
        float Length() const { return std::sqrt(x * x + y * y); }
        Vector2 Normalized() const {
            float len = Length();
            return len > 0 ? Vector2(x / len, y / len) : Vector2(0, 0);
        }
    };

    Transform() : m_position(0.0f, 0.0f), m_scale(1.0f, 1.0f), m_rotation(0.0f) {}

    // Position
    void SetPosition(float x, float y) { m_position.x = x; m_position.y = y; }
    void SetPosition(const Vector2& pos) { m_position = pos; }
    Vector2 GetPosition() const { return m_position; }
    void Translate(float x, float y) { m_position.x += x; m_position.y += y; }
    void Translate(const Vector2& offset) { m_position += offset; }

    // Scale
    void SetScale(float x, float y) { m_scale.x = x; m_scale.y = y; }
    void SetScale(const Vector2& scale) { m_scale = scale; }
    Vector2 GetScale() const { return m_scale; }

    // Rotation
    void SetRotation(float rotation) { m_rotation = rotation; }
    float GetRotation() const { return m_rotation; }
    void Rotate(float deltaRotation) { m_rotation += deltaRotation; }

private:
    Vector2 m_position;
    Vector2 m_scale;
    float m_rotation;
};