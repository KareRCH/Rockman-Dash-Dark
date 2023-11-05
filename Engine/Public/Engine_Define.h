#ifndef Engine_Define_h__
#define Engine_Define_h__

#pragma warning(disable : 26495)
#pragma warning(disable : 4251)


// min max 매크로는 호환성 문제로 사용되지 않습니다. ( assimp )
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

// Assimp 라이브러리
#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


// 다이렉트X 11 라이브러리
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>


// 사운드 라이브러리
#include <fmod.h>
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod_studio.h>
#include <fmod_studio.hpp>

#include <io.h>				// 윈도우 제공
#include <fmod_common.h>



// RapidJSON 라이브러리
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>




//----------------STD---------------------

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// STD 라이브러리
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



// 공용 헤더
#include "Engine_Macro.h"
#include "Engine_Const.h"
#include "Engine_Enum.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"



// 다이렉트 인풋
#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>



// ImGui관련 new 디버그 코드 비활성화
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
extern _bool		g_bLockEsc;		// 비정상 종료를 막기위한 플래그
extern _bool		g_bFullScreen;

#endif // Engine_Define_h__
