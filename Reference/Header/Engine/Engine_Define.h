#ifndef Engine_Define_h__
#define Engine_Define_h__

#pragma warning(disable : 26495)
#pragma warning(disable : 4251)
#pragma warning(disable : 4244)


// min max 매크로는 호환성 문제로 사용되지 않습니다. ( assimp )
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

// 호환성 안좋아서 안써!
//#include "rttr/rttr_enable.h"
//#include "rttr/type.h"
//#include "rttr/registration.h"
//#include "rttr/registration_friend.h"

// Assimp 라이브러리

//#include <assimp/Importer.hpp>
//#include <assimp/cimport.h>
//#include <assimp/postprocess.h>
//#include <assimp/scene.h>
#define AI_LMW_MAX_WEIGHTS 4
#include <assimp/config.h>
#include <assimp/material.h>


// 다이렉트X 11 라이브러리
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <wrl/client.h>

#include <d3dx11effect.h>


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


// 루아 라이브러리
#include <lua.hpp>



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

#include "d3dx11effect.h"
#include "DirectXCollision.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"
#include "ScreenGrab.h"
#include "VertexTypes.h"
#include "PrimitiveBatch.h"
#include "Effects.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"


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

// rttr에서 friend 쓸때 사용
//#define DOXYGEN

// min max 매크로는 호환성 문제로 사용되지 않습니다. ( assimp )
#ifdef min
#undef min
#endif
#ifdef max
#undef max
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
using namespace Microsoft::WRL;
using namespace Engine;

extern HWND			g_hWnd;
extern HINSTANCE	g_hInst;
extern _bool		g_bLockEsc;		// 비정상 종료를 막기위한 플래그
extern _bool		g_bFullScreen;

extern _bool		g_bResizeOnScreen;	// 해상도 스크린에 맞추기
extern _uint		g_iResizeWidth;		// 해상도 변경시 사용되는 변수
extern _uint		g_iResizeHeight;	// 해상도 변경시 사용되는 변수

#endif // Engine_Define_h__
