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

    // 현재 씬
    std::unique_ptr<Scene> m_CurrentScene;
    SceneType m_CurrentSceneType;

    // 씬 팩토리 함수들
    std::unordered_map<SceneType, std::function<std::unique_ptr<Scene>()>> m_SceneFactories;

    // 렌더러 참조 
    std::shared_ptr<SSEngine> m_Renderer;

public:
    // 싱글톤 관련
    static SceneManager* GetInstance();
    static void DestroyInstance();

private:
    SceneManager();
    ~SceneManager();

public:
    // 초기화 및 정리
    void Initialize();
    void Release();

    // 업데이트 및 렌더링
    void Update(float deltaTime);
    void UpdateInput();
    void Render(std::shared_ptr<SSEngine> Renderer);

    // 씬 관리
    void ChangeScene(SceneType nextScene);
    void RegisterScene(SceneType type, std::function<std::unique_ptr<Scene>()> factory);

    // 현재 씬 정보
    Scene* GetCurrentScene() const { return m_CurrentScene.get(); }
    SceneType GetCurrentSceneType() const { return m_CurrentSceneType; }


private:
    // 내부 함수들
    std::unique_ptr<Scene> CreateScene(SceneType sceneType);
    void CompleteSceneTransition();
};

