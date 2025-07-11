#include "00pch.h"
#include "20GameProcess.h"


//////////////////////
//! ���� ������ �ؾ� �Ҳ�
//! ������ ���� ����
//! �������� ���� �ִ� �� ���� �ȵ�


//// static ��� ���� ���� (�޸� �Ҵ�)
ID2D1Factory* GameProcess::g_pD2DFactory = nullptr;
ID2D1HwndRenderTarget* GameProcess::g_pRenderTarget = nullptr;
ID2D1SolidColorBrush* GameProcess::g_pBrush = nullptr;
IDWriteFactory* GameProcess::g_pDWriteFactory = nullptr;
IDWriteTextFormat* GameProcess::g_pTextFormat = nullptr;


GameProcess::GameProcess()
    :m_hwnd(nullptr)
{
}

GameProcess::~GameProcess()
{
}

bool GameProcess::Initialize(HINSTANCE hInstance)
{

    // ������ Ŭ���� ���
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Direct2DWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    // â�� ���ø� ���
    RegisterClass(&wc);

    // ������ ����
    // ���� â �����
    m_hwnd = CreateWindow(
        L"Direct2DWindow",
        L"���̷�Ʈ",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (m_hwnd == NULL) return FALSE;

    // Direct2D �ʱ�ȭ
    if (FAILED(InitializeD2D(m_hwnd))) return FALSE;
    // 1. �Ŵ��� ����� ���� �������� ����
    //SSEngine::GetInstance()->Initialize(m_hwnd);
    // 2. ���Ͼȿ� �̰ɷ� �ٲٸ���
    // CreateManager();


    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hwnd);

    return TRUE;
}

void GameProcess::MessageLoop()
{
    // �޽��� ����
    MSG msg = {};

    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        else
        {
            GameLoop();
        }
    }
}

void GameProcess::GameLoop()
{
    OnRender();
}

LRESULT CALLBACK GameProcess::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
        GameProcess::OnRender();
        ValidateRect(hwnd, NULL);
        return 0;

    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        GameProcess::OnResize(width, height);
    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return TRUE;
}

// ���� �� �Լ� �Ⱦ� 
// �� ���� ����
HRESULT GameProcess::InitializeD2D(HWND hwnd)
{
    HRESULT hr = S_OK;

    // 1. D2D1Factory ����
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
    if (FAILED(hr)) return hr;

    // 2. ������ ũ�� ���
    RECT rc;
    GetClientRect(hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    // 3. HwndRenderTarget ����
    hr = g_pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, size),
        &g_pRenderTarget
    );
    if (FAILED(hr)) return hr;

    // 4. �귯�� ���� (�׸��⿡ ����� ����)
    hr = g_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Black),
        &g_pBrush
    );
    if (FAILED(hr)) return hr;

    // 5. DirectWrite �ʱ�ȭ (�ؽ�Ʈ ��������)
    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&g_pDWriteFactory)
    );
    if (FAILED(hr)) return hr;

    // 6. �ؽ�Ʈ ���� ����
    hr = g_pDWriteFactory->CreateTextFormat(
        L"����",
        NULL,
        DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        16.0f,
        L"ko-kr",
        &g_pTextFormat
    );

    return hr;
}

void GameProcess::OnRender()
{
    if (!g_pRenderTarget) return;

    // ������ ����
    g_pRenderTarget->BeginDraw();

    // ��� �����
    g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    // 1. �簢�� �׸���
    D2D1_RECT_F rectangle = D2D1::RectF(50, 50, 150, 100);

    // ä���� �簢��
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
    g_pRenderTarget->FillRectangle(rectangle, g_pBrush);

    // �簢�� �׵θ�
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
    g_pRenderTarget->DrawRectangle(rectangle, g_pBrush, 2.0f);

    // 2. �� �׸���
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(300, 100), 50, 50);

    // ä���� ��
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
    g_pRenderTarget->FillEllipse(ellipse, g_pBrush);

    // �� �׵θ�
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
    g_pRenderTarget->DrawEllipse(ellipse, g_pBrush, 2.0f);

    // 3. Ÿ�� �׸���
    D2D1_ELLIPSE oval = D2D1::Ellipse(D2D1::Point2F(500, 100), 80, 40);
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
    g_pRenderTarget->FillEllipse(oval, g_pBrush);

    // 4. �� �׸���
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Purple));
    g_pRenderTarget->DrawLine(
        D2D1::Point2F(50, 200),
        D2D1::Point2F(550, 250),
        g_pBrush,
        3.0f
    );

    // 5. �ձ� �𼭸� �簢�� �׸���
    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
        D2D1::RectF(50, 300, 200, 400),
        20, 20  // �ձ� �𼭸� ������
    );
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Orange));
    g_pRenderTarget->FillRoundedRectangle(roundedRect, g_pBrush);

    // 6. �ؽ�Ʈ �׸���
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
    D2D1_RECT_F textRect = D2D1::RectF(300, 300, 600, 400);
    g_pRenderTarget->DrawText(
        L"Direct2D�� �׸� �⺻ ������",
        wcslen(L"Direct2D�� �׸� �⺻ ������"),
        g_pTextFormat,
        textRect,
        g_pBrush
    );

    // ������ ����
    HRESULT hr = g_pRenderTarget->EndDraw();

    // ����̽� �ս� ���� ���� ó��
    if (hr == D2DERR_RECREATE_TARGET)
    {
        // RenderTarget�� ������ؾ� ��
        // ���� ���ӿ����� �� �κ��� ó���ؾ� ��
    }
}

void GameProcess::OnResize(UINT width, UINT height)
{
    if (g_pRenderTarget)
    {
        // ������ ũ�� ���� �� RenderTarget ũ�� ����
        g_pRenderTarget->Resize(D2D1::SizeU(width, height));
    }
}

void GameProcess::CleanupD2D()
{
    // ���ҽ� ���� (��������)
    if (g_pTextFormat)      g_pTextFormat->Release();
    if (g_pDWriteFactory)   g_pDWriteFactory->Release();
    if (g_pBrush)           g_pBrush->Release();
    if (g_pRenderTarget)    g_pRenderTarget->Release();
    if (g_pD2DFactory)      g_pD2DFactory->Release();
}