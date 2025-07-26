// SceneIngame.cpp 
#include "00pch.h"
#include "SceneIngame.h"
#include "30GameObject.h"
#include "32Transform.h"
#include "32Renderer.h"
#include "32InputComponent.h"

SceneIngame::SceneIngame()
{

}

SceneIngame::~SceneIngame()
{
    if (!m_gameObjects.empty())
    {

    }
}


void SceneIngame::Initialize()
{
    Scene::Initialize();

    if (Debug::Scene) return;

    
}

void SceneIngame::Update(float dTime)
{
    Scene::Update(dTime);
}


