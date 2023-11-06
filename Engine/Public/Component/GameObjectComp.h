#pragma once

#include "PrimitiveComponent.h"

BEGIN(Engine)

/// <summary>
/// 좌표기능이 빠진 컴포넌트 클래스
/// 로컬 좌표기능이 필요 없는 파생클래스로 사용합니다.
/// </summary>
class ENGINE_DLL CGameObjectComp abstract : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CGameObjectComp)
protected:
	explicit CGameObjectComp(const DX11DEVICE_T tDevice);
	explicit CGameObjectComp(const CGameObjectComp& rhs);
	virtual ~CGameObjectComp() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual void	PriorityTick() PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	LateTick() PURE;
	virtual void	Render() PURE;

public:
	virtual CGameObjectComp* Clone(void* Arg) PURE;

protected:
	virtual void Free();
};

END