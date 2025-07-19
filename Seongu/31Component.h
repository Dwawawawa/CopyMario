#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace mycore
{
    using MessageID = uint32_t;
}


class GameObject;

class Component
{
public:
    virtual ~Component() = default;
    virtual void Update(float dt) = 0;
    virtual void OnEvent(const std::string& ev) abstract;

    void HandleMessage(mycore::MessageID msg, void* data)
    {
        auto it = m_MessageHandlers.find(msg);
        if (it != m_MessageHandlers.end())
        {
            for (auto& handler : it->second)
                handler(data);
        }
    }

    using HandlerType = std::function<void(void*)>;
    void RegisterMessageHandler(mycore::MessageID msg, HandlerType handler)
    {
        m_MessageHandlers[msg].push_back(std::move(handler));
    }

    void SetOwner(GameObject* owner) { m_Owner = owner; }

protected:
    GameObject* m_Owner = nullptr;

    std::unordered_map<mycore::MessageID, std::vector<HandlerType>> m_MessageHandlers;
};