#pragma once

#include "Component/SceneComponent.h"

BEGIN(Engine)

/// <summary>
/// �� ������Ʈ�κ��� ��ӹ��� ��ǥ�踦 ���� �߰�����
/// ��� ������ �� �� �ֵ��� ������� Ŭ����
/// </summary>
class ENGINE_DLL CTransformComponent : public CSceneComponent
{
	DERIVED_CLASS(CSceneComponent, CTransformComponent)
protected:
	explicit CTransformComponent(ID3D11Device* pGraphicDev);
	explicit CTransformComponent(const CTransformComponent& rhs);
	virtual ~CTransformComponent() = default;

public:
	virtual HRESULT Initialize();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render(ID3D11DeviceContext* pDeviceContext);

public:
	static	void	Create(ID3D11Device* m_pDevice);

protected:
	virtual void	Free();

public:

};

END