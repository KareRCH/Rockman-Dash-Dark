#pragma once

#include "BaseClass/GameObject.h"

BEGIN(Engine)

class CTriBufferComp;
class CColorShaderComp;

END

BEGIN(Client)

class CTestTextureObject final : public CGameObject
{
	DERIVED_CLASS(CGameObject, CTestTextureObject)
protected:
	explicit CTestTextureObject(ID3D11Device* const pDevice);
	explicit CTestTextureObject(const CTestTextureObject& rhs);
	virtual ~CTestTextureObject() = default;

public:
	virtual HRESULT Initialize();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render(ID3D11DeviceContext* const pDeviceContext);

public:
	static CTestTextureObject* Create(ID3D11Device* const pDevice);

protected:
	virtual void	Free();

private:
	HRESULT	Initialize_Component();

private:
	CTriBufferComp* m_TriBufferComp = nullptr;
	CColorShaderComp* m_ColorShaderComp = nullptr;
};

END