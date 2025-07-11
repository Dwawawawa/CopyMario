#pragma once

///////////////
// �̰� ���� �ǹ̰� �ִ°���?
// �̰� ���� �״µ�...
//#ifdef _WIN64
//#ifdef _DEBUG
//#pragma comment(lib, "SSEngine_D2D64d")
//#else
//#pragma comment(lib, "SSEngine_D2D64")	
//#endif
//#else
//#ifdef _DEBUG
//#pragma comment(lib, "SSEngine_D2Dd")
//#else
//#pragma comment(lib, "SSEngine_D2D")	
//#endif
//#endif

#include <d2d1.h>
#include <d2d1helper.h>

// 2���� ���ͻ��
#include <dcommon.h>						// IWICImagingFactory
#include <dwrite.h>							// writefactory
#include <stdio.h>							// IWICImagingFactory

#include <wincodec.h>						// IWICImagingFactory
//#include <xstring>

#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")	// WIC

using namespace D2D1;
using namespace std;

#include <string>

class SSEngine
{
private:
	SSEngine();
	~SSEngine();

	static SSEngine* m_pInstance;

public:
	static SSEngine* GetInstance();

private:
	ID2D1Factory* m_pFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;


	///////////////////////////////////////////////////////
	// �ؽ�Ʈ ���丮
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	// ��Ʈ ����
	//wstring m_MainFont;
	//FLOAT m_MainFontSize;
	//m_MainFont = L"���� ���";
	//m_MainFontSize = 20;

	///////////////////////////////////////////////////////////
	// �̹��� ���丮
	//IWICImagingFactory* m_pImageFactory;

	/////////////////////////////////////////////////////
	// ���� �귯��
	ID2D1SolidColorBrush* m_pNowBrush;

public:
	HRESULT Initialize(HWND hwnd);
	void Release();

	void BeginRender();
	void EndRender();


	

	void ResizeInEngine(UINT width, UINT height)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}

public:
	void DrawSomething();
	/// ���� ���
	void DrawText(float x, float y, const WCHAR* pch, ...);

private:

	template<class Interface>
	inline void
	SafeRelease(
		Interface** ppInterfaceToRelease
	)
	{
		if (*ppInterfaceToRelease != NULL)
		{
			(*ppInterfaceToRelease)->Release();

			(*ppInterfaceToRelease) = NULL;
		}
	}
};
