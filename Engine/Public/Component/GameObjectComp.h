#pragma once

#include "PrimitiveComponent.h"

BEGIN(Engine)

/// <summary>
/// ��ǥ����� ���� ������Ʈ Ŭ����
/// ���� ��ǥ����� �ʿ� ���� �Ļ�Ŭ������ ����մϴ�.
/// </summary>
class ENGINE_DLL CGameObjectComp abstract : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CGameObjectComp)
protected:
	explicit CGameObjectComp(ID3D11Device* pDevice);
	explicit CGameObjectComp(const CGameObjectComp& rhs);
	virtual ~CGameObjectComp() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	LateTick() PURE;
	virtual void	Render(ID3D11DeviceContext* pDeviceContext) PURE;

public:
	virtual CGameObjectComp* Clone() PURE;

protected:
	virtual void Free();
};

END