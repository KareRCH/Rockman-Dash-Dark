#pragma once

#include "InternalComponent.h"

BEGIN(Engine)

enum class EModelGroupIndex : _uint;

/// <summary>
/// 스켈레톤을 관리할 수 있는 컴포넌트
/// 하나의 인스턴스화 된 스켈레톤 그룹을 가진다.
/// 이 컴포넌트는 메쉬 컴포넌트의 부품으로 취급된다.
/// </summary>
class ENGINE_DLL CSkeletonComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CSkeletonComponent)
	struct FInitialData
	{
		wstring strArmatureKey;
	};
protected:
	explicit CSkeletonComponent();
	explicit CSkeletonComponent(const CSkeletonComponent& rhs);
	virtual ~CSkeletonComponent() = default;

public:
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	
public:
	static CSkeletonComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free();

public:
	HRESULT Load_BoneRootNode(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const wstring& strArmatureKey);

private:	// 뼈 정보
	class FArmatureData* m_pBoneRootNode = { nullptr };	// 베이스가 되는 루트 노도의 뼈
};

END