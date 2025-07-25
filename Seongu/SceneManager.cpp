// SceneManager.cpp
// Singleton SceneManager
#include "00pch.h"
#include "SceneManager.h"
#include "SceneIngame.h"
#include "10EngineD2D.h" // ������ ������ ���

SceneManager* SceneManager::s_Instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
    if (s_Instance == nullptr)
    {
        s_Instance = new SceneManager();
    }
    return s_Instance;
}

void SceneManager::DestroyInstance()
{
    if (s_Instance != nullptr)
    {
        delete s_Instance;
        s_Instance = nullptr;
    }
}

SceneManager::SceneManager()
    : m_CurrentScene(nullptr)    
    , m_CurrentSceneType(SceneType::INGAME)
    , m_Renderer(nullptr)
{
}

SceneManager::~SceneManager()
{

}

void SceneManager::Initialize()
{
    RegisterScene(SceneType::INGAME, []() { return std::make_unique<SceneIngame>(); });

    // �ʱ� �� ����
    m_CurrentScene = CreateScene(SceneType::INGAME);
    m_CurrentSceneType = SceneType::INGAME;

    if (m_CurrentScene)
    {
        m_CurrentScene->Initialize();
    }
}

void SceneManager::Release()
{
    if (m_CurrentScene)
    {
        m_CurrentScene->Release();
        m_CurrentScene.reset();
    }

    //if (m_NextScene)
    //{
    //    m_NextScene->Release();
    //    m_NextScene.reset();
    //}

    m_SceneFactories.clear();
}

void SceneManager::Update(float deltaTime)
{
    // ���� �� ������Ʈ
    if (m_CurrentScene)
    {
        m_CurrentScene->Update(deltaTime);
    }
}

void SceneManager::UpdateInput()
{
    if (m_CurrentScene)
    {
        m_CurrentScene->UpdateInput();
    }
}

void SceneManager::Render(std::shared_ptr<SSEngine> Renderer)
{
    if (!Renderer) return;

    // ���� �� ������
    if (m_CurrentScene)
    {
        m_CurrentScene->Render(Renderer);
    }
}

void SceneManager::ChangeScene(SceneType type)
{
    // ���� ���� ���� ������ �����Ϸ��� ��� ����
    if (m_CurrentSceneType == type)
    {
        return;
    }

    // ���ο� �� ����
    auto newScene = CreateScene(type);
    if (!newScene)
    {
        std::cout << "���� ������ �� �����ϴ�: " << static_cast<int>(type) << std::endl;
        return;
    }

    // ���� �� ����
    if (m_CurrentScene)
    {
        m_CurrentScene->Release();
    }
    
    // �� ������ ��ü
    m_CurrentScene = std::move(newScene);
    m_CurrentSceneType = type;
    
    // �� �� �ʱ�ȭ
    if (m_CurrentScene)
    {
        m_CurrentScene->Initialize();
    }

}

void SceneManager::RegisterScene(SceneType type, std::function<std::unique_ptr<Scene>()> factory)
{
    m_SceneFactories[type] = factory;
}


std::unique_ptr<Scene> SceneManager::CreateScene(SceneType sceneType)
{
    auto it = m_SceneFactories.find(sceneType);
    if (it != m_SceneFactories.end())
    {
        return it->second();
    }
    return nullptr;
}

void SceneManager::CompleteSceneTransition()
{
    if (m_CurrentScene)
    {
        m_CurrentScene->Release();
    }

}