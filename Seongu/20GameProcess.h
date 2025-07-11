#pragma once

class GameTimer;

class GameProcess
{
private:
	HWND m_hwnd;
	
	GameTimer* m_pTimer;

public:
	GameProcess();
	~GameProcess();

	bool Initialize(HINSTANCE hInstance);
	
	void MessageLoop();
	void GameLoop();

	void Release();

	static void OnResize(UINT width, UINT height);

public:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

