#pragma once

class GameProcess
{
private:
	HWND m_hwnd;
	
public:
	/// <summary>
	/// 이렇게 스태틱으로 박으면 큰일 나는데;ㅣ;;
	/// </summary>
	
	static ID2D1Factory*           g_pD2DFactory      ;
	static ID2D1HwndRenderTarget*  g_pRenderTarget    ;
	static ID2D1SolidColorBrush*   g_pBrush           ;
	static IDWriteFactory*         g_pDWriteFactory   ;
	static IDWriteTextFormat*      g_pTextFormat      ;


public:
	GameProcess();
	~GameProcess();

	bool Initialize(HINSTANCE hInstance);
	
	void MessageLoop();
	void GameLoop();

	//얘가 릴리즈임
	void CleanupD2D();


	static void OnRender();
	static void OnResize(UINT width, UINT height);

public:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HRESULT InitializeD2D(HWND hwnd);
};

