#include "00pch.h"
#include "SceneIntro.h"
#include "30GameObject.h"
#include "30ObjectManager.h"

SceneIntro::SceneIntro()
{

}

SceneIntro::~SceneIntro()
{
	
}

void SceneIntro::Initialize()
{
	m_pObjectManager = std::make_unique<ObjectManager>();
	
}

void SceneIntro::Update(float dTime)
{
	m_pObjectManager->Update(dTime);
}

void SceneIntro::Render(std::shared_ptr<SSEngine> Renderer)
{
	m_pObjectManager->Render(Renderer);
}

void SceneIntro::ShowDebug()
{

}

void SceneIntro::Release()
{

}


