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
	HRESULT	Initialize();

public:
	static CModelMgr* Create();

private:
	virtual void	Free();
};

END