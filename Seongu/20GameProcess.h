#pragma once

class GameProcess
{
private:
	HWND m_hwnd;
	
public:
	/// <summary>
	/// �̷��� ����ƽ���� ������ ū�� ���µ�;��;;
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

	//�갡 ��������
	void CleanupD2D();


	static void OnRender();
	static void OnResize(UINT width, UINT height);

public:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static HRESULT InitializeD2D(HWND hwnd);
};

