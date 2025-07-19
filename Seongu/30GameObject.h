// 30GameObject.h
#pragma once
#include <memory>
#include <vector>
#include <typeinfo>

class SSEngine;
class Component;
class Transform;

class GameObject
{
public:
    GameObject();
    ~GameObject();

    void Initialize();
    void Update(float deltaTime);
    void Render(std::shared_ptr<SSEngine> renderer);
    void Release();

    // 컴포넌트 관리
    template<typename T>
    T* AddComponent()
    {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

        // 1. 컴포넌트를 생성하고 
        auto component = std::make_unique<T>();
        // 2. 소유자를 설정
        component->SetOwner(this);
        // 3. 초기화 
        component->Initialize();
        // 4. 포인터 저장
        T* ptr = component.get();
        // 4. 컨테이너에 이동 
        m_components.push_back(std::move(component));
        
        return ptr;
    }

    template<typename T>
    T* GetComponent()
    {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

        for (auto& component : m_components)
        {
            if (T* casted = dynamic_cast<T*>(component.get()))
            {
                return casted;
            }
        }
        return nullptr;
    }

    template<typename T>
    void RemoveComponent()
    {
        static_assert(std::is_base_of_v<Component, T>, "T must inherit from Component");

        static_assert(!std::is_same_v<T, Transform>, "Cannot remove Transform component - it's required!");


        m_components.erase(
            std::remove_if(m_components.begin(), m_components.end(),
                [](const std::unique_ptr<Component>& component) {
                    return dynamic_cast<T*>(component.get()) != nullptr;
                }),
            m_components.end());
    }

    // Transform은 필수 컴포넌트로 항상 존재
    Transform* GetTransform() const { return m_transform; }

    bool IsActive() const { return m_isActive; }
    void SetActive(bool active) { m_isActive = active; }


private:
    std::vector<std::unique_ptr<Component>> m_components;
    Transform* m_transform = nullptr; // 빠른 접근을 위한 캐싱 
    bool m_isActive = true;
};

