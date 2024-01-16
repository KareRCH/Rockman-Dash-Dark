#pragma once


#include "Component/PrimitiveComponent.h"
#include "Utility/ClassID.h"

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

public:
	static const _uint g_ClassID = ECast(EComponentID::Navigation);

public:
	struct TCloneDesc
	{
		_uint iCurrentInex;
	};

protected:
	explicit CNavigationComponent();
	explicit CNavigationComponent(const CNavigationComponent& rhs);
	virtual ~CNavigationComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT	Initialize_Prototype(const wstring& strNavigationFilePath);
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	// 프로토타입 제작용 함수
	virtual FSerialData SerializeData_Prototype();
	// 클로닝 전용 함수
	virtual FSerialData SerializeData();

public:
	static CNavigationComponent*	Create();
	static CNavigationComponent*	Create(const wstring& strNavigationFilePath);
	static	CNavigationComponent*	Create(FSerialData& InputData);
	virtual CComponent*				Clone(void* Arg = nullptr) override;
	virtual CComponent*				Clone(FSerialData& InputData);

protected:
	virtual void					Free() override;


public:
	HRESULT Load_NavigationFromFile(const wstring& strNavigationFilePath);
	HRESULT Make_Neighbors();
	_bool	IsMove(_fvector vPosition);


private:
	_int						m_iCurrentNavi = { -1 };		// 현재 들어가 있는 네비게이션 or 담당 네비게이션

	vector<class CNaviCell*>	m_vecCells;						// 네비게이션 셀
	_int						m_iCurrentCell = { -1 };		// 현재 셀
	


#ifdef _DEBUG
private:
	class CEffectComponent* m_pEffectComp = { nullptr };
	class CPipelineComp* m_pPipelineComp = { nullptr };
#endif // _DEBUG

};

template <>
struct TComponentTrait<CNavigationComponent::g_ClassID>
{
	using Class = CNavigationComponent;
};

END