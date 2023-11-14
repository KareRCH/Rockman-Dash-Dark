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
	explicit CTestObject(const DX11DEVICE_T tDevice);
	explicit CTestObject(const CGameObject& rhs);
	virtual ~CTestObject() = default;

public:
	virtual HRESULT Initialize();
	virtual HRESULT Initialize(const _float3 vPos);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	static CTestObject* Create(const DX11DEVICE_T tDevice);
	static CTestObject* Create(const DX11DEVICE_T tDevice, const _float3 vPos);
	CGameObject* Clone(void* Arg);

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