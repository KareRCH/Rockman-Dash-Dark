#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FMeshData final
{
	THIS_CLASS(FMeshData)
private:
	explicit FMeshData() {}
	explicit FMeshData(const FMeshData& rhs)
		: vecVertices(rhs.vecVertices), vecIndices(rhs.vecIndices), vecBoneName(rhs.vecBoneName)
		, iMaterIndex(rhs.iMaterIndex), matTransform(rhs.matTransform)
	{
	}
	virtual ~FMeshData() = default;

public:
	static FMeshData* Create()
	{
		ThisClass* pInstance = new ThisClass();

		if (!pInstance)
		{
			delete pInstance;
			pInstance = nullptr;

			MSG_BOX("FMeshData Create Failed");

			return nullptr;
		}

		return pInstance;
	}
	FMeshData* Clone()
	{
		ThisClass* pInstance = new ThisClass(*this);

		if (!pInstance)
		{
			delete pInstance;
			pInstance = nullptr;

			MSG_BOX("FMeshData Copy Failed");

			return nullptr;
		}

		return pInstance;
	}

	void Free()
	{
		vecVertices.clear();
		vecIndices.clear();
		vecBoneName.clear();
		delete this;
	}

public:
	vector<VERTEX_MODEL>	vecVertices;
	vector<_uint>			vecIndices;
	vector<wstring>			vecBoneName;
	_uint					iMaterIndex;
	_float4x4				matTransform;
};

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FMeshGroup final
{
	THIS_CLASS(FMeshGroup)
private:
	explicit FMeshGroup() {}
	virtual ~FMeshGroup() = default;

public:
	static FMeshGroup* Create()
	{
		ThisClass* pInstance = new ThisClass();

		if (!pInstance)
		{
			delete pInstance;
			pInstance = nullptr;

			MSG_BOX("FMeshGroup Create Failed");

			return nullptr;
		}

		return pInstance;
	}
	void Free()
	{
		for (auto& Pair : mapMeshData)
		{
			Pair.second->Free();
		}
		mapMeshData.clear();
		delete this;
	}

public:
	const FMeshData* const	Get_Mesh(const wstring& strMeshKey)
	{
		auto iter = mapMeshData.find(strMeshKey);
		if (iter == mapMeshData.end())
			return nullptr;

		return (*iter).second;
	}

	void Add_Mesh(const wstring& strMeshKey, FMeshData* const pMeshData)
	{
		auto iter = mapMeshData.find(strMeshKey);
		if (iter != mapMeshData.end())
			return;

		mapMeshData.emplace(strMeshKey, pMeshData);
	}

public:
	_unmap<wstring, FMeshData*> mapMeshData;	// 메시 저장 맵
};

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FBoneData final
{
	THIS_CLASS(FBoneData)
private:
	explicit FBoneData() {}
	virtual ~FBoneData() = default;

public:
	static FBoneData* Create()
	{
		ThisClass* pInstance = new ThisClass();

		if (!pInstance)
		{
			delete pInstance;
			pInstance = nullptr;

			MSG_BOX("FBoneData Create Failed");

			return nullptr;
		}

		return pInstance;
	}

	void Free()
	{
		vecVerticeWeights.clear();
		delete this;
	}

public:
	wstring					strName;
	vector<_uint>			vecVerticeWeights;
	_matrix					matOffset;
};

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FBoneGroup final
{
	THIS_CLASS(FBoneGroup)
private:
	explicit FBoneGroup() {}
	virtual ~FBoneGroup() = default;

public:
	static FBoneGroup* Create()
	{
		ThisClass* pInstance = new ThisClass();

		if (!pInstance)
		{
			delete pInstance;
			pInstance = nullptr;

			MSG_BOX("FBoneGroup Create Failed");

			return nullptr;
		}

		return pInstance;
	}

	void Free()
	{
		for (auto& Pair : mapBoneData)
		{
			Pair.second->Free();
		}
		mapBoneData.clear();
		delete this;
	}

public:
	const FBoneData* const	Get_Bone(const wstring& strBoneKey)
	{
		auto iter = mapBoneData.find(strBoneKey);
		if (iter == mapBoneData.end())
			return nullptr;

		return (*iter).second;
	}

public:
	_unmap<wstring, FBoneData*> mapBoneData;	// 뼈 저장 맵
};



/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FAnimNodeData final
{
	THIS_CLASS(FAnimNodeData)
private:
	explicit FAnimNodeData() {}
	virtual ~FAnimNodeData() = default;

public:
	static FAnimNodeData* Create()
	{
		ThisClass* pInstance = new ThisClass();

		if (!pInstance)
		{
			delete pInstance;
			pInstance = nullptr;

			MSG_BOX("FAnimNodeData Create Failed");

			return nullptr;
		}

		return pInstance;
	}

	void Free()
	{
		vecPositions.clear();
		vecRotations.clear();
		vecScales.clear();
		delete this;
	}

public:
	struct FAnimPosition
	{
		_float vTime;
		_float3 vPos;
	};

	struct FAnimRotation
	{
		_float vTime;
		_float4 qtRot;
	};

	struct FAnimScale
	{
		_float vTime;
		_float3 vScale;
	};

public:
	wstring						strName = { L"" };	// 애니메이션이 작동되는 객체 이름
	vector<FAnimPosition>		vecPositions;
	vector<FAnimRotation>		vecRotations;
	vector<FAnimScale>			vecScales;
};

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FAnimData final
{
	THIS_CLASS(FAnimData)
private:
	explicit FAnimData() {}
	virtual ~FAnimData() = default;

public:
	static FAnimData* Create()
	{
		ThisClass* pInstance = new ThisClass();

		if (!pInstance)
		{
			delete pInstance;
			pInstance = nullptr;

			MSG_BOX("FAnimData Create Failed");

			return nullptr;
		}

		return pInstance;
	}

	void Free()
	{
		for (auto& item : vecNodeAnim)
		{
			item->Free();
		}

		vecNodeAnim.clear();
		delete this;
	}

public:
	_double fDuration = 0.0;
	_double fTicksPerSecond = 0.0;
	vector<FAnimNodeData*> vecNodeAnim;
};

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FAnimGroup final
{
	THIS_CLASS(FAnimGroup)
private:
	explicit FAnimGroup() {}
	virtual ~FAnimGroup() = default;

public:
	static FAnimGroup* Create()
	{
		ThisClass* pInstance = new ThisClass();

		if (!pInstance)
		{
			delete pInstance;
			pInstance = nullptr;

			MSG_BOX("FAnimGroup Create Failed");

			return nullptr;
		}

		return pInstance;
	}
	void Free()
	{
		for (auto& Pair : mapAnimData)
		{
			Pair.second->Free();
		}
		mapAnimData.clear();
		delete this;
	}

public:
	const FAnimData* const	Get_Anim(const wstring& strAnimKey)
	{
		auto iter = mapAnimData.find(strAnimKey);
		if (iter == mapAnimData.end())
			return nullptr;

		return (*iter).second;
	}

	void Add_AnimData(const wstring& strAnimKey, FAnimData* const pAnimData)
	{
		auto iter = mapAnimData.find(strAnimKey);
		if (iter == mapAnimData.end())
			return;

		mapAnimData.emplace(strAnimKey, pAnimData);
	}

public:
	_unmap<wstring, FAnimData*> mapAnimData;	// 애니메이션 저장 맵

};

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FModelGroup final
{
	THIS_CLASS(FModelGroup)
private:
	explicit FModelGroup() {}
	virtual ~FModelGroup() = default;

public:
	static FModelGroup* Create(const _bool bPermanent, const _bool bLoaded)
	{
		ThisClass* pInstance = new ThisClass();

		if (!pInstance)
		{
			delete pInstance;
			pInstance = nullptr;

			MSG_BOX("FModelGroup Create Failed");

			return nullptr;
		}

		pInstance->bPermanent = bPermanent;
		pInstance->bLoaded = bLoaded;

		return pInstance;
	}
	void Free()
	{
		pMeshGroup->Free();
		pBoneGroup->Free();
		pAnimGroup->Free();
		delete this;
	}

public:
	void Set_AllLoaded() { bLoaded = true; }
	void Set_Permanent() { bPermanent = true; }
	void UnSet_Permanent() { bPermanent = false; }

public:
	_bool		bPermanent = false;			// 영구적으로 로드되는 데이터 인가
	_bool		bLoaded = false;			// 로드 되었는가
	FMeshGroup* pMeshGroup = { nullptr };
	FBoneGroup* pBoneGroup = { nullptr };
	FAnimGroup* pAnimGroup = { nullptr };
};

/// <summary>
/// Assimp라이브러리를 사용해 모델을 불러오는 클래스
/// 메쉬, 뼈, 애니메이션 정보를 저장합니다.
/// </summary>
class ENGINE_DLL_DBG CModelMgr final : public CBase
{
	DERIVED_CLASS(CBase, CModelMgr)

private:
	explicit CModelMgr();
	virtual ~CModelMgr() = default;

public:
	HRESULT	Initialize(const string& strMainDir);

public:
	static CModelMgr* Create(const string& strMainDir);

private:
	virtual void	Free();

private:
	string	m_strMainDir = "";			// 참조할 메인 디렉터리

public:
	void					Load_Model(const string& strFileName, const wstring& strGroupKey);
	const FMeshData* const	Get_Mesh(const wstring& strGroupKey, const wstring& strMeshKey);

private:
	FModelGroup* Get_ModelGroup(const wstring& strGroupKey);
	FModelGroup* Add_ModelGroup(const wstring& strGroupKey);

	FMeshGroup* Get_MeshGroup(const wstring& strGroupKey);
	FBoneGroup* Get_BoneGroup(const wstring& strGroupKey);
	FAnimGroup* Get_AnimGroup(const wstring& strGroupKey);

private:
	const aiScene*		m_pScene = nullptr;				// 내부 통신용 씬 저장변수
	vector<FMeshData*>	m_vecMesh;						// 내부 통신용 메쉬 저장변수
	_uint				m_iMaterialCount = 0U;
	_uint				m_iBoneCount = 0U;

private:
	_unmap<wstring, FModelGroup*>	m_mapModelGroup;	// 모델 정보, 메쉬, 뼈, 머터리얼, 애니메이션

};

END