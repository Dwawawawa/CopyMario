// SceneIntro.cpp 
#include "00pch.h"
#include "SceneIntro.h"
#include "30GameObject.h"
#include "32Transform.h"
#include "32Movement.h"
#include "32Renderer.h"
#include "32PhysicsComponent.h"
#include "32ColliderComponent.h"

#include "32InputComponent.h"

void SceneIntro::Initialize()
{
    Scene::Initialize();

    // ��Ʈ���ϴ� ���ӿ�����Ʈ ���� ����
    //GameObject* patrolObject = CreateGameObject();
    //patrolObject->GetTransform()->SetPosition(300, 500);
    //patrolObject->GetTransform()->SetScale(300, 300);

    //// Movement ������Ʈ �߰�
    //Movement* movement = patrolObject->AddComponent<Movement>();
    //movement->SetSpeed(400.0f);
    ////movement->SetBounds(200, -100, 1000, 1000);
    //movement->SetMovementType(Movement::MovementType::Linear);  // �Ǵ� �ٸ� Ÿ��
    //movement->SetVelocity(1, 0);

    ////Renderer ������Ʈ �߰�
    //Renderer* renderer = patrolObject->AddComponent<Renderer>();
    //renderer->SetSize(100, 100);
    //renderer->SetColor(255, 255, 0); // ������

    ////InputComponent ������Ʈ �߰�
    //InputComponent* inputComp = patrolObject->AddComponent<InputComponent>();
    //inputComp->SetupWithMovement(400.0f);

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
    collider->SetCollisionMask({ CollisionLayer::Ground, CollisionLayer::Wall, CollisionLayer::Platform });

    //Input ������Ʈ���� ���� �߰�
    InputComponent* input = mario->AddComponent<InputComponent>();
    input->SetOnJump([physics]() {
        physics->Jump();  // ���� �õ�
        });
    
    input->SetupWithMovement(400.0f);

    //Renderer ������Ʈ �߰�
    Renderer* renderer_mario = mario->AddComponent<Renderer>();
    renderer_mario->SetSize(32, 48);
    renderer_mario->SetColor(255, 0, 0); // ������

    

    // �� ������Ʈ ����
    GameObject* ground = CreateGameObject();
    ground->GetTransform()->SetPosition(50, 500);
    ground->GetTransform()->SetScale(800, 50);

    ColliderComponent* groundCollider = ground->AddComponent<ColliderComponent>();
    groundCollider->SetSize(800, 50);
    groundCollider->SetLayer(CollisionLayer::Ground);
    groundCollider->SetCollisionMask({});  // ���� �ٸ� �Ͱ� �浹 üũ ����

    //Renderer ������Ʈ �߰�
    Renderer* renderer_ground = ground->AddComponent<Renderer>();
    renderer_ground->SetSize(800, 50);
    renderer_ground->SetColor(D2D1::ColorF::Brown);

    //////////////////////////////////////
    // �̷��� �ؼ� ���������� �����ϸ� �� ��
    debugPhysics = mario->GetComponent<PhysicsComponent>();
    debugInput = mario->GetComponent<InputComponent>();
    debugCollider = mario->GetComponent<ColliderComponent>();

}

void SceneIntro::Update(float dTime)
{
    static float tempTime = 0;
    tempTime += dTime;

    

    if(tempTime > 3.0f)
    {
        std::cout << debugInput->IsLeftPressed() << std::endl;
        //printf("Physics Component: %s\n", debugPhysics ? "OK" : "MISSING");
        //printf("Input Component: %s\n", debugInput ? "OK" : "MISSING");
        //printf("Collider Component: %s\n", debugCollider ? "OK" : "MISSING");
        tempTime = 0;
    }
    
    Scene::Update(dTime);
}

//void SceneIntro::Render(std::shared_ptr<SSEngine> Renderer)
//{
//	m_pObjectManager->Render(Renderer);
//}



