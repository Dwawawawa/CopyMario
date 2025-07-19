// Scene.h
#pragma once
#include <memory>
#include <vector>

class GameObject;
class SSEngine;

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

    virtual void Initialize();
    virtual void Update(float deltaTime);
    virtual void Render(std::shared_ptr<SSEngine> renderer);
    virtual void Release();

    // GameObject 包府
    GameObject* CreateGameObject();
    void DestroyGameObject(GameObject* gameObject);

    template<typename T>
    T* CreateGameObject()
    {
        static_assert(std::is_base_of_v<GameObject, T>, "T must inherit from GameObject");

        // emplace_back栏肺 流立 积己
        m_gameObjects.emplace_back(std::make_unique<T>());

        T* ptr = static_cast<T*>(m_gameObjects.back().get());
        ptr->Initialize();
        return ptr;
    }

protected:
    std::vector<std::unique_ptr<GameObject>> m_gameObjects;
    std::vector<GameObject*> m_pendingDestroy;

    void ProcessPendingDestroy();
};