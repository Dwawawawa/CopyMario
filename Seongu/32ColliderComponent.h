// 32ColliderComponent.h
#pragma once
#include "31Component.h"
#include "32Transform.h"
#include <vector>
#include <functional>

enum class ColliderType
{
    Box,        // �簢�� �ݶ��̴�
    Circle,     // ���� �ݶ��̴� (���߿� Ȯ���)
};

enum class CollisionLayer
{
    Player = 0,
    Ground = 1,
    Enemy = 2,
    Item = 3,
    Wall = 4,
    Platform = 5  // �����ؼ� �ö� �� �ִ� �÷���
};

struct CollisionInfo
{
    class ColliderComponent* other = nullptr;
    Transform::Vector2 normal = Transform::Vector2(0, 0);  // �浹 ����
    float penetration = 0.0f;  // ��ģ ����
    bool fromTop = false;      // ������ �浹�ߴ��� (�÷�����)
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


    // �ݶ��̴� ����
    void SetSize(float width, float height) { m_width = width; m_height = height; }
    void SetOffset(float x, float y) { m_offset = Transform::Vector2(x, y); }
    void SetLayer(CollisionLayer layer) { m_layer = layer; }
    void SetIsTrigger(bool trigger) { m_isTrigger = trigger; }

    // �浹 ������ ����ũ ���� (� ���̾�� �浹����)
    void SetCollisionMask(std::vector<CollisionLayer> layers) { m_collisionMask = layers; }
    void AddCollisionLayer(CollisionLayer layer) { m_collisionMask.push_back(layer); }

    // �ݹ� ����
    void SetOnCollisionEnter(CollisionCallback callback) { m_onCollisionEnter = callback; }
    void SetOnCollisionStay(CollisionCallback callback) { m_onCollisionStay = callback; }
    void SetOnCollisionExit(CollisionCallback callback) { m_onCollisionExit = callback; }

    // �浹 üũ
    bool IsColliding(ColliderComponent* other) const;
    CollisionInfo GetCollisionInfo(ColliderComponent* other) const;

    // �ٿ�� �ڽ� ����
    Transform::Vector2 GetMin() const;
    Transform::Vector2 GetMax() const;
    Transform::Vector2 GetCenter() const;

    // Getter �Լ���
    ColliderType GetType() const { return m_type; }
    CollisionLayer GetLayer() const { return m_layer; }
    float GetWidth() const { return m_width; }
    float GetHeight() const { return m_height; }
    bool IsTrigger() const { return m_isTrigger; }
    bool IsGrounded() const { return m_isGrounded; }

    // ���� �Լ� - ��� �ݶ��̴� ����
    static void RegisterCollider(ColliderComponent* collider);
    static void UnregisterCollider(ColliderComponent* collider);
    static void UpdateAllCollisions();

private:
    void CheckCollisions();
    bool ShouldCollideWith(CollisionLayer otherLayer) const;

    Transform* m_transform = nullptr;

    // �ݶ��̴� ����
    ColliderType m_type;
    float m_width = 32.0f;
    float m_height = 32.0f;
    Transform::Vector2 m_offset = Transform::Vector2(0, 0);
    CollisionLayer m_layer = CollisionLayer::Player;
    bool m_isTrigger = false;  // true�� ������ �浹 ���� �̺�Ʈ��

    // �浹 ����ũ (� ���̾�� �浹����)
    std::vector<CollisionLayer> m_collisionMask;

    // �浹 �ݹ��
    CollisionCallback m_onCollisionEnter;
    CollisionCallback m_onCollisionStay;
    CollisionCallback m_onCollisionExit;

    // �浹 ����
    std::vector<ColliderComponent*> m_currentCollisions;
    bool m_isGrounded = false;

    // ���� �ݶ��̴� ����Ʈ
    static std::vector<ColliderComponent*> s_allColliders;
};