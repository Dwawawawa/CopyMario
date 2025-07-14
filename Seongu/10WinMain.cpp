#include "00pch.h"
#include "20GameProcess.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    std::unique_ptr<GameProcess> gp = std::make_unique<GameProcess>();

    if (!gp->Initialize(hInstance)) {
        MessageBox(nullptr, L"Initialize 실패!", L"Error", MB_OK);
        return FALSE;
    }

    gp->Run();

    gp->Finalize();

    return 0;
}


//////////////////////
//! 이제 다음에 해야 할껀
//! 엔진을 빼는 거임
//! 전역으로 박혀 있는 게 말이 안돼