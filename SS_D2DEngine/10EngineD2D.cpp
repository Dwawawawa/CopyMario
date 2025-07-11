#include "00pch.h"
#include "10EngineD2D.h"

SSEngine* SSEngine::m_pInstance = nullptr;

SSEngine::SSEngine()
	: m_pFactory(nullptr), m_pDWriteFactory(nullptr), m_pNowBrush(nullptr),
	m_pRenderTarget(nullptr), m_pTextFormat(nullptr)//, m_pImageFactory(nullptr)
	//, m_MainFont(L""), m_MainFontSize(0)
{
}

SSEngine::~SSEngine()
{
}

SSEngine* SSEngine::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_pInstance = new SSEngine;
	}

	return m_pInstance;
}

//ID2D1HwndRenderTarget* SSEngine::GetRenderTarget()
//{
//	return m_pRenderTarget;
//}

HRESULT SSEngine::Initialize(HWND hwnd)
{
	HRESULT hr = S_OK;

	// 1. D2D1Factory 생성
	// Factory 객체를 생성한다.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
	if (FAILED(hr)) return hr;

	/// 렌더 타겟 생성
	// 2. 윈도우 크기 얻기
	RECT rc;
	GetClientRect(hwnd, &rc);	// 클라이언트 영역의 좌표를 얻는다.
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	// 3. HwndRenderTarget 생성
	// Render Target을 생성한다.
	hr = m_pFactory->CreateHwndRenderTarget(
		RenderTargetProperties(),
		HwndRenderTargetProperties(hwnd, size),
		&m_pRenderTarget);
	if (FAILED(hr)) return hr;

	// 4. 브러시 생성 (그리기에 사용할 색상)
	// 기본 브러쉬 생성
	hr = m_pRenderTarget->CreateSolidColorBrush(ColorF(ColorF::WhiteSmoke), &m_pNowBrush);
	if (FAILED(hr)) return hr;

	// 5. DirectWrite 초기화 (텍스트 렌더링용)
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	if (FAILED(hr)) return hr;

	// 6. 텍스트 포맷 생성
	hr = m_pDWriteFactory->CreateTextFormat(
		L"굴림",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"ko-kr",
		&m_pTextFormat
	);

	return hr;


}

void SSEngine::Release()
{
	SafeRelease(&m_pFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pNowBrush);

	// 텍스트 관련
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pTextFormat);

	// Instance 삭제
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void SSEngine::BeginRender()
{
	m_pRenderTarget->BeginDraw();
	// 화면을 지운다.
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}

void SSEngine::EndRender()
{
	// 렌더링 종료
	HRESULT hr = m_pRenderTarget->EndDraw();

	// 디바이스 손실 등의 오류 처리
	if (hr == D2DERR_RECREATE_TARGET)
	{
		// RenderTarget을 재생성해야 함
		// 실제 게임에서는 이 부분을 처리해야 함
	}
}

void SSEngine::DrawSomething()
{
	if (!m_pRenderTarget) return;


	// 배경 지우기
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	// 1. 사각형 그리기
	D2D1_RECT_F rectangle = D2D1::RectF(50, 50, 150, 100);

	// 채워진 사각형
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
	m_pRenderTarget->FillRectangle(rectangle, m_pNowBrush);

	// 사각형 테두리
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	m_pRenderTarget->DrawRectangle(rectangle, m_pNowBrush, 2.0f);

	// 2. 원 그리기
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(300, 100), 50, 50);

	// 채워진 원
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
	m_pRenderTarget->FillEllipse(ellipse, m_pNowBrush);

	// 원 테두리
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	m_pRenderTarget->DrawEllipse(ellipse, m_pNowBrush, 2.0f);

	// 3. 타원 그리기
	D2D1_ELLIPSE oval = D2D1::Ellipse(D2D1::Point2F(500, 100), 80, 40);
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
	m_pRenderTarget->FillEllipse(oval, m_pNowBrush);

	// 4. 선 그리기
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Purple));
	m_pRenderTarget->DrawLine(
		D2D1::Point2F(50, 200),
		D2D1::Point2F(550, 250),
		m_pNowBrush,
		3.0f
	);

	// 5. 둥근 모서리 사각형 그리기
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
		D2D1::RectF(50, 300, 200, 400),
		20, 20  // 둥근 모서리 반지름
	);
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Orange));
	m_pRenderTarget->FillRoundedRectangle(roundedRect, m_pNowBrush);

	// 6. 텍스트 그리기
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	D2D1_RECT_F textRect = D2D1::RectF(300, 300, 600, 400);
	m_pRenderTarget->DrawText(
		L"Direct2D로 그린 기본 도형들",
		wcslen(L"Direct2D로 그린 기본 도형들"),
		m_pTextFormat,
		textRect,
		m_pNowBrush
	);


}


void SSEngine::DrawText(float x, float y, const WCHAR* pch, ...)
{
	// 포맷 스트링 관련 버퍼, 가변인자 처리
	va_list ap;			// 현재 읽고 있는 번지를 기억
	va_start(ap, pch);	// ap 포인터 변수가 첫번째 가변인수를 가리키도록 초기화

	int len = _vscwprintf(pch, ap) + 1;
	WCHAR* buffer = new WCHAR[len];

	vswprintf_s(buffer, len, pch, ap);
	va_end(ap);

	int nLen = lstrlen(buffer);

	// 폰트별 차지하는 영역 계산 필요
	m_pRenderTarget->DrawText(buffer, wcslen(buffer), m_pTextFormat,
		D2D1::RectF((float)x, (float)y, (float)(x + 500), (float)(y + 40)),
		m_pNowBrush);

	// 포맷관련 버퍼 삭제
	delete[] buffer;
}

