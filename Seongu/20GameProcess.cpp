#include "00pch.h"
#include "20GameProcess.h"

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
    const wchar_t* className = L"D2DLesson2";
    const wchar_t* windowName = L"D2DLesson2";

    // ��� Ŭ������ Create ȣ�� (__super::Create�� ����)
    if (false == __super::Create(className, windowName, SS_XSIZE, SS_YSIZE))
    {
        return false;
    }

    m_Renderer = std::make_shared<SSEngine>();
    m_Renderer->Initialize(m_hWnd);
    
    ////////////////////////////////////
    // [ImGUI] ����

    ID3D11Device* pd3dDevice = m_Renderer->GetD3DDevice();

    // 2) ��� ���ؽ�Ʈ ���
    ID3D11DeviceContext* pd3dDeviceContext = nullptr;
    pd3dDeviceContext = m_Renderer->GetD3DContext();


    // Ÿ�̸� �ʱ�ȭ
    m_pTimer = new GameTimer();
    m_pTimer->Reset();

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
    if (m_Renderer == nullptr) return;

    m_Renderer->RenderBegin();

    m_Renderer->DrawCircle(100, 100, 12, D2D1::ColorF(1, 1, 1));

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

////////////////////////////////
//! �� �츱 �ʿ䰡 �ִµ�, JJ�������� ��� ������?
//! �̰��� ������� ����!
//! �׷� �̰� �� â���̰ڳ�?
//! �����ڱ�
//! 
void GameProcess::OnResize(int width, int height)
{
    __super::OnResize(width, height);

    if (m_Renderer != nullptr) m_Renderer->Resize(width, height);


}