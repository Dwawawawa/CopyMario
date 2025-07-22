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
    // 마리오 오브젝트 추가
    GameObject* mario = CreateGameObject();
    mario->GetTransform()->SetPosition(100, 300);
    mario->GetTransform()->SetScale(32, 48);  // 마리오 크기

    /////////////////////////////
    // Physics 컴포넌트 추가
    PhysicsComponent* physics = mario->AddComponent<PhysicsComponent>();
    physics->SetGravity(800.0f);
    physics->SetJumpPower(350.0f);
    physics->SetMaxFallSpeed(500.0f);

    /////////////////////////////
    // Collider 컴포넌트 추가
    ColliderComponent* collider = mario->AddComponent<ColliderComponent>();
    collider->SetSize(1, 1);  // 마리오보다 살짝 작게
    collider->SetLayer(CollisionLayer::Player);
    collider->SetCollisionMask({ CollisionLayer::Ground, CollisionLayer::Wall, CollisionLayer::Platform });

    /////////////////////////////
    //Input 컴포넌트에서 점프 추가
    InputComponent* input = mario->AddComponent<InputComponent>();

    //1. 점프 콜백 등록
    input->RegisterKeyCallback(InputKey::Jump, KeyState::Pressed, [physics](InputKey key) {
        // PhysicsComponent의 Jump() 함수를 호출합니다.
        if (physics) {
            physics->Jump();
        }
        });

    // 2. 좌우 이동 콜백을 PhysicsComponent에 맞게 새로 등록
    float moveSpeed = 200.0f; // 이동 속도 설정
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


    //3. 키를 뗐을 때 멈추도록 콜백 등록
    input->RegisterKeyCallback(InputKey::Left, KeyState::Released, [physics, input](InputKey key) {
        // 오른쪽 키가 눌려있지 않을 때만 속도를 0으로 만듭니다.
        if (physics && !input->IsKeyHold(InputKey::Right)) {
            auto currentVel = physics->GetVelocity();
            physics->SetVelocity(0, currentVel.y);
        }
        });

    input->RegisterKeyCallback(InputKey::Right, KeyState::Released, [physics, input](InputKey key) {
        // 왼쪽 키가 눌려있지 않을 때만 속도를 0으로 만듭니다.
        if (physics && !input->IsKeyHold(InputKey::Left)) {
            auto currentVel = physics->GetVelocity();
            physics->SetVelocity(0, currentVel.y);
        }
        });

    /////////////////////////////
    //Renderer 컴포넌트 추가
    Renderer* renderer_mario = mario->AddComponent<Renderer>();
    renderer_mario->SetSize(32, 48);
    renderer_mario->SetColor(D2D1::ColorF::MediumOrchid);

    ///////////////////////////
    ///////////////////////////
    ///////////////////////////
    ///////////////////////////
    //땅 오브젝트 생성

    GameObject* ground = CreateGameObject();
    ground->GetTransform()->SetPosition(50, 400);
    ground->GetTransform()->SetScale(800, 50);

    ColliderComponent* groundCollider = ground->AddComponent<ColliderComponent>();
    groundCollider->SetSize(1, 1);
    groundCollider->SetLayer(CollisionLayer::Ground);
    groundCollider->SetCollisionMask({});  // 땅은 다른 것과 충돌 체크 안함

    //Renderer 컴포넌트 추가
    Renderer* renderer_ground = ground->AddComponent<Renderer>();
    renderer_ground->SetSize(800, 50);
    renderer_ground->SetColor(D2D1::ColorF::Chocolate);
}

void SceneBlack::Update(float dTime)
{
    Scene::Update(dTime);
}



