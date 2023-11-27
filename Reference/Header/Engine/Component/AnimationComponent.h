#pragma once

#include "InternalComponent.h"
#include "Component/Interface/IAnimationComponent.h"

BEGIN(Engine)

class FBoneData;

/// <summary>
/// 애니메이션에 대한 처리를 담당하는 컴포넌트
/// </summary>
class ENGINE_DLL CAnimationComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CAnimationComponent)
protected:
	explicit CAnimationComponent() = default;
	explicit CAnimationComponent(const CAnimationComponent& rhs);
	virtual ~CAnimationComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CAnimationComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;


public:
	// 뼈에 대해 애니메이션을 적용해준다.
	HRESULT Apply_BoneAnimation(FBoneData* const pBoneData);

private:
	class FBoneAnimData* m_pAnimData = { nullptr };
	// 애니메이션은 뼈와 동기화되어 작동한다.
	// 뼈에 대해 적용할 때 속도를 위해 인덱스를 사용하는 방식을 사용한다.
	// 때문에 뼈는 인덱싱이 완료된 상태여야 하며, 애니메이션 또한 뼈에 대한 인덱싱이 완료된 데이터를 사용해야 한다.
	// 이는 모델 매니저에서 로드한 정보대로 작동하도록 한다.
};

END