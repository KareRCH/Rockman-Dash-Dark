#pragma once

#include "Component/SceneComponent.h"

BEGIN(Engine)

/// <summary>
/// 씬 컴포넌트로부터 상속받은 좌표계를 통해 추가적인
/// 행렬 조작을 할 수 있도록 만들어진 클래스
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