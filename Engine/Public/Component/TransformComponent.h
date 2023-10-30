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
	explicit CTransformComponent();
	explicit CTransformComponent(const CTransformComponent& rhs);
	virtual ~CTransformComponent();

public:
	virtual HRESULT Initialize();
	virtual _int	Update(const _float& fTimeDelta);
	virtual void	LateUpdate();
	virtual void	Render();

public:
	static	void	Create(ID3D11Device* m_pDevice);

protected:
	virtual void	Free();

public:

};

END