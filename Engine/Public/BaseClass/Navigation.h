#pragma once


#include "GameObject.h"


BEGIN(Engine)

/// <summary>
/// 네비게이션을 터레인에 귀속시키지 않고 별도로 관리하기 위해 생성한 클래스
/// 네비게이션의 월드 위치와 이웃하는 네비게이션을 가진다.
/// 대체적으로 관련 기능은 네비게이션 컴포넌트에 지니고 있으며.
/// 이 클래스는 네비게이션간의 영역을 오가는 기능을 가진다.
/// </summary>
class ENGINE_DLL CNavigation : public CGameObject
{
	DERIVED_CLASS(CGameObject, CNavigation)

protected:
	explicit CNavigation();
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype(const _float3 vPos);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(const _float3 vPos);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CNavigation* Create();
	static CNavigation* Create(const _float3 vPos);
	CGameObject*		Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

public:
	// 직렬화 데이터 뽑아낼 때 사용하는 함수. 각 오브젝트 별로 뽑아내는 데이터가 다릅니다.
	virtual FSerialData SerializeData();

private:
	HRESULT	Initialize_Component();

private:
	class CNavigationComponent* m_pNaviComp = { nullptr };
};

END