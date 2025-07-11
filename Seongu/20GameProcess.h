#pragma once

class GameTimer;
class SceneManager;

class GameProcess
{
private:
	HWND m_hwnd;
	
	GameTimer* m_pTimer;
	

public:
	GameProcess();
	~GameProcess();

	bool Initialize(HINSTANCE hInstance);
	void Release();

	void MessageLoop();
	void GameLoop();

	void CreateManager();
	//void LoadResources();
	//void CreateFileFolder();
	//void CreateScene();



	////////////////
	// ÇïÇÁ ÇÔ¼ö
	static void OnResize(UINT width, UINT height);

public:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

