#pragma once

#include <memory>

class GameTimer;
class SceneManager;

class GameProcess
{
public:
	GameProcess();
	~GameProcess();

	bool Initialize(HINSTANCE hInstance);
	void Finalize();

	void Run();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
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
	std::wostringstream woss_m_deltaTime  ;
	std::wostringstream woss_m_deltaTimeMS;
	std::wostringstream woss_m_totalTime  ;



};

