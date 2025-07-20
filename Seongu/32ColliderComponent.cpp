// 32ColliderComponent.cpp
#include "00pch.h"
#include "32ColliderComponent.h"
#include "32PhysicsComponent.h"
#include <algorithm>

std::vector<ColliderComponent*> ColliderComponent::s_allColliders;

ColliderComponent::ColliderComponent(ColliderType type) : m_type(type)
{
    // �⺻ �浹 ����ũ ����
    m_collisionMask = { CollisionLayer::Ground, CollisionLayer::Wall, CollisionLayer::Platform };
}

void ColliderComponent::Initialize()
{
    Component::Initialize();
    m_transform = GetOwner()->GetComponent<Transform>();
    RegisterCollider(this);
}

void ColliderComponent::Update(float deltaTime)
{
    if (!IsActive()) return;
    Component::Update(deltaTime);

    CheckCollisions();
}

void ColliderComponent::Release()
{
    // 1. ���� �ݶ��̴� ����Ʈ���� �ڽ��� ����
    UnregisterCollider(this);

    // 2. ���� �浹 ���� �ٸ� �ݶ��̴��鿡�� �浹 ���� �˸�
    for (auto* other : m_currentCollisions)
    {
        if (other && other->m_onCollisionExit)
        {
            CollisionInfo info;
            info.other = this;
            other->m_onCollisionExit(info);
        }

        // ������ �浹 ����Ʈ������ �ڽ��� ����
        auto it = std::find(other->m_currentCollisions.begin(),
            other->m_currentCollisions.end(), this);
        if (it != other->m_currentCollisions.end())
        {
            other->m_currentCollisions.erase(it);
        }
    }

    // 3. ���� �浹 ����Ʈ Ŭ����
    m_currentCollisions.clear();

    // 4. �ݹ� �Լ��� Ŭ���� (lambda�� function ��ü�� �޸� ����)
    m_onCollisionEnter = nullptr;
    m_onCollisionStay = nullptr;
    m_onCollisionExit = nullptr;

    // 5. �浹 ����ũ Ŭ����
    m_collisionMask.clear();

    // 6. Transform ������ null�� ���� (�������� �����Ƿ� delete ����)
    m_transform = nullptr;

    // 7. �θ� Ŭ������ Release ȣ��
    Component::Release();
}

bool ColliderComponent::IsColliding(ColliderComponent* other) const
{
    if (!other || other == this) return false;

    auto myMin = GetMin();
    auto myMax = GetMax();
    auto otherMin = other->GetMin();
    auto otherMax = other->GetMax();

    return !(myMax.x < otherMin.x || myMin.x > otherMax.x ||
        myMax.y < otherMin.y || myMin.y > otherMax.y);
}

CollisionInfo ColliderComponent::GetCollisionInfo(ColliderComponent* other) const
{
    CollisionInfo info;
    info.other = other;

    if (!IsColliding(other)) return info;

    auto myCenter = GetCenter();
    auto otherCenter = other->GetCenter();
    auto myMin = GetMin();
    auto myMax = GetMax();
    auto otherMin = other->GetMin();
    auto otherMax = other->GetMax();

#undef max
#undef min

    // ��ģ ���� ���
    float overlapX = std::min(myMax.x - otherMin.x, otherMax.x - myMin.x);
    float overlapY = std::min(myMax.y - otherMin.y, otherMax.y - myMin.y);

    // �� ���� ��ħ�� �浹 ����
    if (overlapX < overlapY)
    {
        info.normal.x = (myCenter.x < otherCenter.x) ? -1.0f : 1.0f;
        info.normal.y = 0.0f;
        info.penetration = overlapX;
    }
    else
    {
        info.normal.x = 0.0f;
        info.normal.y = (myCenter.y < otherCenter.y) ? -1.0f : 1.0f;
        info.penetration = overlapY;

        // ������ �浹�ߴ��� üũ (�÷�����)
        info.fromTop = (myCenter.y < otherCenter.y) && (info.normal.y < 0);
    }

    return info;
}

Transform::Vector2 ColliderComponent::GetMin() const
{
    if (!m_transform) return Transform::Vector2(0, 0);
    auto pos = m_transform->GetPosition();
    auto scale = m_transform->GetScale();

    return Transform::Vector2(
        pos.x + m_offset.x - (m_width * scale.x * 0.5f),
        pos.y + m_offset.y - (m_height * scale.y * 0.5f)
    );
}

Transform::Vector2 ColliderComponent::GetMax() const
{
    if (!m_transform) return Transform::Vector2(0, 0);
    auto pos = m_transform->GetPosition();
    auto scale = m_transform->GetScale();

    return Transform::Vector2(
        pos.x + m_offset.x + (m_width * scale.x * 0.5f),
        pos.y + m_offset.y + (m_height * scale.y * 0.5f)
    );
}

Transform::Vector2 ColliderComponent::GetCenter() const
{
    if (!m_transform) return Transform::Vector2(0, 0);
    auto pos = m_transform->GetPosition();
    return Transform::Vector2(pos.x + m_offset.x, pos.y + m_offset.y);
}

void ColliderComponent::CheckCollisions()
{
    std::vector<ColliderComponent*> newCollisions;
    bool wasGrounded = m_isGrounded;
    m_isGrounded = false;

    for (auto* other : s_allColliders)
    {
        if (other == this || !other->IsActive()) continue;
        if (!ShouldCollideWith(other->GetLayer())) continue;

        if (IsColliding(other))
        {
            newCollisions.push_back(other);

            CollisionInfo info = GetCollisionInfo(other);

            // �� üũ (�Ʒ��ʿ��� �浹�ϰ� Ground�� Platform ���̾��� ���)
            if ((other->GetLayer() == CollisionLayer::Ground ||
                other->GetLayer() == CollisionLayer::Platform) &&
                info.fromTop)
            {
                m_isGrounded = true;

                // Physics ������Ʈ�� ������ �� ���� �˸�
                auto physics = GetOwner()->GetComponent<PhysicsComponent>();
                if (physics)
                {
                    physics->SetGrounded(true);
                    // �Ʒ��� �������� �־��ٸ� �ӵ� ����
                    if (physics->GetVelocity().y > 0)
                    {
                        physics->SetVelocity(Vector2(0,0));
                    }
                }
            }

            // ���ο� �浹���� üũ
            bool isNewCollision = std::find(m_currentCollisions.begin(),
                m_currentCollisions.end(), other) == m_currentCollisions.end();

            if (isNewCollision && m_onCollisionEnter)
            {
                m_onCollisionEnter(info);
            }
            else if (!isNewCollision && m_onCollisionStay)
            {
                m_onCollisionStay(info);
            }
        }
    }

    // �浹 ���� üũ
    for (auto* oldCollision : m_currentCollisions)
    {
        bool stillColliding = std::find(newCollisions.begin(), newCollisions.end(), oldCollision) != newCollisions.end();
        if (!stillColliding && m_onCollisionExit)
        {
            CollisionInfo info;
            info.other = oldCollision;
            m_onCollisionExit(info);
        }
    }

    m_currentCollisions = newCollisions;

    // ������ ������ ��� Physics�� �˸�
    if (wasGrounded && !m_isGrounded)
    {
        auto physics = GetOwner()->GetComponent<PhysicsComponent>();
        if (physics)
        {
            physics->SetGrounded(false);
        }
    }
}

bool ColliderComponent::ShouldCollideWith(CollisionLayer otherLayer) const
{
    return std::find(m_collisionMask.begin(), m_collisionMask.end(), otherLayer) != m_collisionMask.end();
}

void ColliderComponent::RegisterCollider(ColliderComponent* collider)
{
    s_allColliders.push_back(collider);
}

void ColliderComponent::UnregisterCollider(ColliderComponent* collider)
{
    // ���� �� �������� ����
    s_allColliders.erase(std::remove(s_allColliders.begin(), s_allColliders.end(), collider), s_allColliders.end());
}

void ColliderComponent::UpdateAllCollisions()
{
    // ���� �浹 ������Ʈ (�ʿ�� ���)
    for (auto* collider : s_allColliders)
    {
        if (collider->IsActive())
        {
            collider->CheckCollisions();
        }
    }
}

/*
��� ����: ������ ĳ���� ����

GameObject* mario = CreateGameObject();
mario->GetTransform()->SetPosition(100, 300);
mario->GetTransform()->SetScale(32, 48);  // ������ ũ��

// Physics ������Ʈ �߰�
PhysicsComponent* physics = mario->AddComponent<PhysicsComponent>();
physics->SetGravity(800.0f);
physics->SetJumpPower(350.0f);
physics->SetMaxFallSpeed(500.0f);

// Collider ������Ʈ �߰�
ColliderComponent* collider = mario->AddComponent<ColliderComponent>();
collider->SetSize(28, 44);  // ���������� ��¦ �۰�
collider->SetLayer(CollisionLayer::Player);
collider->SetCollisionMask({CollisionLayer::Ground, CollisionLayer::Wall, CollisionLayer::Platform});

// Input ������Ʈ���� ���� �߰�
InputComponent* input = mario->AddComponent<InputComponent>();
input->SetOnJump([physics]() {
    physics->Jump();  // ���� �õ�
});

// �� ������Ʈ ����
GameObject* ground = CreateGameObject();
ground->GetTransform()->SetPosition(400, 500);
ground->GetTransform()->SetScale(800, 50);

ColliderComponent* groundCollider = ground->AddComponent<ColliderComponent>();
groundCollider->SetSize(800, 50);
groundCollider->SetLayer(CollisionLayer::Ground);
groundCollider->SetCollisionMask({});  // ���� �ٸ� �Ͱ� �浹 üũ ����
*/