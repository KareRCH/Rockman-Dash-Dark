#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"
#include "Utility/LogicDeviceBasic.h"

#include "Component/ParticlePointBufferComp.h"

BEGIN(Engine)
class CInstancingModelComp;
END

BEGIN(Client)

class CEffect_Particle : public CGameObject
{
	DERIVED_CLASS(CGameObject, CEffect_Particle)

public:
	struct TParticleDesc
	{
		_uint iNumInstances;
		CInstancingBufferComp::INSTANCING_DESC InstancingDesc;
		wstring strTexturePath;
		_uint	iNumTextures;
		_float4 vBlendColor;
		_float  fBlendStrength;
	};

protected:
	explicit CEffect_Particle();
	explicit CEffect_Particle(const CEffect_Particle& rhs);
	virtual ~CEffect_Particle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	virtual void OnCreated();

public:
	static CEffect_Particle* Create();
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void			Free() override;

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

public:
	HRESULT Initialize_Component();
	HRESULT Bind_ShaderResources();
	HRESULT Create_Instancing(TParticleDesc Desc);

private:
	CInstancingModelComp* m_pInstancingComp = { nullptr };

private:
	FGauge				m_fLifeTime = FGauge(1.f);
	_float4				m_vBlendColor = { 1.f, 1.f, 1.f, 1.f };
	_float				m_fBlendStrength = { 1.f };
};

END