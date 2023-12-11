#pragma once

#include "ModelComponent.h"

BEGIN(Engine)


/// <summary>
/// 학원 프레임워크에 맞춰서 제작된 범용 모델 컴포넌트
/// 이를 기반으로 추후 모델로드 매니저 개선을 할 예정
/// 
/// 모델 매니저와 통신도 가능한 형태로 제작된다.
/// 이 컴포넌트에도 모델 로드 기능은 존재.
/// 이는 프로토타입으로도 모델을 제작 할 수 있게 하기 위함임.
/// </summary>
class ENGINE_DLL CCommonModelComp final : public CModelComponent
{
	DERIVED_CLASS(CModelComponent, CCommonModelComp)

public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

protected:
	explicit CCommonModelComp() = default;
	explicit CCommonModelComp(const CCommonModelComp& rhs);
	virtual ~CCommonModelComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	static CCommonModelComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;


private:
	// 데이터들을 모두 컴포넌트 형태로 저장함. 원본과 다른 부분
	TYPE	m_eModelType = { TYPE_END };


private:	// 메쉬 관련
	_uint m_iNumMeshes = { 0 };
	vector<class CMeshComponent*>	m_Meshes;


public:		// 머터리얼 관련
	_uint	m_iNumMaterials = { 0 };
	vector<class CMaterialComponent*>	m_pMaterials;


private:	// 뼈 관련
	// 뼈 하나의 정보가 저장된 컴포넌트 여러개, 언리얼 용어를 따왔다.
	_uint	m_iNumSkeletals = { 0 };
	vector<class CSkeletalComponent*>	m_Skeletals;


private:	// 애니메이션 관련
	_uint	m_iNumAnimations = { 0 };
	_uint	m_iCurrentAnimIndex = { 0 };
	vector<class CAnimationComponent*>	m_pAnimations;

};

END