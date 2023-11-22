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
	explicit CGameObjectComp() = default;
	explicit CGameObjectComp(const CGameObjectComp& rhs);
	virtual ~CGameObjectComp() = default;

public:
	virtual HRESULT Initialize(void* Arg = nullptr) PURE;
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void Free() override;
};

END