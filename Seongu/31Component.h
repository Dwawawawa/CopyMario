// 31Component.h
#include "00pch.h"
#pragma once

class GameObject;
class SSEngine;

class Component
{
public:
    Component() = default;
    virtual ~Component() = default;

    virtual void Initialize() {}
    virtual void Update(float deltaTime) {}
    virtual void Render(std::shared_ptr<SSEngine> renderer) {}
    virtual void Release() {}

    void SetOwner(GameObject* owner) { m_owner = owner; }
    GameObject* GetOwner() const { return m_owner; }

    bool IsActive() const { return m_isActive; }
    void SetActive(bool active) { m_isActive = active; }

protected:
    GameObject* m_owner = nullptr;
    bool m_isActive = true;
};