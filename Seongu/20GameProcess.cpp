#include "00pch.h"
#include "20GameProcess.h"

GameProcess::GameProcess()
    :m_pTimer(nullptr), m_hwnd(nullptr)
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
        SS_XSIZE, SS_YSIZE,
        NULL, NULL, hInstance, NULL
    );

    if (m_hwnd == NULL) return FALSE;


    ///////////////////////////////////////////////////
    // D2D엔진으로 옮기는 중 
    // Direct2D 초기화
    //if (FAILED(InitializeD2D(m_hwnd))) return FALSE;
    // 1. 매니저 만들기 전에 엔진직접 접근
    SSEngine::GetInstance()->Initialize(m_hwnd);
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
    //float FPS = m_pTimer->GetFPS();
    float _dTime = m_pTimer->DeltaTime();

    SSEngine::GetInstance()->BeginRender();
    SSEngine::GetInstance()->DrawSomething();

    
    // 드로우 텍스트로 테스트
    //SSEngine::GetInstance()->DrawText(0, 0, L"FPS : %.0f", FPS);
 	SSEngine::GetInstance()->DrawText(0, 20, L"DeltaTime : %f", _dTime);
 	//SSEngine::GetInstance()->DrawText(300, 0, L"MousePosX : %d", InputManager::GetInstance()->GetMousePos().x);
 	//SSEngine::GetInstance()->DrawText(300, 20, L"MousePosY  : %d", InputManager::GetInstance()->GetMousePos().y);
    


    SSEngine::GetInstance()->EndRender();
}

void GameProcess::Release()
{
    ///////////////////////
    // 릴리즈를 안해도 누수가 안 생길까?
    // 왤까?
    //
    if (m_pTimer != nullptr)
    {
        delete m_pTimer;
        m_pTimer = nullptr;
    }

    SSEngine::GetInstance()->Release();
}

LRESULT CALLBACK GameProcess::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        GameProcess::OnResize(width, height);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

////////////////////////////////
//! 얜 살릴 필요가 있는데, JJ엔진에는 어디에 있을까?
//! 이겜은 리사이즈가 없음!
//! 그럼 이건 내 창작이겠네?
//! 가보자구
//! 
void GameProcess::OnResize(UINT width, UINT height)
{
    SSEngine::GetInstance()->ResizeInEngine(width, height);

}