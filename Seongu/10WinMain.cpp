#include "00pch.h"
#include "20GameProcess.h"

#ifdef _DEBUG
#include <io.h>
#include <fcntl.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
// new 연산자를 재정의하여 파일명과 라인번호 추적
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    ///////////////////////////////////////////////////////
    // 메모리 누수 발생 시 그 지점 정보를 출력하도록 한다.
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(201);

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
    ////////////////////////////////////////////////////
    // 콘솔 창 할당
    AllocConsole();


    // 표준 출력을 콘솔로 리디렉션
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
        MessageBox(nullptr, L"Initialize 실패!", L"Error", MB_OK);
        return FALSE;
    }

    gp->Run();

    gp->Finalize();

#ifdef _DEBUG
    // 콘솔 해제
    FreeConsole();

    // 누수 탐지
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}
