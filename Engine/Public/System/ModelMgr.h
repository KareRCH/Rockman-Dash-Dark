#pragma once

#include "Base.h"

BEGIN(Engine)

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
	void	Load_Model(const string& strFileName);

	const aiScene* m_pScene = nullptr;
	vector<aiMesh>	m_vecMesh;
	_uint m_iMaterialCount = 0U;
	_uint m_iBoneCount = 0U;
};

END