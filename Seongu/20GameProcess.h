#pragma once

class GameTimer;
class SceneManager;

class GameProcess
{
public:
	GameProcess();
	~GameProcess();

	bool Initialize(HINSTANCE hInstance);
	void Release();

	void Run();
	

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
	

	void UpdateTime();
	void UpdateInput();
	void UpdateLogic();
	void Render();

	void CreateManager();
	//void LoadResources();
	//void CreateFileFolder();
	//void CreateScene();



	////////////////
	// ���� �Լ�
	static void OnResize(UINT width, UINT height);

private:
	/// <summary>
	/// �굵 �����ϰ�
	/// </summary>
	HWND m_hwnd;

	GameTimer* m_pTimer;
	float m_deltaTime;
	float m_deltaTimeMS;
	float m_totalTime;
	



};

