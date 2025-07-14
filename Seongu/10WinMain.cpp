#include "00pch.h"
#include "20GameProcess.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    std::unique_ptr<GameProcess> gp = std::make_unique<GameProcess>();

    if (!gp->Initialize(hInstance)) {
        MessageBox(nullptr, L"Initialize ����!", L"Error", MB_OK);
        return FALSE;
    }

    gp->Run();

    gp->Finalize();

    return 0;
}


//////////////////////
//! ���� ������ �ؾ� �Ҳ�
//! ������ ���� ����
//! �������� ���� �ִ� �� ���� �ȵ�