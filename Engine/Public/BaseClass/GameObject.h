#pragma once

#include "Base.h"
#include "Component/TransformComponent.h"

BEGIN_NAME(Engine)

class CPrimitiveComponent;


/// <summary>
/// 씬에 추가되어 사용되는 좌표를 기본적으로 탑재하는 오브젝트 클래스
/// 좌표가 필요없는 클래스는 씬이 아닌 별도의 시스템을 사용합니다.
/// </summary>
class ENGINE_DLL CGameObject abstract : public CBase
{
	DERIVED_CLASS(CBase, CGameObject)
protected:
	explicit CGameObject();
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	virtual HRESULT Initialize();
	virtual _int	Update(const _float& fTimeDelta);
	virtual void	LateUpdate();
	virtual void	Render();

protected:
	virtual void	Free();

private:	// 기본 속성
	_uint_64					m_iID = 0ULL;			// 식별용 ID
	wstring						m_strName = L"";		// 오브젝트 이름
	_unset<wstring>				m_setTag;				// 분류, 제어용 태그
	_uint						m_iStateFlag = 0U;		// 상태 플래그
	
	_float						m_fPriority[Cast_EnumDef(EUPDATE_T::SIZE)];	// 우선도

public: // 각 오브젝트는 자식 오브젝트를 가질 수 있음
	GETSET_2(CGameObject*, m_pOwner, Owner, GET_REF_C, SET__C)

private:
	CGameObject* m_pOwner = nullptr;

public:
	void Add_Component(const wstring& strName, CPrimitiveComponent* pComponent);
	CPrimitiveComponent* Get_Component(const _uint iIndex)
	{
		return m_vecComponent[iIndex];
	}
	CPrimitiveComponent* Get_Component(const wstring& strName);

public:		// 컴포넌트의 상태 변경시 자동으로 변경해주기 위한 이벤트 함수
	void OnStateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue);
	void OnStateLateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue);
	void OnStateRender_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue);

private:
	HRESULT Initialize_Component();

private:	// 컴포넌트 속성
	vector<CPrimitiveComponent*>			m_vecComponent;				// 컴포넌트 관리 컨테이너

	using list_comp =						list<CPrimitiveComponent*>;
	list_comp								m_listUpdateComp[Cast_EnumDef(EUPDATE_T::SIZE)];	// 컴포넌트 업데이트 관리 리소스


public:		// 트랜스폼 컴포넌트에 대한 함수 정의
	const _vec3	Get_Position() const { return m_pTransformComp->Get_Position(); }
	void		Set_Position(const _vec3& value) { m_pTransformComp->Set_Position(value); }

	const _vec3	Get_Rotation() const { return m_pTransformComp->Get_Rotation(); }
	void		Set_Rotation(const _vec3& value) { m_pTransformComp->Set_Rotation(value); }

	const _vec3	Get_Scale() const { return m_pTransformComp->Get_Scale(); }
	void		Set_Scale(const _vec3& value) { m_pTransformComp->Set_Scale(value); }

	void		Calculate_Transform() { m_pTransformComp->Calculate_Transform(); }
	void		Calculate_TransformFromParent(const _matrix& matTransform) 
					{ m_pTransformComp->Calculate_TransformFromParent(matTransform); }

private:	// 게임 오브젝트 기본 정의 컴포넌트
	CTransformComponent*					m_pTransformComp = nullptr;
};

END_NAME