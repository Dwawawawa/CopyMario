// 32ColliderComponent.cpp
#include "00pch.h"
#include "32ColliderComponent.h"
#include "32PhysicsComponent.h"
#include <algorithm>

std::vector<ColliderComponent*> ColliderComponent::s_allColliders;

ColliderComponent::ColliderComponent(ColliderType type) : m_type(type)
{
    // 기본 충돌 마스크 설정
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
    // 1. 전역 콜라이더 리스트에서 자신을 제거
    UnregisterCollider(this);

    // 2. 현재 충돌 중인 다른 콜라이더들에게 충돌 종료 알림
    for (auto* other : m_currentCollisions)
    {
        if (other && other->m_onCollisionExit)
        {
            CollisionInfo info;
            info.other = this;
            other->m_onCollisionExit(info);
        }

        // 상대방의 충돌 리스트에서도 자신을 제거
        auto it = std::find(other->m_currentCollisions.begin(),
            other->m_currentCollisions.end(), this);
        if (it != other->m_currentCollisions.end())
        {
            other->m_currentCollisions.erase(it);
        }
    }

    // 3. 현재 충돌 리스트 클리어
    m_currentCollisions.clear();

    // 4. 콜백 함수들 클리어 (lambda나 function 객체의 메모리 해제)
    m_onCollisionEnter = nullptr;
    m_onCollisionStay = nullptr;
    m_onCollisionExit = nullptr;

    // 5. 충돌 마스크 클리어
    m_collisionMask.clear();

    // 6. Transform 포인터 null로 설정 (소유권이 없으므로 delete 안함)
    m_transform = nullptr;

    // 7. 부모 클래스의 Release 호출
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

    // 겹친 영역 계산
    float overlapX = std::min(myMax.x - otherMin.x, otherMax.x - myMin.x);
    float overlapY = std::min(myMax.y - otherMin.y, otherMax.y - myMin.y);

    // 더 작은 겹침이 충돌 방향
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

        // 위에서 충돌했는지 체크 (플랫폼용)
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

            // 땅 체크 (아래쪽에서 충돌하고 Ground나 Platform 레이어인 경우)
            if ((other->GetLayer() == CollisionLayer::Ground ||
                other->GetLayer() == CollisionLayer::Platform) &&
                info.fromTop)
            {
                m_isGrounded = true;

                // Physics 컴포넌트가 있으면 땅 상태 알림
                auto physics = GetOwner()->GetComponent<PhysicsComponent>();
                if (physics)
                {
                    physics->SetGrounded(true);
                    // 아래로 떨어지고 있었다면 속도 제거
                    if (physics->GetVelocity().y > 0)
                    {
                        physics->SetVelocity(Vector2(0,0));
                    }
                }
            }

            // 새로운 충돌인지 체크
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

    // 충돌 종료 체크
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

    // 땅에서 떨어진 경우 Physics에 알림
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
    // 아직 뭐 수정하지 않음
    s_allColliders.erase(std::remove(s_allColliders.begin(), s_allColliders.end(), collider), s_allColliders.end());
}

void ColliderComponent::UpdateAllCollisions()
{
    // 전역 충돌 업데이트 (필요시 사용)
    for (auto* collider : s_allColliders)
    {
        if (collider->IsActive())
        {
            collider->CheckCollisions();
        }
    }
}

/*
사용 예제: 마리오 캐릭터 설정

GameObject* mario = CreateGameObject();
mario->GetTransform()->SetPosition(100, 300);
mario->GetTransform()->SetScale(32, 48);  // 마리오 크기

// Physics 컴포넌트 추가
PhysicsComponent* physics = mario->AddComponent<PhysicsComponent>();
physics->SetGravity(800.0f);
physics->SetJumpPower(350.0f);
physics->SetMaxFallSpeed(500.0f);

// Collider 컴포넌트 추가
ColliderComponent* collider = mario->AddComponent<ColliderComponent>();
collider->SetSize(28, 44);  // 마리오보다 살짝 작게
collider->SetLayer(CollisionLayer::Player);
collider->SetCollisionMask({CollisionLayer::Ground, CollisionLayer::Wall, CollisionLayer::Platform});

// Input 컴포넌트에서 점프 추가
InputComponent* input = mario->AddComponent<InputComponent>();
input->SetOnJump([physics]() {
    physics->Jump();  // 점프 시도
});

// 땅 오브젝트 생성
GameObject* ground = CreateGameObject();
ground->GetTransform()->SetPosition(400, 500);
ground->GetTransform()->SetScale(800, 50);

ColliderComponent* groundCollider = ground->AddComponent<ColliderComponent>();
groundCollider->SetSize(800, 50);
groundCollider->SetLayer(CollisionLayer::Ground);
groundCollider->SetCollisionMask({});  // 땅은 다른 것과 충돌 체크 안함
*/