/////
// �� Ŭ���� ����� ������ ���̸� ����'
// ������ �������̽��� ���� �� �� ���� ���δ�.
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

	// ������Ʈ �Ŵ��� ����
	//if (m_pObjectManager != nullptr)
	//{
	//	delete(m_pObjectManager);
	//	m_pObjectManager = nullptr;
	//}
}


void Scene::Render(std::shared_ptr<SSEngine> Renderer) 
{

}