#pragma once

#include "BaseClass/GameObject.h"
#include "Client_Define.h"

#include "Utility/LogicDeviceBasic.h"

BEGIN(Engine)

class CTriBufferComp;
class CCommonModelComp;
class CColorShaderComp;
class CModelShaderComp;

END



BEGIN(Client)

/// <summary>
/// 테스트용 오브젝트
/// </summary>
class CPlayer final : public CGameObject
{
	DERIVED_CLASS(CGameObject, CPlayer)

protected:
	explicit CPlayer();
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype(const _float3 vPos);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(const _float3 vPos);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CPlayer* Create();
	static CPlayer* Create(const _float3 vPos);
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();

private:
	CCommonModelComp* m_pModelComp = { nullptr };

private:
	FGauge	m_Gauge = FGauge(2.f);
	_bool	m_bTest = false;
};

END