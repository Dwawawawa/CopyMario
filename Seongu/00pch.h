// 00pch.h: �̸� �����ϵ� ��� �����Դϴ�.
// �Ʒ� ������ ������ �� ���� �����ϵǾ�����, ���� ���忡 ���� ���� ������ ����մϴ�.
// �ڵ� ������ �� ���� �ڵ� �˻� ����� �����Ͽ� IntelliSense ���ɿ��� ������ ��Ĩ�ϴ�.
// �׷��� ���⿡ ������ ������ ���� �� ������Ʈ�Ǵ� ��� ��� �ٽ� �����ϵ˴ϴ�.
// ���⿡ ���� ������Ʈ�� ������ �߰����� ������. �׷��� ������ ���ϵ˴ϴ�.


///////////////////////
//! ����
//! �뼺��
//! �� ���̽� ����


#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>

#include <iostream>
#include <sstream>						// float�� wstring���� 

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
//! ���� ��� : �߰� ���� ���͸� �̿��ϱ�
//! 
//! �ƴ� �̰� �ƴѵ� lib�� ������ݾ�. �����ΰ� ���Ӽ����� ���µ� ����?
//! #include "10EngineD2D.h"
//! 1. �̷��� �ϴ� ����� �־�. -> ¥ħ
//! #include "../SS_D2DEngine/10EngineD2D.h"
//! 
//! 2. �Ʒ� ó��? : �߰� ���� ���͸� (Include Directories)
//! Project Properties �� C/C++ �� General �� Additional Include Directories
//! 
//! --------------------------
//! 1. #pragma comment(lib, "SS_D2DEngine.lib") 
//! ��Ȯ�� ��ġ���ٰ� ������ ��
//! #pragma comment(lib, "x64/Debug/SS_D2DEngine.lib") 
//! 
//! 2. �׷��ٸ� ������� ��� �ؾ� �ұ�?
//! ���� ��������. pch.h�� �� �� ����߾���. 
//! 
//! 3. ���Ϻ� PCH ����:
//! 00pch.cpp: ��Ŭ�� �� �Ӽ� �� C / C++ �� �̸� �����ϵ� ��� �� "�����(/Yc)"
//! 10winmain.cpp : ��Ŭ�� �� �Ӽ� �� C / C++ �� �̸� �����ϵ� ��� �� "���(/Yu)"
//! �ٸ� ���.cpp ���� : "���(/Yu)"
//! 
#include "10EngineD2D.h"
#include "20GameTimer.h"
#include "30GameObject.h" // GameObject Ŭ������ ���Ǹ� �����ϴ� ��� ���� �߰�
#include "SceneManager.h"

#include "01Debug.h"

#ifdef _DEBUG
#pragma comment(lib, "x64/Debug/SS_D2DEngine.lib") 
#else
#pragma comment(lib, "x64/Release/SS_D2DEngine.lib") 
#endif

#endif //PCH_H


