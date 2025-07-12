#include "00pch.h"
#include "20GameProcess.h"

GameProcess::GameProcess()
    :m_pTimer(nullptr), m_hwnd(nullptr)
    ,m_deltaTime(0),m_deltaTimeMS(0),m_totalTime(0)
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



    CreateManager();


    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hwnd);

    return TRUE;
}

void GameProcess::Run()
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
            UpdateTime();
            UpdateInput();
            UpdateLogic();
            Render();
        }
    }
}


void GameProcess::UpdateTime()
{
    // Ÿ�̸� ƽ (�� �����Ӹ��� ���� ����!)
    m_pTimer->Tick();

    // ��Ÿ Ÿ�� ���
    m_deltaTime = m_pTimer->DeltaTime();
    m_deltaTimeMS = m_pTimer->DeltaTimeMS();
    m_totalTime = m_pTimer->TotalTime();
}

void GameProcess::UpdateInput()
{
}

void GameProcess::UpdateLogic()
{
}

void GameProcess::Render()
{

    SSEngine::GetInstance()->BeginRender();
    SSEngine::GetInstance()->DrawSomething();


    if(1)
    {
        SSEngine::GetInstance()->DrawText(0, 0, L"TotalTime : %.2f sec", m_totalTime);
        SSEngine::GetInstance()->DrawText(0, 20, L"DeltaTime : %4f sec", m_deltaTime);
        SSEngine::GetInstance()->DrawText(0, 40, L"DeltaTimeMS : %.2f ms", m_deltaTimeMS);
        SSEngine::GetInstance()->DrawText(0, 60, L"FPS : %.0f", 1.0f / m_deltaTime);
    }

    SSEngine::GetInstance()->EndRender();
}

void GameProcess::CreateManager()
{
    // ���� ����
    SSEngine::GetInstance()->Initialize(m_hwnd);

    // Ÿ�̸� ����
    m_pTimer = new GameTimer();
    m_pTimer->Reset();


    // ��ǲ, ���� ���⼭ ����� ��
}

void GameProcess::Release()
{

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