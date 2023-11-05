#ifndef Engine_Define_h__
#define Engine_Define_h__

#pragma warning(disable : 26495)
#pragma warning(disable : 4251)


// min max ��ũ�δ� ȣȯ�� ������ ������ �ʽ��ϴ�. ( assimp )
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

// Assimp ���̺귯��
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


// ���̷�ƮX 11 ���̺귯��
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>


// ���� ���̺귯��
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_studio.h>
#include <fmod_studio.hpp>

#include <io.h>				// ������ ����
#include <fmod_common.h>



// RapidJSON ���̺귯��
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>




//----------------STD---------------------

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// STD ���̺귯��
#include <array>
#include <vector>
#include <list>
#include <queue>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <tuple>
#include <algorithm>
#include <functional>
#include <string>

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>



// ���� ���
#include "Engine_Macro.h"
#include "Engine_Const.h"
#include "Engine_Enum.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"



// ���̷�Ʈ ��ǲ
#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>



// ImGui���� new ����� �ڵ� ��Ȱ��ȭ
#ifndef __IMGUI
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif



#else
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#endif

using namespace std;
using namespace DirectX;
using namespace Engine;

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
extern _bool		g_bLockEsc;		// ������ ���Ḧ �������� �÷���
extern _bool		g_bFullScreen;

#endif // Engine_Define_h__
