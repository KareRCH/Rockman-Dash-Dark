#pragma once

#include "Base.h"

BEGIN(Engine)

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
		mapMesh.clear();
		delete this;
	}

public:
	_bool bPermanent = false;		// 영구적으로 로드되는 데이터 인가
	_bool bLoaded = false;			// 로드 되었는가
	_unmap<wstring, MESH> mapMesh;	// 메시 저장 맵
};

/// <summary>
/// Assimp라이브러리를 사용해 모델을 불러오는 클래스
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
	void				Load_Model(const string& strFileName, const wstring& strGroupKey);
	const MESH* const	Get_Model(const wstring& strGroupKey, const wstring& strModelKey);

private:
	const aiScene*	m_pScene = nullptr;
	vector<MESH>	m_vecMesh;
	_uint			m_iMaterialCount = 0U;
	_uint			m_iBoneCount = 0U;

private:
	_unmap<wstring, FModelGroup*>	m_mapModelGroup;
};

END