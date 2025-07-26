#include "00pch.h"
#include "20GameProcess.h"


GameProcess::GameProcess() : m_deltaTime(0)
{

}

GameProcess::~GameProcess()
{
	Finalize();
}
 
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
    HWND m_hwnd = nullptr;

    m_hwnd = CreateWindow(
        L"Direct2DWindow",
        L"다이렉트",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        SS_XSIZE, SS_YSIZE,
        NULL, NULL, hInstance, NULL
    );

    if (m_hwnd == NULL) return FALSE;

    ///////////////////////////////////////////////////////////////////
    //! 초기화 하는 곳

    // 씬 매니저 초기화
    SceneManager::GetInstance()->Initialize();

    // 랜더러 초기화
    m_Renderer = std::make_unique<SSEngine>();
    m_Renderer->Initialize(m_hwnd);

    // 타이머 초기화
    m_pTimer = new GameTimer();
    m_pTimer->Reset();

    // this 포인터를 윈도우에 저장
    SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));


    ///////////////////////////////////////////////////////////////////
    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hwnd);

    return TRUE;

}

void GameProcess::Run()
{
    // 메시지 루프
    MSG msg = {};

    try
    {
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
    catch (...)
    {
		Finalize();
        throw;
    }
}

LRESULT GameProcess::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //////
    //WndProc에서 this 복원
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
    // 타이머 틱 (매 프레임마다 가장 먼저!)
    m_pTimer->Tick();

    //////////////////////////////////
    // 델타 타임 얻기
    m_deltaTime   = m_pTimer->DeltaTime();
    woss_m_deltaTime.str(L"");
    woss_m_deltaTime.clear();
    woss_m_deltaTime << m_deltaTime;

    
}

void GameProcess::UpdateInput()
{
    SceneManager::GetInstance()->UpdateInput();
}

void GameProcess::UpdateLogic()
{
    tempTime += m_deltaTime;  // 이 라인이 있는지 확인

    if (tempTime > 8.f) {
        
        tempint++;

        // COUNT 넘지 않게 처리
        if (tempint >= static_cast<int>(SceneType::COUNT)) {
            tempint = 0;
        }

        SceneManager::GetInstance()->ChangeScene(SceneType(tempint));
        tempTime = 0;
    }
    SceneManager::GetInstance()->Update(m_deltaTime);
}



void GameProcess::Render()
{
    if (m_Renderer == nullptr) return;

    m_Renderer->RenderBegin();

    for(int i = 0; i < 10; i++)
    {
        m_Renderer->DrawLine(0, i * 100, 1000, i * 100, D2D1::ColorF::Gray);
        m_Renderer->DrawLine( i * 100 , 0,   i * 100, 1000, D2D1::ColorF::Gray);
    }
    
    //m_Renderer->DrawCircle(500, 500, 102, D2D1::ColorF::HotPink);
    
    SceneManager::GetInstance()->Render(m_Renderer);

    m_Renderer->DrawMessage(
         2.f, 2.f, 1000.f, 1000.f, D2D1::ColorF::HotPink
    , L"델타타임: %.4f", m_deltaTime
    );

    m_Renderer->DrawMessage(
        2.f, 17.f, 1000.f, 1000.f, D2D1::ColorF::HotPink
        , L"템프타임: %.4f", tempTime
    );

    m_Renderer->DrawMessage(
        2.f, 32.f, 1000.f, 1000.f, D2D1::ColorF::AliceBlue
        , L"m_CurrentSceneType: %d", SceneManager::GetInstance()->GetCurrentSceneType()
    );

    m_Renderer->DrawMessage(
        2.f, 47.f, 1000.f, 1000.f, D2D1::ColorF::HotPink
        , L"템프인트: %d", tempint
    );

    m_Renderer->RenderEnd(true);
}


void GameProcess::Finalize()
{

	SceneManager* sceneManager = SceneManager::GetInstance();
    if (sceneManager)
    {
        sceneManager->Release();
		sceneManager->DestroyInstance();
    }


    if (m_pTimer)
    {
        delete m_pTimer;
        m_pTimer = nullptr;
    }
    
    

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
