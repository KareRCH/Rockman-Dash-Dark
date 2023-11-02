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
	HRESULT	Initialize();

public:
	static CModelMgr* Create();

private:
	virtual void	Free();
};

END