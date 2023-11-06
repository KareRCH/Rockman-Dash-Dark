#pragma once

#include "BaseClass/GameObject.h"
#include "Client_Define.h"

BEGIN(Engine)

class CTriBufferComp;
class CModelBufferComp;
class CColorShaderComp;
class CModelShaderComp;

END

BEGIN(Client)

/// <summary>
/// �׽�Ʈ�� ������Ʈ
/// </summary>
class CTestObject final : public CGameObject
{
	DERIVED_CLASS(CGameObject, CTestObject)
protected:
	explicit CTestObject(const DX11DEVICE_T tDevice);
	explicit CTestObject(const CGameObject& rhs);
	virtual ~CTestObject() = default;

public:
	virtual HRESULT Initialize();
	virtual void	PriorityTick();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render();

public:
	static CTestObject* Create(const DX11DEVICE_T tDevice);

protected:
	virtual void	Free();

private:
	HRESULT	Initialize_Component();

private:
	CTriBufferComp* m_pTriBufferComp = nullptr;
	CModelBufferComp* m_pModelBufferComp = nullptr;
	CColorShaderComp* m_pColorShaderComp = nullptr;
	CModelShaderComp* m_pModelShaderComp = nullptr;
};

END