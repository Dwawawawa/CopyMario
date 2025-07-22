#pragma once

class GameTimer;
////////////
//ÀÓ½Ã·Î ¾À
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


	////////////////
	// ÇïÇÁ ÇÔ¼ö
	void OnResize(int width, int height);



private:
	std::shared_ptr<SSEngine> m_Renderer = nullptr; 

	GameTimer*	m_pTimer	  = 0;
	float		m_deltaTime   = 0;
	float		tempTime      = 0;
	int			tempint       = 0;

	std::wostringstream woss_m_deltaTime  ;

};

