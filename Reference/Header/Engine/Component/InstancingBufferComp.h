#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

class CInstancingBufferComp abstract : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CInstancingBufferComp)

public:
	typedef struct
	{
		_float3		vCenter;
		_float		fRange;
		_float2		vSpeed;
		_float2		vScale;
		_float2		vLifeTime;
	}INSTANCING_DESC;

protected:
	explicit CInstancingBufferComp() = default;
	explicit CInstancingBufferComp(const CInstancingBufferComp& rhs);
	virtual ~CInstancingBufferComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void Free() override;

public:
	virtual HRESULT Bind_Buffer() override;
	virtual void	Update_Buffer(_float fTimeDelta);
	virtual HRESULT Render_Buffer();

protected:
	ID3D11Buffer*				m_pVBInstance = { nullptr };
	_uint						m_iInstanceStride = { 0 };
	_uint						m_iNumInstance = { 0 };
	_uint						m_iIndexCountPerInstance = { 0 };

protected:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

protected:
	_float3*					m_pSpeeds = { nullptr };
	_float*						m_pLifeTimes = { nullptr };
	INSTANCING_DESC				m_InstancingDesc;
	_float						m_fTimeAcc = { 0.0f };

};

END