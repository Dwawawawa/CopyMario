// SceneManager.h
#pragma once
#include "00pch.h"
#include "Scene.h"


class SSEngine;


enum class SceneType
{
    INTRO,
    INGAME1_1,
    BLACK,
    OUTRO,
    COUNT
};

class SceneManager
{
private:
    static SceneManager* s_Instance;

    // ���� ��
    std::unique_ptr<Scene> m_CurrentScene;
    SceneType m_CurrentSceneType;

    // �� ���丮 �Լ���
    std::unordered_map<SceneType, std::function<std::unique_ptr<Scene>()>> m_SceneFactories;

    // ������ ���� 
    std::shared_ptr<SSEngine> m_Renderer;

public:
    // �̱��� ����
    static SceneManager* GetInstance();
    static void DestroyInstance();

private:
    SceneManager();
    ~SceneManager();

public:
    // �ʱ�ȭ �� ����
    void Initialize();
    void Release();

    // ������Ʈ �� ������
    void Update(float deltaTime);
    void UpdateInput();
    void Render(std::shared_ptr<SSEngine> Renderer);

    // �� ����
    void ChangeScene(SceneType nextScene);
    void RegisterScene(SceneType type, std::function<std::unique_ptr<Scene>()> factory);

    // ���� �� ����
    Scene* GetCurrentScene() const { return m_CurrentScene.get(); }
    SceneType GetCurrentSceneType() const { return m_CurrentSceneType; }


private:
    // ���� �Լ���
    std::unique_ptr<Scene> CreateScene(SceneType sceneType);
    void CompleteSceneTransition();
};

