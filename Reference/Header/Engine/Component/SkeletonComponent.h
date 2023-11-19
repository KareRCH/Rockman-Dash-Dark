#pragma once

#include "InternalComponent.h"

BEGIN(Engine)

/// <summary>
/// ���̷����� ������ �� �ִ� ������Ʈ
/// </summary>
class ENGINE_DLL CSkeletonComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CSkeletonComponent)
protected:
	explicit CSkeletonComponent();
	explicit CSkeletonComponent(const CSkeletonComponent& rhs);
	virtual ~CSkeletonComponent() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta) {}
	virtual void	Render() {}
	
public:
	static CSkeletonComponent* Create();
	virtual CComponent* Clone(void* Arg);

protected:
	virtual void	Free();
};

END