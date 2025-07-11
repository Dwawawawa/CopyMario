#pragma once

///////////////
// 이게 무슨 의미가 있는거지?
// 이것 땜에 죽는데...
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

// 2차원 벡터사용
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
	// 텍스트 팩토리
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	// 폰트 관련
	//wstring m_MainFont;
	//FLOAT m_MainFontSize;
	//m_MainFont = L"맑은 고딕";
	//m_MainFontSize = 20;

	///////////////////////////////////////////////////////////
	// 이미지 팩토리
	//IWICImagingFactory* m_pImageFactory;

	/////////////////////////////////////////////////////
	// 현재 브러쉬
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
	/// 문자 출력
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
