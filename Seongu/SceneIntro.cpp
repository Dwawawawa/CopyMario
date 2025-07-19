// SceneIntro.cpp 
#include "00pch.h"
#include "SceneIntro.h"
#include "30GameObject.h"
#include "32Transform.h"
#include "32Movement.h"
#include "32Renderer.h"

void SceneIntro::Initialize()
{
    Scene::Initialize();

    // 패트롤하는 게임오브젝트 생성 예시
    GameObject* patrolObject = CreateGameObject();
    patrolObject->GetTransform()->SetPosition(300, 300);

    // Movement 컴포넌트 추가
    Movement* movement = patrolObject->AddComponent<Movement>();
    movement->SetSpeed(400.0f);
    movement->SetBounds(0, 800);
    movement->SetMovementType(Movement::MovementType::Bounce);  // 또는 다른 타입
    movement->SetVelocity(1, 0);

    //Renderer 컴포넌트 추가
    Renderer* renderer = patrolObject->AddComponent<Renderer>();
    renderer->SetSize(100, 100);
    renderer->SetColor(255, 0, 0); // 빨간색
	
}

void SceneIntro::Update(float dTime)
{
    Scene::Update(dTime);
}

//void SceneIntro::Render(std::shared_ptr<SSEngine> Renderer)
//{
//	m_pObjectManager->Render(Renderer);
//}



