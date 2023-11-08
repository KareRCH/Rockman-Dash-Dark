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
	explicit CTransformComponent(const DX11DEVICE_T tDeivce);
	explicit CTransformComponent(const CTransformComponent& rhs);
	virtual ~CTransformComponent() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	static	CTransformComponent* Create(const DX11DEVICE_T tDevice);
	virtual CPrimitiveComponent* Clone(void* Arg) override;

protected:
	virtual void	Free();

};

END