#include "00pch.h"
#include "20GameProcess.h"


GameProcess::GameProcess()
    :m_deltaTime(0), m_deltaTimeMS(0), m_totalTime(0)
{
}

GameProcess::~GameProcess()
{
}

///////////////////
//! �̰� �Ƿ���?
//! �ǳ� �� �� �Լ��� �׳� ��� ����. 
std::wstring ConvertToWString(const std::string& str)
{
    size_t len = 0;
    mbstowcs_s(&len, nullptr, 0, str.c_str(), _TRUNCATE);
    if (len == 0)
        return L"";

    std::wstring wstr(len, L'\0');
    mbstowcs_s(&len, &wstr[0], len, str.c_str(), _TRUNCATE);
    wstr.resize(len - 1); // Remove the null terminator added by mbstowcs_s  
    return wstr;
}

std::string WStringToString(const std::wstring& wstr)
{
    size_t len = 0;
    wcstombs_s(&len, nullptr, 0, wstr.c_str(), _TRUNCATE);
    if (len == 0)
        return "";
    std::string str(len, '\0');
    wcstombs_s(&len, &str[0], len, wstr.c_str(), _TRUNCATE);
    str.resize(len - 1); // Remove the null terminator added by wcstombs_s
    return str;
}


////////////////////////////////
//! ������ �ڵ�� �޸� NzWndBase�� ������� �ʱ� ������ 
//! ���������� �ϳ� ����. 
//! ��... 
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
    HWND m_hwnd = nullptr;

    m_hwnd = CreateWindow(
        L"Direct2DWindow",
        L"���̷�Ʈ",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        SS_XSIZE, SS_YSIZE,
        NULL, NULL, hInstance, NULL
    );

    if (m_hwnd == NULL) return FALSE;

    // ������ �ʱ�ȭ
    m_Renderer = std::make_unique <SSEngine>();
    m_Renderer->Initialize(m_hwnd);

    // Ÿ�̸� �ʱ�ȭ
    m_pTimer = new GameTimer();
    m_pTimer->Reset();

    // this �����͸� �����쿡 ����
    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hwnd);

    return TRUE;

}

void GameProcess::Run()
{
    // �޽��� ����
    MSG msg = {};

    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
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

LRESULT GameProcess::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //////
    //WndProc���� this ����
    GameProcess* pThis = reinterpret_cast<GameProcess*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));


    switch (uMsg)
    {
    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        pThis->GameProcess::OnResize(width, height);
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




void GameProcess::UpdateTime()
{
    // Ÿ�̸� ƽ (�� �����Ӹ��� ���� ����!)
    m_pTimer->Tick();

    // ��Ÿ Ÿ�� ���
    m_deltaTime   = m_pTimer->DeltaTime();
    m_deltaTimeMS = m_pTimer->DeltaTimeMS();
    m_totalTime   = m_pTimer->TotalTime();


    woss_m_deltaTime   . str(L"");
    woss_m_deltaTimeMS . str(L"");
    woss_m_totalTime   . str(L"");


    woss_m_deltaTime.clear();
    woss_m_deltaTimeMS.clear();
    woss_m_totalTime.clear();

    woss_m_deltaTime   << m_deltaTime;
    woss_m_deltaTimeMS << m_deltaTimeMS;
    woss_m_totalTime   << m_totalTime;
}

void GameProcess::UpdateInput()
{

}

void GameProcess::UpdateLogic()
{

}

void GameProcess::Render()
{
    if (m_Renderer == nullptr) return;

    m_Renderer->RenderBegin();

    m_Renderer->DrawCircle(100, 100, 102, D2D1::ColorF::AliceBlue);
    
    
    m_Renderer->DrawMessage((woss_m_totalTime.str() + std::wstring(L" dads")).c_str()
        , 2.f, 2.f, 1000.f, 1000.f, D2D1::ColorF::AliceBlue);

    m_Renderer->RenderEnd(true);
}


void GameProcess::Finalize()
{
    // [ImGUI] ����

    if (m_Renderer != nullptr)
    {
        m_Renderer->Uninitialize();
        m_Renderer.reset();
    }
}


void GameProcess::OnResize(int width, int height)
{
    m_Renderer->Resize(width, height);
}
