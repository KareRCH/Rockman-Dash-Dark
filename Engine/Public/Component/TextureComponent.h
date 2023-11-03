#pragma once

#include "SceneComponent.h"


BEGIN(Engine)

/// <summary>
/// �ؽ�ó�� �����ϴ� ������Ʈ
/// </summary>
class ENGINE_DLL CTextureComponent : public CSceneComponent
{
	DERIVED_CLASS(CSceneComponent, CTextureComponent)
protected:
	explicit CTextureComponent(ID3D11Device* pDevice);
	explicit CTextureComponent(const CTextureComponent& rhs);
	virtual ~CTextureComponent() = default;

public:
	virtual HRESULT Initialize();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render(ID3D11DeviceContext* pDeviceContext);

public:
	static CTextureComponent* Create(ID3D11Device* pDevice);
	virtual CPrimitiveComponent* Clone();

protected:
	virtual void	Free();
};

END