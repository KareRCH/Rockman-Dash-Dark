#pragma once

#include "BaseClass/GameObject.h"
#include "Client_Define.h"

#include "Utility/LogicDeviceBasic.h"

BEGIN(Engine)

class CTriBufferComp;
class CSkinnedModelComp;
class CColorShaderComp;
class CModelShaderComp;

END

BEGIN(Client)

/// <summary>
/// 테스트용 오브젝트
/// </summary>
class CTestObject final : public CGameObject
{
	DERIVED_CLASS(CGameObject, CTestObject)

protected:
	explicit CTestObject();
	explicit CTestObject(const CTestObject& rhs);
	virtual ~CTestObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(const _float3 vPos);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual _int	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual void	Render() override;

public:
	static CTestObject* Create();
	static CTestObject* Create(const _float3 vPos);
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();

private:
	CSkinnedModelComp* m_pModelComp = { nullptr };

private:
	FGauge	m_Gauge = FGauge(30.f);
};

END