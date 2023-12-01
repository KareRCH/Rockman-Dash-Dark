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
	// 참조할 모델 데이터를 추가한다.
	void Set_ModelData(class FModelData* pModelData);
	// 아마추어를 로드한다.
	HRESULT Load_Skeletal(const wstring& strSkeletalKey);

	// 최종 트랜스폼의 주소를 저장한 벡터를 내보내, 버퍼에 전달 할 수 있도록 해준다.
	vector<_float4x4>	Get_FinalTransforms();

private:	// 뼈 정보
	class FModelData* m_pModelData = { nullptr };		// 모델 데이터, 해당 모델을 바인딩 시켜놓기 위해 해놓는다.
	class FSkeletalData* m_pSkeletalData = { nullptr };		// 아마추어, 이 녀석이 뼈의 계층구조와 직렬화된 배열을 가지고 있다.
	
};

END