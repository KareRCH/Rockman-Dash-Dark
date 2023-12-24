#pragma once


#include "Component/PrimitiveComponent.h"


BEGIN(Engine)


/// <summary>
/// 네비게이션에 접근하여 이동할 수 있도록 하는 클래스
/// 네비게이션 셀을 생성하는 기능, 탐색 기능이 존재한다.
/// 전자는 네비게이션을 관리하는 객체. Navigation 클래스이며
/// 후자는 네비게이션을 사용하는 객체. 게임오브젝트들에 해당한다.
/// </summary>
class ENGINE_DLL CNavigationComponent final : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CNavigationComponent)
protected:
	explicit CNavigationComponent() = default;
	explicit CNavigationComponent(const CNavigationComponent& rhs);
	virtual ~CNavigationComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg) override { return S_OK; }
	virtual HRESULT	Initialize_Prototype(const wstring& strNavigationFilePath);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CNavigationComponent*	Create(const wstring& strNavigationFilePath);
	virtual CComponent*				Clone(void* Arg = nullptr) override;

protected:
	virtual void					Free() override;


private:
	_int						m_iCurrentNavi = { -1 };		// 현재 들어가 있는 네비게이션 or 담당 네비게이션

	vector<class CNaviCell*>	m_vecCells;						// 네비게이션 셀
	_int						m_iCurrentCell = { -1 };		// 현재 셀
	


#ifdef _DEBUG
private:
	class CEffectComponent* m_pEffectComp = { nullptr };
#endif // _DEBUG

};

END