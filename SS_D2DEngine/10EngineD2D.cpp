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

	// 1. D2D1Factory ����
	// Factory ��ü�� �����Ѵ�.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
	if (FAILED(hr)) return hr;

	/// ���� Ÿ�� ����
	// 2. ������ ũ�� ���
	RECT rc;
	GetClientRect(hwnd, &rc);	// Ŭ���̾�Ʈ ������ ��ǥ�� ��´�.
	D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

	// 3. HwndRenderTarget ����
	// Render Target�� �����Ѵ�.
	hr = m_pFactory->CreateHwndRenderTarget(
		RenderTargetProperties(),
		HwndRenderTargetProperties(hwnd, size),
		&m_pRenderTarget);
	if (FAILED(hr)) return hr;

	// 4. �귯�� ���� (�׸��⿡ ����� ����)
	// �⺻ �귯�� ����
	hr = m_pRenderTarget->CreateSolidColorBrush(ColorF(ColorF::WhiteSmoke), &m_pNowBrush);
	if (FAILED(hr)) return hr;

	// 5. DirectWrite �ʱ�ȭ (�ؽ�Ʈ ��������)
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_pDWriteFactory),
		reinterpret_cast<IUnknown**>(&m_pDWriteFactory));
	if (FAILED(hr)) return hr;

	// 6. �ؽ�Ʈ ���� ����
	hr = m_pDWriteFactory->CreateTextFormat(
		L"����",
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

	// �ؽ�Ʈ ����
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pTextFormat);

	// Instance ����
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void SSEngine::BeginRender()
{
	m_pRenderTarget->BeginDraw();
	// ȭ���� �����.
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));
}

void SSEngine::EndRender()
{
	// ������ ����
	HRESULT hr = m_pRenderTarget->EndDraw();

	// ����̽� �ս� ���� ���� ó��
	if (hr == D2DERR_RECREATE_TARGET)
	{
		// RenderTarget�� ������ؾ� ��
		// ���� ���ӿ����� �� �κ��� ó���ؾ� ��
	}
}

void SSEngine::DrawSomething()
{
	if (!m_pRenderTarget) return;


	// ��� �����
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	// 1. �簢�� �׸���
	D2D1_RECT_F rectangle = D2D1::RectF(50, 50, 150, 100);

	// ä���� �簢��
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
	m_pRenderTarget->FillRectangle(rectangle, m_pNowBrush);

	// �簢�� �׵θ�
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	m_pRenderTarget->DrawRectangle(rectangle, m_pNowBrush, 2.0f);

	// 2. �� �׸���
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(300, 100), 50, 50);

	// ä���� ��
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
	m_pRenderTarget->FillEllipse(ellipse, m_pNowBrush);

	// �� �׵θ�
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	m_pRenderTarget->DrawEllipse(ellipse, m_pNowBrush, 2.0f);

	// 3. Ÿ�� �׸���
	D2D1_ELLIPSE oval = D2D1::Ellipse(D2D1::Point2F(500, 100), 80, 40);
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
	m_pRenderTarget->FillEllipse(oval, m_pNowBrush);

	// 4. �� �׸���
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Purple));
	m_pRenderTarget->DrawLine(
		D2D1::Point2F(50, 200),
		D2D1::Point2F(550, 250),
		m_pNowBrush,
		3.0f
	);

	// 5. �ձ� �𼭸� �簢�� �׸���
	D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
		D2D1::RectF(50, 300, 200, 400),
		20, 20  // �ձ� �𼭸� ������
	);
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Orange));
	m_pRenderTarget->FillRoundedRectangle(roundedRect, m_pNowBrush);

	// 6. �ؽ�Ʈ �׸���
	m_pNowBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	D2D1_RECT_F textRect = D2D1::RectF(300, 300, 600, 400);
	m_pRenderTarget->DrawText(
		L"Direct2D�� �׸� �⺻ ������",
		wcslen(L"Direct2D�� �׸� �⺻ ������"),
		m_pTextFormat,
		textRect,
		m_pNowBrush
	);


}


void SSEngine::DrawText(float x, float y, const WCHAR* pch, ...)
{
	// ���� ��Ʈ�� ���� ����, �������� ó��
	va_list ap;			// ���� �а� �ִ� ������ ���
	va_start(ap, pch);	// ap ������ ������ ù��° �����μ��� ����Ű���� �ʱ�ȭ

	int len = _vscwprintf(pch, ap) + 1;
	WCHAR* buffer = new WCHAR[len];

	vswprintf_s(buffer, len, pch, ap);
	va_end(ap);

	int nLen = lstrlen(buffer);

	// ��Ʈ�� �����ϴ� ���� ��� �ʿ�
	m_pRenderTarget->DrawText(buffer, wcslen(buffer), m_pTextFormat,
		D2D1::RectF((float)x, (float)y, (float)(x + 500), (float)(y + 40)),
		m_pNowBrush);

	// ���˰��� ���� ����
	delete[] buffer;
}

