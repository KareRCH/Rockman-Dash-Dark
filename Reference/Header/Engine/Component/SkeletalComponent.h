#pragma once

#include "InternalComponent.h"

#include "Component/Interface/ISkeletalComponent.h"

BEGIN(Engine)

enum class EModelGroupIndex : _uint;

/// <summary>
/// 스켈레톤을 관리할 수 있는 컴포넌트
/// 하나의 인스턴스화 된 스켈레톤 그룹을 가진다.
/// 이 컴포넌트는 메쉬 컴포넌트의 부품으로 취급된다.
/// </summary>
class ENGINE_DLL CSkeletalComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CSkeletalComponent)
	struct FInitData
	{
		EModelGroupIndex eGroupIndex;
		wstring strGroupKey;
		wstring strArmatureKey;
	};
protected:
	explicit CSkeletalComponent() = default;
	explicit CSkeletalComponent(const CSkeletalComponent& rhs);
	virtual ~CSkeletalComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	
public:
	static CSkeletalComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

public:
	HRESULT Load_BoneRootNode(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring& strArmatureKey);

private:	// 뼈 정보
	class FArmatureData* m_pArmature = { nullptr };
	
	// 뼈 정보는 계층 정보를 제공해야한다.
	// 뼈 정보는 빠른 참조를 하기 위해 다음과 같은 검색 정보를 제공해야한다.
	// 뼈는 그 그룹이 되는 Armature의 이름으로 찾아진 다음, ID를 통해 찾을 수 있어야 한다.
	// 뼈는 한번 만들어진 다음 추가할 수 없다는 전제하에 로직이 짜여진다.
	// 혹시나 뼈의 계층에 다른 컴포넌트가 붙는다는 가정하에 어타치 기능도 쓸 수 있게 만들어 놓는다.
	// 이때 또한 상수 시간에 접근을 할 수 있도록 ID로 접근하도록 지원한다.
	// 뼈는 기본적으로 계층구조를 가지기 때문에
	
};

END