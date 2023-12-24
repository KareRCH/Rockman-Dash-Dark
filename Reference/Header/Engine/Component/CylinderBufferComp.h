#pragma once

#include "Component/VIBufferComp.h"


BEGIN(Engine)


/// <summary>
/// 원기둥 버퍼를 가지는 클래스
/// 사용전 Create_Buffer로 버퍼를 세팅해주어야 사용이 가능하다.
/// </summary>
class ENGINE_DLL CCylinderBufferComp : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CCylinderBufferComp)
public:
	struct TBufferDesc
	{
		_uint iNumSegments;
	};

protected:
	explicit CCylinderBufferComp() = default;
	explicit CCylinderBufferComp(const CCylinderBufferComp& rhs);
	virtual ~CCylinderBufferComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static	CCylinderBufferComp*	Create();
	virtual CComponent*				Clone(void* Arg = nullptr) override;

protected:
	virtual void					Free() override;

public:
	HRESULT Create_Buffer(const TBufferDesc tDesc);
};

END