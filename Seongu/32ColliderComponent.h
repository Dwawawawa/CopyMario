// 32ColliderComponent.h
#pragma once
#include "31Component.h"
#include "32Transform.h"
#include <vector>
#include <functional>

enum class ColliderType
{
    Box,        // 사각형 콜라이더
    Circle,     // 원형 콜라이더 (나중에 확장용)
};

enum class CollisionLayer
{
    Player = 0,
    Ground = 1,
    Enemy = 2,
    Item = 3,
    Wall = 4,
    Platform = 5  // 점프해서 올라갈 수 있는 플랫폼
};

struct CollisionInfo
{
    class ColliderComponent* other = nullptr;
    Transform::Vector2 normal = Transform::Vector2(0, 0);  // 충돌 방향
    float penetration = 0.0f;  // 겹친 깊이
    bool fromTop = false;      // 위에서 충돌했는지 (플랫폼용)
};

class ColliderComponent : public Component
{
public:
    using CollisionCallback = std::function<void(const CollisionInfo&)>;

    ColliderComponent(ColliderType type = ColliderType::Box);
    ~ColliderComponent() override = default;

    void Initialize() override;
    void Update(float deltaTime) override;
    void Release() override;


    // 콜라이더 설정
    void SetSize(float width, float height) { m_width = width; m_height = height; }
    void SetOffset(float x, float y) { m_offset = Transform::Vector2(x, y); }
    void SetLayer(CollisionLayer layer) { m_layer = layer; }
    void SetIsTrigger(bool trigger) { m_isTrigger = trigger; }

    // 충돌 감지용 마스크 설정 (어떤 레이어와 충돌할지)
    void SetCollisionMask(std::vector<CollisionLayer> layers) { m_collisionMask = layers; }
    void AddCollisionLayer(CollisionLayer layer) { m_collisionMask.push_back(layer); }

    // 콜백 설정
    void SetOnCollisionEnter(CollisionCallback callback) { m_onCollisionEnter = callback; }
    void SetOnCollisionStay(CollisionCallback callback) { m_onCollisionStay = callback; }
    void SetOnCollisionExit(CollisionCallback callback) { m_onCollisionExit = callback; }

    // 충돌 체크
    bool IsColliding(ColliderComponent* other) const;
    CollisionInfo GetCollisionInfo(ColliderComponent* other) const;

    // 바운딩 박스 정보
    Transform::Vector2 GetMin() const;
    Transform::Vector2 GetMax() const;
    Transform::Vector2 GetCenter() const;

    // Getter 함수들
    ColliderType GetType() const { return m_type; }
    CollisionLayer GetLayer() const { return m_layer; }
    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }
    bool IsTrigger() const { return m_isTrigger; }
    bool IsGrounded() const { return m_isGrounded; }

    // 정적 함수 - 모든 콜라이더 관리
    static void RegisterCollider(ColliderComponent* collider);
    static void UnregisterCollider(ColliderComponent* collider);
    static void UpdateAllCollisions();

private:
    void CheckCollisions();
    bool ShouldCollideWith(CollisionLayer otherLayer) const;

    Transform* m_transform = nullptr;

    // 콜라이더 정보
    ColliderType m_type;
    float m_width = 32.0f;
    float m_height = 32.0f;
    Transform::Vector2 m_offset = Transform::Vector2(0, 0);
    CollisionLayer m_layer = CollisionLayer::Player;
    bool m_isTrigger = false;  // true면 물리적 충돌 없이 이벤트만

    // 충돌 마스크 (어떤 레이어와 충돌할지)
    std::vector<CollisionLayer> m_collisionMask;

    // 충돌 콜백들
    CollisionCallback m_onCollisionEnter;
    CollisionCallback m_onCollisionStay;
    CollisionCallback m_onCollisionExit;

    // 충돌 상태
    std::vector<ColliderComponent*> m_currentCollisions;
    bool m_isGrounded = false;

    // 전역 콜라이더 리스트
    static std::vector<ColliderComponent*> s_allColliders;
};