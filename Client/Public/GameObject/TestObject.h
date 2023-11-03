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
/// 테스트용 오브젝트
/// </summary>
class CTestObject final : public CGameObject
{
	DERIVED_CLASS(CGameObject, CTestObject)
protected:
	explicit CTestObject(ID3D11Device* const pDevice);
	explicit CTestObject(const CGameObject& rhs);
	virtual ~CTestObject() = default;

public:
	virtual HRESULT Initialize();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render(ID3D11DeviceContext* const pDeviceContext);

public:
	static CTestObject* Create(ID3D11Device* const pDevice);

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