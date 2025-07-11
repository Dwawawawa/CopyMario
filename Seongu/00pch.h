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

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

#include <memory>						//����ũ������


//#define CW_XSIZE 1920
//#define CW_YSIZE 1080

///////////////////////////////////////////////
//! �ƴ� �̰� �ƴѵ� lib�� ������ݾ�. �����ΰ� ���Ӽ����� ���µ� ����?
//! #include "10EngineD2D.h"
//! 1. �̷��� �ϴ� ����� �־�. 
//! #include "../SS_D2DEngine/10EngineD2D.h"
//! �׷����� 
//! Project Properties �� C/C++ �� General �� Additional Include Directories
//! 2. �Ʒ� ó��? : �߰� ���� ���͸� (Include Directories)
#include "10EngineD2D.h"


#endif //PCH_H


