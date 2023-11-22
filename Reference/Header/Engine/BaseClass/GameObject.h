#pragma once

#include "Base.h"
#include "Component/TransformComponent.h"
#include "Component/Component_Define.h"
#include "Component/Interface/ITransform.h"

#include "BaseClass/GameObject_Define.h"

BEGIN(Engine)

class CPrimitiveComponent;

/// <summary>
/// 씬에 추가되어 사용되는 좌표를 기본적으로 탑재하는 오브젝트 클래스
/// 좌표가 필요없는 클래스는 씬이 아닌 별도의 시스템을 사용합니다.
/// </summary>
class ENGINE_DLL CGameObject abstract : public CBase, public ITransform
{
	DERIVED_CLASS(CBase, CGameObject)
protected:
	explicit CGameObject() = default;
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype() PURE;
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	virtual CGameObject* Clone(void* Arg) PURE;

protected:
	virtual void	Free() override;

public:
	GETSET_2(wstring, m_strName, Name, GET_C_REF, SET_C_REF)

	void		Add_Tag(const wstring& strTag) { m_setTag.emplace(strTag); }
	void		Delete_Tag(const wstring& strTag);

	_bool		IsDead() { return m_iStateFlag & Cast_EnumDef(EGObjectState::Dead); }
	void		Set_Dead() { m_iStateFlag |= Cast_EnumDef(EGObjectState::Dead); }
	
	_float		Get_Priority(_uint iIndex) { return m_fPriority[iIndex]; }

	void		TurnOn_State(const EGObjectState value) { m_iStateFlag |= Cast_EnumDef(value); }
	void		TurnOff_State(const EGObjectState value) { m_iStateFlag &= ~Cast_EnumDef(value); }
	void		Toggle_State(const EGObjectState value) { m_iStateFlag ^= Cast_EnumDef(value); }
	_bool		IsState(const EGObjectState value) { return (m_iStateFlag & Cast_EnumDef(value)); }

private:	// 기본 속성
	_uint_64					m_iID = 0ULL;			// 식별용 ID
	wstring						m_strName = L"";		// 오브젝트 이름
	_unset<wstring>				m_setTag;				// 분류, 제어용 태그
	_uint						m_iStateFlag = 0U;		// 상태 플래그
	
	_float						m_fPriority[Cast_EnumDef(EGObjTickPriority::Size)];	// 우선도

public: // 각 오브젝트는 자식 오브젝트를 가질 수 있음
	GETSET_2(CGameObject*, m_pOwner, Owner, GET_REF_C, SET__C)

private:
	CGameObject* m_pOwner = nullptr;

#pragma region 컴포넌트
public:
	// 검색용
	HRESULT Add_Component(const wstring& strName, CPrimitiveComponent* pComponent);
	CPrimitiveComponent* Get_Component(const _uint iIndex)
	{
		return m_vecComponent[iIndex];
	}
	CPrimitiveComponent* Get_Component(const wstring& strName);

private:
	HRESULT Initialize_Component();

public:		// 컴포넌트의 상태 변경시 자동으로 변경해주기 위한 이벤트 함수
	void OnStateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue);
	void OnStateLateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue);
	void OnStateRender_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue);

private:	// 컴포넌트 속성
	vector<CPrimitiveComponent*>			m_vecComponent;				// 컴포넌트 관리 컨테이너
	_unmap<wstring, CPrimitiveComponent*>	m_mapPrimComp;				// 오브젝트 전용 컴포넌트 관리 컨테이너

	using list_comp =						list<CPrimitiveComponent*>;
	list_comp								m_listUpdateComp[Cast_EnumDef(ECompTickType::Size)];	// 컴포넌트 업데이트 관리 리소스
#pragma endregion

#pragma region 트랜스폼 컴포넌트
public:		// 트랜스폼 컴포넌트에 대한 함수 정의
	// 반드시 초기화가 되었을 때 사용해야 함.
	inline virtual CTransformComponent& Transform() override { return (*m_pTransformComp); }
	inline virtual void Release_Transform() override { Safe_Release(m_pTransformComp); }

private:	// 게임 오브젝트 기본 정의 컴포넌트
	CTransformComponent* m_pTransformComp = nullptr;
#pragma endregion



};

END