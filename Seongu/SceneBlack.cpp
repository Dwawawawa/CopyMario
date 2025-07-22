// SceneBlack.cpp
#include "00pch.h"
#include "SceneBlack.h"
#include "30GameObject.h"
#include "32Transform.h"
#include "32Movement.h"
#include "32Renderer.h"
#include "32PhysicsComponent.h"
#include "32ColliderComponent.h"
#include "32InputComponent.h"

SceneBlack::SceneBlack()
{
}

SceneBlack::~SceneBlack()
{
    if (!m_gameObjects.empty())
    {
        m_gameObjects[0]->RemoveComponent<PhysicsComponent>();
        m_gameObjects[0]->RemoveComponent<ColliderComponent>();
        m_gameObjects[0]->RemoveComponent<InputComponent>();
        m_gameObjects[0]->RemoveComponent<Renderer>();

        m_gameObjects[1]->RemoveComponent<ColliderComponent>();
        m_gameObjects[1]->RemoveComponent<Renderer>();
    }
}

void SceneBlack::Initialize()
{
    Scene::Initialize();

    if (0) return;

    /////////////////////////////
    /////////////////////////////
    /////////////////////////////
    /////////////////////////////
    // ������ ������Ʈ �߰�
    GameObject* mario = CreateGameObject();
    mario->GetTransform()->SetPosition(100, 300);
    mario->GetTransform()->SetScale(32, 48);  // ������ ũ��

    /////////////////////////////
    // Physics ������Ʈ �߰�
    PhysicsComponent* physics = mario->AddComponent<PhysicsComponent>();
    physics->SetGravity(800.0f);
    physics->SetJumpPower(350.0f);
    physics->SetMaxFallSpeed(500.0f);

    /////////////////////////////
    // Collider ������Ʈ �߰�
    ColliderComponent* collider = mario->AddComponent<ColliderComponent>();
    collider->SetSize(1, 1);  // ���������� ��¦ �۰�
    collider->SetLayer(CollisionLayer::Player);
    collider->SetCollisionMask({ CollisionLayer::Ground, CollisionLayer::Wall, CollisionLayer::Platform });

    /////////////////////////////
    //Input ������Ʈ���� ���� �߰�
    InputComponent* input = mario->AddComponent<InputComponent>();

    //1. ���� �ݹ� ���
    input->RegisterKeyCallback(InputKey::Jump, KeyState::Pressed, [physics](InputKey key) {
        // PhysicsComponent�� Jump() �Լ��� ȣ���մϴ�.
        if (physics) {
            physics->Jump();
        }
        });

    // 2. �¿� �̵� �ݹ��� PhysicsComponent�� �°� ���� ���
    float moveSpeed = 200.0f; // �̵� �ӵ� ����
    input->RegisterKeyCallback(InputKey::Left, KeyState::Hold, [physics, moveSpeed](InputKey key) {
        if (physics) {
            auto currentVel = physics->GetVelocity();
            physics->SetVelocity(-moveSpeed, currentVel.y);
        }
        });

    input->RegisterKeyCallback(InputKey::Right, KeyState::Hold, [physics, moveSpeed](InputKey key) {
        if (physics) {
            auto currentVel = physics->GetVelocity();
            physics->SetVelocity(moveSpeed, currentVel.y);
        }
        });


    //3. Ű�� ���� �� ���ߵ��� �ݹ� ���
    input->RegisterKeyCallback(InputKey::Left, KeyState::Released, [physics, input](InputKey key) {
        // ������ Ű�� �������� ���� ���� �ӵ��� 0���� ����ϴ�.
        if (physics && !input->IsKeyHold(InputKey::Right)) {
            auto currentVel = physics->GetVelocity();
            physics->SetVelocity(0, currentVel.y);
        }
        });

    input->RegisterKeyCallback(InputKey::Right, KeyState::Released, [physics, input](InputKey key) {
        // ���� Ű�� �������� ���� ���� �ӵ��� 0���� ����ϴ�.
        if (physics && !input->IsKeyHold(InputKey::Left)) {
            auto currentVel = physics->GetVelocity();
            physics->SetVelocity(0, currentVel.y);
        }
        });

    /////////////////////////////
    //Renderer ������Ʈ �߰�
    Renderer* renderer_mario = mario->AddComponent<Renderer>();
    renderer_mario->SetSize(32, 48);
    renderer_mario->SetColor(D2D1::ColorF::MediumOrchid);

    ///////////////////////////
    ///////////////////////////
    ///////////////////////////
    ///////////////////////////
    //�� ������Ʈ ����

    GameObject* ground = CreateGameObject();
    ground->GetTransform()->SetPosition(50, 400);
    ground->GetTransform()->SetScale(800, 50);

    ColliderComponent* groundCollider = ground->AddComponent<ColliderComponent>();
    groundCollider->SetSize(1, 1);
    groundCollider->SetLayer(CollisionLayer::Ground);
    groundCollider->SetCollisionMask({});  // ���� �ٸ� �Ͱ� �浹 üũ ����

    //Renderer ������Ʈ �߰�
    Renderer* renderer_ground = ground->AddComponent<Renderer>();
    renderer_ground->SetSize(800, 50);
    renderer_ground->SetColor(D2D1::ColorF::Chocolate);
}

void SceneBlack::Update(float dTime)
{
    Scene::Update(dTime);
}



