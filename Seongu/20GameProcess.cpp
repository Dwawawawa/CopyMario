#include "00pch.h"
#include "20GameProcess.h"

///////////////////
//! 이거 되려나?
//! 되네 이 두 함수는 그냥 들고 가자. 
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
//! 교수님 코드와 달리 NzWndBase를 상속하지 않기 때문에 
//! 수정사항이 꽤나 많다. 
//! 흠... 
bool GameProcess::Initialize(HINSTANCE hInstance)
{
    const wchar_t* className = L"D2DLesson2";
    const wchar_t* windowName = L"D2DLesson2";

    // 기반 클래스의 Create 호출 (__super::Create와 동일)
    if (false == __super::Create(className, windowName, SS_XSIZE, SS_YSIZE))
    {
        return false;
    }

    m_Renderer = std::make_shared<SSEngine>();
    m_Renderer->Initialize(m_hWnd);
    
    ////////////////////////////////////
    // [ImGUI] 생략

    ID3D11Device* pd3dDevice = m_Renderer->GetD3DDevice();

    // 2) 즉시 컨텍스트 얻기
    ID3D11DeviceContext* pd3dDeviceContext = nullptr;
    pd3dDeviceContext = m_Renderer->GetD3DContext();


    // 타이머 초기화
    m_pTimer = new GameTimer();
    m_pTimer->Reset();

    return TRUE;
}

void GameProcess::Run()
{
    // 메시지 루프
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


void GameProcess::UpdateTime()
{
    // 타이머 틱 (매 프레임마다 가장 먼저!)
    m_pTimer->Tick();

    // 델타 타임 얻기
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
    if (m_Renderer == nullptr) return;

    m_Renderer->RenderBegin();

    m_Renderer->DrawCircle(100, 100, 12, D2D1::ColorF(1, 1, 1));

    m_Renderer->RenderEnd(true);
}


void GameProcess::Finalize()
{
    // [ImGUI] 생략

    if (m_Renderer != nullptr)
    {
        m_Renderer->Uninitialize();
        m_Renderer.reset();
    }
}

////////////////////////////////
//! 얜 살릴 필요가 있는데, JJ엔진에는 어디에 있을까?
//! 이겜은 리사이즈가 없음!
//! 그럼 이건 내 창작이겠네?
//! 가보자구
//! 
void GameProcess::OnResize(int width, int height)
{
    __super::OnResize(width, height);

    if (m_Renderer != nullptr) m_Renderer->Resize(width, height);


}