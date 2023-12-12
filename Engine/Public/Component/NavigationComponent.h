#pragma once


#include "Component/PrimitiveComponent.h"


BEGIN(Engine)


/// <summary>
/// 네비게이션 메시를 만들기 위한 컴포넌트이다.
/// </summary>
class CNavigationComponent final : public CPrimitiveComponent
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
	static CNavigationComponent* Create(const wstring& strNavigationFilePath);
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void		Free() override;


private:
	vector<class CNaviCell*>	m_vecCells;


#ifdef _DEBUG
private:
	class CEffectComponent* m_pEffectComp = { nullptr };
#endif // _DEBUG

};

END