#pragma once

#include "BaseClass/GameObject.h"
#include "Client_Define.h"

BEGIN(Client)

class CTestObject final : public CGameObject
{
	DERIVED_CLASS(CGameObject, CTestObject)
protected:
	explicit CTestObject(ID3D11Device* pGraphicDev);
	explicit CTestObject(const CGameObject& rhs);
	virtual ~CTestObject() = default;

public:
	virtual HRESULT Initialize();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render(ID3D11DeviceContext* pDeviceContext);

protected:
	virtual void	Free();
};

END