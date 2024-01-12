#pragma once

#include "SceneComponent.h"

#include "Component/MaterialComponent.h"


BEGIN(Engine)

/// <summary>
/// 트랜스폼을 가지고, 기본적으로 그래픽을 표시하기 위한 버퍼를 가지는 씬 컴포넌트.
/// 일반 모델 컴포넌트는 단순히 모델을 표시하기 위한 버퍼와, 재질 정도를 가집니다.
/// 뼈, 애니메이션을 포함하는 객체를 만드려면 이 객체를 상속받아 정의 하십시오.
/// </summary>
class ENGINE_DLL CModelComponent : public CSceneComponent
{
	DERIVED_CLASS(CSceneComponent, CModelComponent)

protected:
	explicit CModelComponent();
	explicit CModelComponent(const CModelComponent& rhs);
	virtual ~CModelComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render() PURE;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void	Free() override;


public:
	// 원하는 패스를 그릴 수 있도록 추가한 함수
	void Set_ActivePass(_uint iPass);
	// 필요없어진 패스를 지울 수 있도록 설정하는 함수
	void Unset_ActivePass(_uint iPass);
	// 액티브 패스 초기화
	void Reset_ActivePass();

protected:
	_uint				m_iNumActivePasses = { 0 };
	vector<_uint>		m_vecActivePasses;
	
};

END