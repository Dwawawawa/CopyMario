/////
// 씬 클래스 상당히 안좋아 보이면 개추'
// 완전히 인터페이스로 빼는 게 더 좋아 보인다.
#include "00pch.h"
#include "Scene.h"
#include "SceneIntro.h"
//#include "Button.h"
#include "30ObjectManager.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::Initialize()
{
	
}

void Scene::ShowDebug()
{
	static bool _IsDebugShow = false;

	//if (InputManager::GetInstance()->CheckF5ButtonUp())
	//{
	//	if (!_IsDebugShow)
	//		_IsDebugShow = true;
	//	else
	//		_IsDebugShow = false;

	//	InputManager::GetInstance()->Reset();
	//}

	if (!_IsDebugShow)
	{
		return;
	}
}

void Scene::Release()
{
	//m_pObjectManager->Release();

	// 오브젝트 매니저 제거
	//if (m_pObjectManager != nullptr)
	//{
	//	delete(m_pObjectManager);
	//	m_pObjectManager = nullptr;
	//}
}


void Scene::Render(std::shared_ptr<SSEngine> Renderer) 
{

}