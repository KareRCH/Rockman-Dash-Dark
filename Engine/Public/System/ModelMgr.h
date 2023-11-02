#pragma once

#include "Base.h"

BEGIN(Engine)

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
	void	Load_Model(const string& strFileName);

	const aiScene* m_pScene = nullptr;
	vector<aiMesh>	m_vecMesh;
	_uint m_iMaterialCount = 0U;
	_uint m_iBoneCount = 0U;
};

END