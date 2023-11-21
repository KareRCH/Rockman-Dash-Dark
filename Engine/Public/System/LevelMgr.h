#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// 레벨을 변경하고 연결해주고, 레벨에 관한 자원해제와 같은 것을 컨트롤 한다.
/// </summary>
class CLevelMgr final : public CBase
{
	DERIVED_CLASS(CBase, CLevelMgr)
private:
	explicit CLevelMgr() {}
	explicit CLevelMgr(const CLevelMgr& rhs) = delete;
	virtual ~CLevelMgr() = default;

public:
	HRESULT Initialize();
	void Tick(const _float& fTimeDelta);
	HRESULT Render();

public:
	static CLevelMgr* Create();

private:
	virtual void Free() override;

public:
	HRESULT Open_Level(_uint iCurrentLevelIndex, class CLevel* pNewLevel);

private:
	_uint			m_iCurrentLevelIndex = { 0 };
	class CLevel*	m_pCurrentLevel = { nullptr };
};

END