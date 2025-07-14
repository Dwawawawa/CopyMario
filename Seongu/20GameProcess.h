#pragma once
#include "19WndBase.h" 
#include <memory>

class GameTimer;
class SceneManager;

class GameProcess : public NzWndBase
{
public:
	GameProcess() = default;
	~GameProcess() = default;

	bool Initialize(HINSTANCE hInstance);
	void Finalize();

	void Run();
	
private:
	void UpdateTime();
	void UpdateInput();
	void UpdateLogic();

	void Render();

	
	//void LoadResources();
	//void CreateFileFolder();
	//void CreateScene();



	////////////////
	// ÇïÇÁ ÇÔ¼ö
	void OnResize(int width, int height);


	std::shared_ptr<SSEngine> m_Renderer = nullptr; 

	GameTimer* m_pTimer;
	float m_deltaTime;
	float m_deltaTimeMS;
	float m_totalTime;
	



};

