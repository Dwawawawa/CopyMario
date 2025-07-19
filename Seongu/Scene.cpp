// Scene.cpp
#include "00pch.h"
#include "Scene.h"
#include "30GameObject.h"

void Scene::Initialize()
{
    for (auto& gameObject : m_gameObjects)
    {
        gameObject->Initialize();
    }
}

void Scene::Update(float deltaTime)
{
    // 활성화된 게임오브젝트들만 업데이트
    for (auto& gameObject : m_gameObjects)
    {
        if (gameObject->IsActive())
        {
            gameObject->Update(deltaTime);
        }
    }

    ProcessPendingDestroy();
}

void Scene::Render(std::shared_ptr<SSEngine> renderer)
{
    for (auto& gameObject : m_gameObjects)
    {
        if (gameObject->IsActive())
        {
            gameObject->Render(renderer);
        }
    }
}

void Scene::Release()
{
    for (auto& gameObject : m_gameObjects)
    {
        gameObject->Release();
    }
    m_gameObjects.clear();
    m_pendingDestroy.clear();
}

GameObject* Scene::CreateGameObject()
{
    auto gameObject = std::make_unique<GameObject>();
    GameObject* ptr = gameObject.get();
    m_gameObjects.push_back(std::move(gameObject));
    ptr->Initialize();
    return ptr;
}

void Scene::DestroyGameObject(GameObject* gameObject)
{
    m_pendingDestroy.push_back(gameObject);
}

void Scene::ProcessPendingDestroy()
{
    for (GameObject* gameObject : m_pendingDestroy)
    {
        auto it = std::find_if(m_gameObjects.begin(), m_gameObjects.end(),
            [gameObject](const std::unique_ptr<GameObject>& ptr) {
                return ptr.get() == gameObject;
            });

        if (it != m_gameObjects.end())
        {
            (*it)->Release();
            m_gameObjects.erase(it);
        }
    }
    m_pendingDestroy.clear();
}