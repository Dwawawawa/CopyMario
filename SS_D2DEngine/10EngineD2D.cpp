#include "00pch.h"
#include "10EngineD2D.h"




void SSEngine::Initialize(HWND hwnd)
{
	m_hwnd = hwnd;

	CreateDeviceAndSwapChain(hwnd);
	CreateRenderTargets();
	CreateWriteResource();

	ComPtr<IWICImagingFactory> wicFactory;

	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&wicFactory));

	DX::ThrowIfFailed(hr);

	m_wicFactory = wicFactory;

}


void SSEngine::Uninitialize()
{
	ReleaseRenderTargets();

	m_wicFactory = nullptr;

	m_targetBitmap = nullptr;
	m_brush = nullptr;

	m_d2dContext = nullptr;
	m_d2dDevice = nullptr;

	m_swapChain = nullptr;
	m_d3dDevice = nullptr;
}

void SSEngine::Resize(UINT width, UINT height)
{
	if (nullptr == m_swapChain) return; // 초기화 전에 호출이 될 수 있음.
	ReleaseRenderTargets();

	// 스왑체인 크기 조정 후 렌더 타겟 재생성
	DX::ThrowIfFailed(m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0));

	CreateRenderTargets();
}

void SSEngine::DrawLine(float x1, float y1, float x2, float y2, const D2D1::ColorF& color)
{
	m_brush->SetColor(color);
	m_d2dContext->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), m_brush.Get());
}

void SSEngine::DrawCircle(float x, float y, float radius, const D2D1::ColorF& color)
{
	m_brush->SetColor(color);
	m_d2dContext->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), m_brush.Get());

}

void SSEngine::DrawRectangle(float left, float top, float right, float bottom, const D2D1::ColorF& color)
{
	m_brush->SetColor(color);
	m_d2dContext->DrawRectangle(D2D1::Rect(left, top, right, bottom), m_brush.Get());
}

void SSEngine::DrawBitmap(ID2D1Bitmap1* bitmap, D2D1_RECT_F dest)
{
	m_d2dContext->DrawBitmap(bitmap, dest);
}

void SSEngine::DrawBitmap(ID2D1Bitmap1* bitmap, D2D1_RECT_F destRect, D2D1_RECT_F srcRect, float opacity)
{
	m_d2dContext->DrawBitmap(
		bitmap,
		destRect,
		opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		srcRect
	);
}

void SSEngine::DrawMessage(const wchar_t* text, float left, float top, float width, float height, const D2D1::ColorF& color)
{
	if (nullptr == m_textBrush)
	{
		m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(color), &m_textBrush);
	}

	m_textBrush->SetColor(color);
	D2D1_RECT_F layoutRect = D2D1::RectF(left, top, left + width, top + height);

	m_d2dContext->DrawTextW(
		text,
		static_cast<UINT32>(wcslen(text)),
		m_textFormat.Get(),
		layoutRect,
		m_textBrush.Get(),
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL);
}

void SSEngine::SetTransform(const D2D1_MATRIX_3X2_F tm)
{
	if (m_d2dContext) m_d2dContext->SetTransform(tm);
}


void SSEngine::RenderBegin()
{
	m_d2dContext->BeginDraw();
	m_d2dContext->Clear(D2D1::ColorF(D2D1::ColorF::White)); // 배경을 흰색으로 초기화
}


void SSEngine::RenderEnd(bool bPresent)
{
	m_d2dContext->EndDraw();

	if (bPresent)
	{
		Present();
	}
}