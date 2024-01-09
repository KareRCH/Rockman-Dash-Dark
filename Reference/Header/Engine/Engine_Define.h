#ifndef Engine_Define_h__
#define Engine_Define_h__

#pragma warning(disable : 26495)
#pragma warning(disable : 4251)
#pragma warning(disable : 4244)


// min max ��ũ�δ� ȣȯ�� ������ ������ �ʽ��ϴ�. ( assimp )
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

// ȣȯ�� �����Ƽ� �Ƚ�!
//#include "rttr/rttr_enable.h"
//#include "rttr/type.h"
//#include "rttr/registration.h"
//#include "rttr/registration_friend.h"

// Assimp ���̺귯��

//#include <assimp/Importer.hpp>
//#include <assimp/cimport.h>
//#include <assimp/postprocess.h>
//#include <assimp/scene.h>
#define AI_LMW_MAX_WEIGHTS 4
#include <assimp/config.h>
#include <assimp/material.h>


// ���̷�ƮX 11 ���̺귯��
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <wrl/client.h>

#include <d3dx11effect.h>


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


// ��� ���̺귯��
#include <lua.hpp>



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

// rttr���� friend ���� ���
//#define DOXYGEN

// min max ��ũ�δ� ȣȯ�� ������ ������ �ʽ��ϴ�. ( assimp )
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
extern _bool		g_bLockEsc;		// ������ ���Ḧ �������� �÷���
extern _bool		g_bFullScreen;

extern _bool		g_bResizeOnScreen;	// �ػ� ��ũ���� ���߱�
extern _uint		g_iResizeWidth;		// �ػ� ����� ���Ǵ� ����
extern _uint		g_iResizeHeight;	// �ػ� ����� ���Ǵ� ����

#endif // Engine_Define_h__
