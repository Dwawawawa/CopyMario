#include "00pch.h"
#include "20GameProcess.h"


//////////////////////
//! 이제 다음에 해야 할껀
//! 엔진을 빼는 거임
//! 전역으로 박혀 있는 게 말이 안돼


//// static 멤버 변수 정의 (메모리 할당)
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

    // 윈도우 클래스 등록
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Direct2DWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    // 창의 템플릿 등록
    RegisterClass(&wc);

    // 윈도우 생성
    // 실제 창 만들기
    m_hwnd = CreateWindow(
        L"Direct2DWindow",
        L"다이렉트",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInstance, NULL
    );

    if (m_hwnd == NULL) return FALSE;

    // Direct2D 초기화
    if (FAILED(InitializeD2D(m_hwnd))) return FALSE;
    // 1. 매니저 만들기 전에 엔진직접 접근
    //SSEngine::GetInstance()->Initialize(m_hwnd);
    // 2. 내일안에 이걸로 바꾸리라
    // CreateManager();


    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hwnd);

    return TRUE;
}

void GameProcess::MessageLoop()
{
    // 메시지 루프
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

// 이제 이 함수 안씀 
// 곧 삭제 예정
HRESULT GameProcess::InitializeD2D(HWND hwnd)
{
    HRESULT hr = S_OK;

    // 1. D2D1Factory 생성
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &g_pD2DFactory);
    if (FAILED(hr)) return hr;

    // 2. 윈도우 크기 얻기
    RECT rc;
    GetClientRect(hwnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    // 3. HwndRenderTarget 생성
    hr = g_pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(hwnd, size),
        &g_pRenderTarget
    );
    if (FAILED(hr)) return hr;

    // 4. 브러시 생성 (그리기에 사용할 색상)
    hr = g_pRenderTarget->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Black),
        &g_pBrush
    );
    if (FAILED(hr)) return hr;

    // 5. DirectWrite 초기화 (텍스트 렌더링용)
    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&g_pDWriteFactory)
    );
    if (FAILED(hr)) return hr;

    // 6. 텍스트 포맷 생성
    hr = g_pDWriteFactory->CreateTextFormat(
        L"굴림",
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

    // 렌더링 시작
    g_pRenderTarget->BeginDraw();

    // 배경 지우기
    g_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

    // 1. 사각형 그리기
    D2D1_RECT_F rectangle = D2D1::RectF(50, 50, 150, 100);

    // 채워진 사각형
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Red));
    g_pRenderTarget->FillRectangle(rectangle, g_pBrush);

    // 사각형 테두리
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
    g_pRenderTarget->DrawRectangle(rectangle, g_pBrush, 2.0f);

    // 2. 원 그리기
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(300, 100), 50, 50);

    // 채워진 원
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
    g_pRenderTarget->FillEllipse(ellipse, g_pBrush);

    // 원 테두리
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
    g_pRenderTarget->DrawEllipse(ellipse, g_pBrush, 2.0f);

    // 3. 타원 그리기
    D2D1_ELLIPSE oval = D2D1::Ellipse(D2D1::Point2F(500, 100), 80, 40);
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
    g_pRenderTarget->FillEllipse(oval, g_pBrush);

    // 4. 선 그리기
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Purple));
    g_pRenderTarget->DrawLine(
        D2D1::Point2F(50, 200),
        D2D1::Point2F(550, 250),
        g_pBrush,
        3.0f
    );

    // 5. 둥근 모서리 사각형 그리기
    D2D1_ROUNDED_RECT roundedRect = D2D1::RoundedRect(
        D2D1::RectF(50, 300, 200, 400),
        20, 20  // 둥근 모서리 반지름
    );
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Orange));
    g_pRenderTarget->FillRoundedRectangle(roundedRect, g_pBrush);

    // 6. 텍스트 그리기
    g_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
    D2D1_RECT_F textRect = D2D1::RectF(300, 300, 600, 400);
    g_pRenderTarget->DrawText(
        L"Direct2D로 그린 기본 도형들",
        wcslen(L"Direct2D로 그린 기본 도형들"),
        g_pTextFormat,
        textRect,
        g_pBrush
    );

    // 렌더링 종료
    HRESULT hr = g_pRenderTarget->EndDraw();

    // 디바이스 손실 등의 오류 처리
    if (hr == D2DERR_RECREATE_TARGET)
    {
        // RenderTarget을 재생성해야 함
        // 실제 게임에서는 이 부분을 처리해야 함
    }
}

void GameProcess::OnResize(UINT width, UINT height)
{
    if (g_pRenderTarget)
    {
        // 윈도우 크기 변경 시 RenderTarget 크기 조정
        g_pRenderTarget->Resize(D2D1::SizeU(width, height));
    }
}

void GameProcess::CleanupD2D()
{
    // 리소스 정리 (역순으로)
    if (g_pTextFormat)      g_pTextFormat->Release();
    if (g_pDWriteFactory)   g_pDWriteFactory->Release();
    if (g_pBrush)           g_pBrush->Release();
    if (g_pRenderTarget)    g_pRenderTarget->Release();
    if (g_pD2DFactory)      g_pD2DFactory->Release();
}