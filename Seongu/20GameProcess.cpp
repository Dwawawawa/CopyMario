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
        SS_XSIZE, SS_YSIZE,
        NULL, NULL, hInstance, NULL
    );

    if (m_hwnd == NULL) return FALSE;


    ///////////////////////////////////////////////////
    // D2D�������� �ű�� �� 
    // Direct2D �ʱ�ȭ
    //if (FAILED(InitializeD2D(m_hwnd))) return FALSE;
    // 1. �Ŵ��� ����� ���� �������� ����
    SSEngine::GetInstance()->Initialize(m_hwnd);
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
    //float FPS = m_pTimer->GetFPS();
    float _dTime = m_pTimer->DeltaTime();

    SSEngine::GetInstance()->BeginRender();
    SSEngine::GetInstance()->DrawSomething();

    
    // ��ο� �ؽ�Ʈ�� �׽�Ʈ
    //SSEngine::GetInstance()->DrawText(0, 0, L"FPS : %.0f", FPS);
 	SSEngine::GetInstance()->DrawText(0, 20, L"DeltaTime : %f", _dTime);
 	//SSEngine::GetInstance()->DrawText(300, 0, L"MousePosX : %d", InputManager::GetInstance()->GetMousePos().x);
 	//SSEngine::GetInstance()->DrawText(300, 20, L"MousePosY  : %d", InputManager::GetInstance()->GetMousePos().y);
    


    SSEngine::GetInstance()->EndRender();
}

void GameProcess::Release()
{
    ///////////////////////
    // ����� ���ص� ������ �� �����?
    // �ͱ�?
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
//! �� �츱 �ʿ䰡 �ִµ�, JJ�������� ��� ������?
//! �̰��� ������� ����!
//! �׷� �̰� �� â���̰ڳ�?
//! �����ڱ�
//! 
void GameProcess::OnResize(UINT width, UINT height)
{
    SSEngine::GetInstance()->ResizeInEngine(width, height);

}