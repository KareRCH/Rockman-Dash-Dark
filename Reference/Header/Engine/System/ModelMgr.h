#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// � �𵨿� ���� �׷�
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
	_bool bPermanent = false;		// ���������� �ε�Ǵ� ������ �ΰ�
	_bool bLoaded = false;			// �ε� �Ǿ��°�
	_unmap<wstring, MESH> mapMesh;	// �޽� ���� ��
};

/// <summary>
/// Assimp���̺귯���� ����� ���� �ҷ����� Ŭ����
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
	string	m_strMainDir = "";			// ������ ���� ���͸�

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