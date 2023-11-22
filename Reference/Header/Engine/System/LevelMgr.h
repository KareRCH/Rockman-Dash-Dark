#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// ������ �����ϰ� �������ְ�, ������ ���� �ڿ������� ���� ���� ��Ʈ�� �Ѵ�.
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