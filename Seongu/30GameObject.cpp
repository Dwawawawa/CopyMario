//30GameObject.cpp
#include "00pch.h"
#include "30GameObject.h"
#include "32Transform.h"

GameObject::GameObject()
{
	m_transform = AddComponent<Transform>();
}

GameObject::~GameObject()
{
	Release();
}

void GameObject::Initialize()
{
    for (auto& component : m_components)
    {
        component->Initialize();
    }
}

void GameObject::Update(float deltaTime)
{
    if (!m_isActive) return;

    for (auto& component : m_components)
    {
        if (component->IsActive())
        {
            component->Update(deltaTime);
        }
    }
}

void GameObject::Render(std::shared_ptr<SSEngine> renderer)
{
    if (!m_isActive) return;

    for (auto& component : m_components)
    {
        if (component->IsActive())
        {
            component->Render(renderer);
        }
    }
}

void GameObject::Release()
{
    for (auto& component : m_components)
    {
        component->Release();
    }
    m_components.clear();
}