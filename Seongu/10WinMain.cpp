#include "00pch.h"
#include "20GameProcess.h"

#ifdef _DEBUG
#include <io.h>
#include <fcntl.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
// new �����ڸ� �������Ͽ� ���ϸ�� ���ι�ȣ ����
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ///////////////////////////////////////////////////////
    // �޸� ���� �߻� �� �� ���� ������ ����ϵ��� �Ѵ�.
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(201);

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
    ////////////////////////////////////////////////////
    // �ܼ� â �Ҵ�
    AllocConsole();


    // ǥ�� ����� �ַܼ� ���𷺼�
    FILE* pCout;
    freopen_s(&pCout, "CONOUT$", "w", stdout);

    FILE* pCin;
    freopen_s(&pCin, "CONIN$", "r", stdin);

    FILE* pCerr;
    freopen_s(&pCerr, "CONOUT$", "w", stderr);

#endif

    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    std::unique_ptr<GameProcess> gp = std::make_unique<GameProcess>();

    if (!gp->Initialize(hInstance)) {
        MessageBox(nullptr, L"Initialize ����!", L"Error", MB_OK);
        return FALSE;
    }

    gp->Run();

    gp->Finalize();

#ifdef _DEBUG
    // �ܼ� ����
    FreeConsole();

    // ���� Ž��
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}
