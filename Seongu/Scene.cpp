// Scene.cpp
#include "00pch.h"
#include "Scene.h"
#include "30GameObject.h"
#include "32Renderer.h"
#include "algorithm"

Scene::~Scene()
{
    std::cout << "[DEBUG] Scene destroyed, gameObjects.size(): " << m_gameObjects.size() << "\n";
}

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

void Scene::UpdateInput()
{

}




void Scene::Render(std::shared_ptr<SSEngine> renderer)
{
    // 1. 모든 Renderer 수집
    std::vector<std::pair<Renderer*, GameObject*>> renderData;

    for (auto& gameObject : m_gameObjects)
    {
        if (gameObject->IsActive())
        {
            auto rendererComp = gameObject->GetComponent<Renderer>();
            if (rendererComp && rendererComp->IsActive())
            {
                renderData.push_back({ rendererComp, gameObject.get() });
            }
        }
    }

    // 2. 레이어별 정렬
    std::sort(renderData.begin(), renderData.end(),
        [](const auto& a, const auto& b) {
            return a.first->GetLayer() < b.first->GetLayer();
        });

    // 3. 정렬된 순서로 렌더링
    for (auto& rd : renderData)
    {
        // 씬 -> 오브제 ->랜더러        (x)
        // 씬 -> 랜더러                (o)
        rd.first->Render(renderer);
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