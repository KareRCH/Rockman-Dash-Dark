#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"
#include <wrl/client.h>

using namespace Microsoft::WRL;

using namespace std;
class unorded_map;


namespace Engine
{

	typedef struct tagVertexColor
	{
		_float3		vPosition;		
		_float4		vColor;

	}VTXCOL;
	//const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	struct MATRIX_BUFFER_T
	{
		_matrix		matWorld;
		_matrix		matView;
		_matrix		matProj;

		static constexpr D3D11_BUFFER_DESC BufferDesc = {
			64U * 3U, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0
		};
	};

	struct LIGHT_BUFFER_T
	{
		_float4		vAmbientColor;		// �ֺ���
		_float4		vDiffuseColor;		// ���ݻ�
		_float3		vLightDirection;
		_float		fSpecularPower;
		_float4		vSpecularColor;		// ���ݻ�

		static constexpr D3D11_BUFFER_DESC BufferDesc = {
			64U, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0
		};
	};

	struct CAMERA_BUFFER_T
	{
		_float3		vPosition;
		_float		fPadding;

		static constexpr D3D11_BUFFER_DESC BufferDesc = {
			16U, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0
		};
	};

	struct BONE_COMMON_BUFFER_T
	{
		_matrix		matTransform[128];

		static constexpr D3D11_BUFFER_DESC BufferDesc = {
			64U * 128U, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0
		};
	};

	// ���� ��ġ���� ������ ���̴�����ü
	struct SHADER_VTX_SINGLE
	{
		_float3		vPosition;

		static constexpr _uint iNumElements = 1;
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	//
	struct SHADER_VTX_TEXCOORD
	{
		_float3		vPosition;
		_float2		vTexCoord;

		static constexpr _uint iNumElements = 2;
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	};

	struct SHADER_VTX_CUBETEX
	{
		_float3		vPosition;
		_float3		vTexCoord;

		static constexpr _uint iNumElements = 2;
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	};

	struct SHADER_VTX_COMMODEL
	{
		_float3		vPosition;
		_float3		vNormal;
		_float2		vTexCoord;

		static constexpr _uint iNumElements = 3;
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	};

	struct SHADER_VTX_NORM
	{
		_float3		vPosition;
		_float3		vNormal;
		_float2		vTexcoord;

		static constexpr unsigned int	iNumElements = 3;
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	struct SHADER_VTX_MODEL
	{
		_float3		vPosition;
		_float3		vNormal;
		_float2		vTexcoord;
		_float3		vTangent;

		static constexpr unsigned int	iNumElements = 4;
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	struct SHADER_VTX_SKINMODEL
	{
		_float3		vPosition;
		_float3		vNormal;
		_float2		vTexCoord;
		_float3		vTangent;	
		_int4		vBoneID;
		_float4		vWeight;

		static constexpr _uint iNumElements = 6;
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BONEID", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
	};

	struct SHADER_VTX_PARTICLE_POINT
	{
		_float3		vPosition;
		_float3		vNormal;
		_float2		vTexCoord;
		_float3		vTangent;
		_int4		vBoneID;
		_float4		vWeight;

		static const _uint iNumElements = 7;
		static constexpr D3D11_INPUT_ELEMENT_DESC Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PSIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};
	};

	struct SAMPLER_COMMON_DESC
	{
		static constexpr D3D11_SAMPLER_DESC Desc = {
			D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
			0.f, 1, D3D11_COMPARISON_NEVER, { 0, 0, 0, 0 }, 0, D3D11_FLOAT32_MAX
		};
	};

	struct VTXPOINT
	{
		_float3 vPosition;
		_float2 vPSize;

		static const unsigned int					iNumElements = 2;
		static constexpr D3D11_INPUT_ELEMENT_DESC	Elements[iNumElements] = {
			
		};
	};

	struct VTX_PARTICLE_POINT
	{
		static const unsigned int						iNumElements = 7;
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PSIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		};
	};

	typedef struct
	{
		_float4		vRight, vUp, vLook, vPosition;
		_float4		vColor;
	}VTXINSTANCE;

	struct DX11DEVICE_T
	{
		ComPtr<ID3D11Device> pDevice;
		ComPtr<ID3D11DeviceContext> pDeviceContext;
	};

	struct TLIGHT_DESC
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };
		TYPE			eType;
		_float4			vDirection;
		_float4			vPosition;

		float			fRange;
		float			fPower;
		float			fDamping;

		_float4		vDiffuse;
		_float4		vAmbient;
		_float4		vSpecular;
	};

	//typedef struct tagVertexTexture
	//{
	//	_float3		vPosition;
	//	_float3		vNormal;
	//	_float2		vTexUV;

	//}VTXTEX;
	//const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	//typedef struct tagVertexCubeTexture
	//{
	//	_float3		vPosition;
	//	_float3		vTexUV;

	//}VTXCUBE;

	//const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0); // �ؽ�ó�� UV ��ǥ ���� FLOAT�� 3���� ǥ���ϰڴٴ� ��ũ��(��ȣ���� ���� 0�� �ǹ̴� ���� ���ؽ��� �ؽ��� UV���� �������� �� �� �ִµ� ���� 0��° ���� �����ϰڴٴ� �ǹ�)

	//typedef struct tagIndex16
	//{
	//	_ushort  _0;
	//	_ushort  _1;
	//	_ushort  _2;

	//}INDEX16;

	//typedef struct tagIndex32
	//{
	//	_ulong	_0;
	//	_ulong	_1;
	//	_ulong	_2;

	//}INDEX32;


	//typedef struct tagParticle
	//{
	//	D3DXVECTOR3 vPosition;
	//	D3DCOLOR    Color;
	//}Particle;

	//const _ulong	FVF_Particle = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	//typedef struct tagAttribute
	//{
	//	tagAttribute() // ������ 
	//	{
	//		fLifeTime = 0.0f;
	//		fAge = 0.0f;
	//		bIsAlive = true;
	//	}
	//	_float3		vPosition; // ���� ��ġ�� ����
	//	_float3		vVelocity; // ���� �ӵ��� ����
	//	_float3		vAcceleration; // ���� ���ӵ��� ���� 
	//	_float      fLifeTime;     // ��ü�� ����ִµ����� ���� - �󸶳� ���� �����Ұ��ΰ� 
	//	_float      fAge;          // ��ü�� ���� ���� - �󸶳� ���� �����ߴ°� 
	//	D3DXCOLOR   Color;        // ���� ���� - � �������� ������ �Ǿ�� �ϴ°� 
	//	D3DXCOLOR   ColorFade;    // �ð��� ���� ������ ��� ���ؾ� �ϴ°�
	//	_bool       bIsAlive; // ��ü�� ����ִ��� ���� - Ȱ���������� �ƴϸ� �Ҹ��ߴ��� 

	//}Attribute;

	//typedef struct Frame
	//{
	//	//Frame
	//	_float		fFrame = 0.f;
	//	_float		fFrameEnd = 0.f;
	//	_float		fFrameSpeed = 0.f;

	//	//Repeat
	//	_float		fRepeat = 0;

	//	//LifeTime
	//	_float		fAge = 0.f;
	//	_float		fLifeTime = 0.f;

	//}FRAME;

	//typedef struct tagBoundingBox
	//{
	//	_float3 vMin;
	//	_float3 vMax;
	//}BoundingBox;


#pragma region ���� ���¸ӽ�
	// ���� �ƹ����� �� �� �ִ� ���¸ӽ� ����ü
	template <typename T>
	struct tagState
	{
		tagState() : eState(T()), ePrevState(T()), eNextState(T())
		{
			bIsEnter = false;
			bIsExit = false;
			bIsReserved = false;
		}
		~tagState() {}

		bool	bIsEnter;	// ����
		bool	bIsExit;	// Ż��
		bool	bIsReserved;// ����
		T		eState;		// ���� ����
		T		ePrevState; // ���� ����
		T		eNextState; // ���� ����

#pragma region ���¸ӽ� �Լ�
		
		// ���� ���¸� �ٲٴ� �Լ� (Ż�� �ڵ�� ���� �ڵ带 �۵� ��Ų��.)
		void Set_State(T _eState)
		{
			if (bIsExit)
				return;

			ePrevState = eState;
			eState = _eState;
			bIsExit = true;
			bIsEnter = true;
		}

		// ���� ��ȯ ���� �Լ�
		void Reserve_State(T _eState)
		{
			eNextState = _eState;
			bIsReserved = true;
		}

		// ������ ��
		bool IsState_Entered()
		{
			if (bIsEnter && !bIsReserved)
			{
				bIsEnter = false;
				bIsExit = false;		// ���� ���Խ� Ż�� ���� �ڵ� ��Ȱ��ȭ
				return true;
			}
			return false;
		}

		// �������� ��
		bool IsState_Exit()
		{
			// ���� ���¿����� ���� ���¸� Set_State �Ѵ�.
			if (bIsReserved)
			{
				Set_State(eNextState);
				bIsReserved = false;
				bIsExit = false;
				return true;
			}

			// �̹� Entered �Լ��� �ҷ����� �� Ż�� ������ OFF ��Ų��.
			if (!bIsEnter)
				bIsExit = false;

			if (bIsExit)
			{
				bIsExit = false;
				return true;
			}
			return false;
		}

		// ������ ���� ���¿����� ������Ʈ�� �� �� �ִ�.
		bool Can_Update()
		{
			return !bIsReserved;
		}

		// ���� ���°� �Ű������� �´��� üũ
		bool IsOnState(T _eState)
		{
			return (eState == _eState);
		}
#pragma endregion
	};

	// ���ǻ��� : Ű���� ���ڿ��� �ҰŸ� ��Ʈ������ ����ؾ� �Ѵ�.

	template <typename T>
	using STATE_INFO = tagState<T>;

	// �ܼ��ϰ� ���� ���� ���ø�
	template <typename Key, typename Func>
	using MAP_FUNC = unordered_map<Key, function<Func>>;

	// ���¸ӽ� ��Ʈ
	template<typename Key, typename Func>
	struct STATE_SET
	{
		STATE_SET() {}
		~STATE_SET() {}

	public:
		STATE_INFO<Key> tState;
		MAP_FUNC<Key, Func> mapFunc;

	public:
#pragma region ���¸ӽ� �Լ�
		void Set_State(Key _eState)
		{
			tState.Set_State(_eState);
		}

		// + ���� �߰� : ���� ���°��� ��ȯ
		Key Get_State()
		{
			return tState.eState;
		}

		void Reserve_State(Key _eState)
		{
			tState.Reserve_State();
		}

		// ������ ��
		bool IsState_Entered()
		{
			return tState.IsState_Entered();
		}

		// �������� ��
		bool IsState_Exit()
		{
			return tState.IsState_Exit();
		}

		// ������ ���� ���¿����� ������Ʈ�� �� �� �ִ�.
		bool Can_Update()
		{
			return tState.Can_Update();
		}

		bool IsOnState(Key _eState)
		{
			return tState.IsOnState(_eState);
		}
#pragma endregion
#pragma region �Լ� ��
		void Add_Func(Key eState, function<Func>&& fn)
		{
			mapFunc.emplace(eState, fn);
		}

		function<Func> Get_StateFunc()
		{
			return mapFunc[tState.eState];
		}
#pragma endregion

	};
#pragma endregion

#pragma region ������
	// ������ �뵵�� ���� ����ü
	template<typename T = float>
	struct _DELAY
	{
		static_assert(std::is_arithmetic<T>::value, "T�� ���� Ÿ���̾�߸� �մϴ�.");

	public:
		T Max, Cur;
	private:
		T PrevCur;


	public:
		_DELAY() : Max(T()), Cur(T()), PrevCur(Cur) {}
		_DELAY(T _Max, bool bMax = false) : Max(_Max), Cur(T(T(bMax)* T(_Max))), PrevCur(Cur) {}
		~_DELAY() {}

		// �� ������Ʈ �� �ƽ��� ���޽� ��ȯ
		bool Update(T increase, bool bAutoReset = false)
		{
			PrevCur = Cur;
			Cur += increase;
			if (Cur >= Max)
			{
				if (bAutoReset)
					Cur = T();
				else
					Cur = Max;
				return true;
			}

			return false;
		}

		// Ư�� ����Ʈ(����)�� �Ѿ�� ��ȯ
		bool Update(T increase, T point, bool bAutoReset = false)
		{
			PrevCur = Cur;
			Cur += increase;
			if (Cur >= point)
			{
				if (bAutoReset)
					Cur = T();
				else
					Cur = Max;
				return true;
			}

			return false;
		}

		// ���簪 �ʱ�ȭ
		void Reset()
		{
			Cur = T();
		}

		// Max �� �缳�� �� ���簪 �ʱ�ȭ
		void ReAdjust(T max)
		{
			Max = max;
			Cur = T();
		}

		// Ư�� ����Ʈ(����)�� �Ѿ�� ��� Ʈ��
		bool IsReach(T point)
		{
			return (Cur >= point);
		}

		// �������� üũ�ϰ��� �ϴ� ������ �ѹ��� ������ ���� üũ�մϴ�.
		bool IsReach_Once(T point, T increase)
		{
			return (Cur >= point - increase * (T)0.5f && Cur < point + increase * (T)0.5f);
		}

		// �ƽ��� ��� ��� Ʈ��
		bool IsMax()
		{
			return (Cur >= Max);
		}

		// �ƽ��� ��� �ѹ��� Ʈ��
		bool IsMax_Once()
		{
			return (Cur >= Max && PrevCur != Cur);
		}

		// �ۼ�Ʈ �� ��ȯ
		float Get_Percent()
		{
			return (static_cast<float>(Cur) / static_cast<float>(Max));
		}
	};

	template <typename T = float>
	using DELAY = _DELAY<T>;

	template <typename T = float>
	using GAUGE = _DELAY<T>;
#pragma endregion

#pragma region �׼�
	typedef struct _ACTION
	{
		_ACTION() : bAction() {}
		~_ACTION() {}

		bool bAction;

		/// <summary>
		/// �� ������ �������� ���� ��
		/// </summary>
		void Act()
		{
			bAction = true;
		}

		/// <summary>
		/// �� ������ �������� ���� ��
		/// </summary>
		void Update()
		{
			bAction = false;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool Sync()
		{
			if (bAction)
			{
				bAction = false;
				return true;
			}
			return false;
		}

		bool IsAct()
		{
			return bAction;
		}
	}ACTION;

	template<typename Key>
	using MAP_ACTION = unordered_map<Key, ACTION>;

	template<typename Key>
	class CMapAction_Updator
	{
	public:
		void operator() (pair<const Key, ACTION>& Action)
		{
			Action.second.Update();
		}
	};

	template<typename Key>
	struct ACTION_SET
	{
		ACTION_SET() {}
		~ACTION_SET() {}

	public:
		map<Key, ACTION> mapAction;

		void Add_Action(Key&& tKey)
		{
			mapAction.emplace(tKey, ACTION());
		}

		ACTION& operator[] (Key&& tKey)
		{
			return mapAction[tKey];
		}

		void Act(Key&& tKey)
		{
			mapAction[tKey].Act();
		}

		_bool IsAct(Key&& tKey)
		{
			return mapAction[tKey].IsAct();
		}

		void Reset()
		{
			for_each(mapAction.begin(), mapAction.end(), CMapAction_Updator<Key>());
		}
	};
#pragma endregion

}


#endif // Engine_Struct_h__
