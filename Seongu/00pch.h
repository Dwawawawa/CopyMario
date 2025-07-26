// 00pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.


///////////////////////
//! 성우
//! 대성우
//! 빅 보이스 액터


#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>

#include <iostream>
#include <sstream>						// float를 wstring으로 

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>				


#define SS_XSIZE 1280
#define SS_YSIZE 720

///////////////////////////////////////////////
//! 
//! 공부 기록 : 추가 포함 디렉터리 이용하기
//! 
//! 아니 이건 아닌데 lib로 만들었잖아. 정적인거 종속성으로 쓰는데 굳이?
//! #include "10EngineD2D.h"
//! 1. 이렇게 하는 방법이 있어. -> 짜침
//! #include "../SS_D2DEngine/10EngineD2D.h"
//! 
//! 2. 아래 처럼? : 추가 포함 디렉터리 (Include Directories)
//! Project Properties → C/C++ → General → Additional Include Directories
//! 
//! --------------------------
//! 1. #pragma comment(lib, "SS_D2DEngine.lib") 
//! 정확한 위치에다가 만들어야 함
//! #pragma comment(lib, "x64/Debug/SS_D2DEngine.lib") 
//! 
//! 2. 그렇다면 릴리즈는 어떻게 해야 할까?
//! 내가 문제였네. pch.h를 잘 못 사용했었어. 
//! 
//! 3. 파일별 PCH 설정:
//! 00pch.cpp: 우클릭 → 속성 → C / C++ → 미리 컴파일된 헤더 → "만들기(/Yc)"
//! 10winmain.cpp : 우클릭 → 속성 → C / C++ → 미리 컴파일된 헤더 → "사용(/Yu)"
//! 다른 모든.cpp 파일 : "사용(/Yu)"
//! 
#include "10EngineD2D.h"
#include "20GameTimer.h"
#include "30GameObject.h" // GameObject 클래스의 정의를 포함하는 헤더 파일 추가
#include "SceneManager.h"

#include "01Debug.h"

#ifdef _DEBUG
#pragma comment(lib, "x64/Debug/SS_D2DEngine.lib") 
#else
#pragma comment(lib, "x64/Release/SS_D2DEngine.lib") 
#endif

#endif //PCH_H


