#pragma once

#include "Component/TransformComponent.h"
#include "Component/PipelineComp.h"

#include "System/Define/RenderMgr_Define.h"
#include "Component/Define/Component_Define.h"
#include "BaseClass/GameObject_Define.h"
#include "Component/GameObjectComp.h"
#include "Utility/RapidJsonSerial.h"

BEGIN(Engine)

class CGameObjectComp;

/// <summary>
/// 씬에 추가되어 사용되는 좌표를 기본적으로 탑재하는 오브젝트 클래스
/// 좌표가 필요없는 클래스는 씬이 아닌 별도의 시스템을 사용합니다.
/// </summary>
class ENGINE_DLL CGameObject abstract : public CBase, public ITransform
{
	DERIVED_CLASS(CBase, CGameObject)
	friend class CObjectMgr;

public:
	struct FInitGameObjectBase
	{

	};

protected:
	explicit CGameObject();
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	// 실제 게임에 추가되었을 때 작동하는 이벤트 함수. ObjectMgr에서 작동시킴
	virtual void OnCreate() {}
	// 실제 게임에서 죽어없어질 때 작동하는 이벤트 함수. ObjectMgr에서 작동시킴
	virtual void OnDead() {}

public:
	// 직렬화 데이터 뽑아낼 때 사용하는 함수. 각 오브젝트 별로 뽑아내는 데이터가 다릅니다.
	virtual FSerialData SerializeData_Prototype();
	virtual FSerialData SerializeData();

public:
	virtual CGameObject* Clone(void* Arg) PURE;
	virtual CGameObject* Clone(FSerialData& InputData) { return nullptr; }

protected:
	virtual void	Free() override;


#pragma region 기본 속성
public:
	GETSET_1(_uint, m_iID, ID, GET_C_REF)
	GETSET_2(wstring, m_strName, Name, GET_C_REF, SET_C_REF)

	void		Add_Tag(const EGObjTag eTagType, const wstring& strTag) { m_setTag[ECast(eTagType)].emplace(strTag); }
	_bool		Has_Tag(const EGObjTag eTagType, const wstring& strTag) const { return (m_setTag[ECast(eTagType)].find(strTag) != m_setTag[ECast(eTagType)].end()); }
	void		Delete_Tag(const EGObjTag eTagType, const wstring& strTag);
	_uint		Tag_Size(const EGObjTag eTagType) const { return Cast<_uint>(m_setTag[ECast(eTagType)].size()); }

	_bool		IsDead() const { return m_iStateFlag & ECast(EGObjectState::Dead); }
	void		Set_Dead() { m_iStateFlag |= ECast(EGObjectState::Dead); }

	_float		Get_Priority(_uint iIndex) const { return m_fPriority[iIndex]; }
	void		Set_Priority(_uint iIndex, _float fValue) { m_fPriority[iIndex] = fValue; }

	void		TurnOn_State(const EGObjectState value) { m_iStateFlag |= ECast(value); }
	void		TurnOff_State(const EGObjectState value) { m_iStateFlag &= ~ECast(value); }
	void		Toggle_State(const EGObjectState value) { m_iStateFlag ^= ECast(value); }
	_bool		IsState(const EGObjectState value) const { return (m_iStateFlag & ECast(value)); }

	void					Set_RenderGroup(const ERenderGroup eRenderGroup) { m_eRenderGroup = eRenderGroup; }
	const ERenderGroup&		Get_RenderGroup() const { return m_eRenderGroup; }

	GETSET_2(wstring, m_strPrototypeName, ProtoName, GET_C_REF, SET_C_REF)
	GETSET_2(_float, m_fCullRadius, CullRadius, GET_C_REF, SET_C_REF)

private:	// 기본 속성
	_uint				m_iID = 0U;							// 식별용 ID, 오브젝트 관리에 사용된다.
	wstring				m_strName = L"";					// 오브젝트 이름, 인스턴스를 식별하기 위해 사용된다.
	_uint				m_iStateFlag = 0U;					// 상태 플래그, 32가지의 상태를 구현한다. 시스템에서 쓰인다.
	_unset<wstring>		m_setTag[ECast(EGObjTag::Size)];	// 분류 태그. 시스템과 게임 로직에 사용된다.

	ERenderGroup		m_eRenderGroup = ERenderGroup::NonBlend;

	_float				m_fPriority[ECast(EGObjTickPriority::Size)] = {};	// 우선도

	wstring				m_strPrototypeName = TEXT("");					// 프로토타입 이름
	_float				m_fCullRadius = { 1.f };

public:		// 각 오브젝트는 자식 오브젝트를 가질 수 있음
	GETSET_2(CGameObject*, m_pOwner, Owner, GET_REF_C, SET__C)

private:
	CGameObject* m_pOwner = { nullptr };
#pragma endregion





#pragma region 컴포넌트
public:
	// 검색용
	HRESULT Add_Component(const wstring& strName, CGameObjectComp* pComponent);
	_uint	Get_NumComponents() { return m_iNumComponents; }
	template<class T, typename = enable_if_t<is_base_of_v<CGameObjectComp, T>>>
	T* Get_Component(const _uint iIndex) { return DynCast<T*>(m_vecComponents[iIndex]); }
	template<class T, typename = enable_if_t<is_base_of_v<CGameObjectComp, T>>>
	T* Get_Component(const wstring& strName);

public:		// 컴포넌트의 상태 변경시 자동으로 변경해주기 위한 이벤트 함수
	void OnStateUpdate_Updated(const CGameObjectComp* const pComp, const ECompTickAuto& bValue);
	void OnStateLateUpdate_Updated(const CGameObjectComp* const pComp, const ECompTickAuto& bValue);
	void OnStateRender_Updated(const CGameObjectComp* const pComp, const ECompTickAuto& bValue);

private:	// 컴포넌트 속성
	_uint								m_iNumComponents = { 0 };	// 컴포넌트 개수
	vector<CGameObjectComp*>			m_vecComponents;				// 컴포넌트 관리 컨테이너
	_unmap<wstring, CGameObjectComp*>	m_mapPrimComp;				// 오브젝트 전용 컴포넌트 관리 컨테이너

	using list_comp = list<CGameObjectComp*>;
	list_comp		  m_listUpdateComp[ECast(ECompTickType::Size)];	// 컴포넌트 업데이트 관리 리소스
#pragma endregion





#pragma region 트랜스폼 컴포넌트
	// 트랜스폼 컴포넌트에 대한 함수 정의
public:
	// 반드시 초기화가 되었을 때 사용해야 함.
	inline virtual CTransformComponent& Transform() override { return (*m_pTransformComp); }
	inline virtual void Release_Transform() override { Safe_Release(m_pTransformComp); }

private:	// 게임 오브젝트 기본 정의 컴포넌트
	CTransformComponent* m_pTransformComp = { nullptr };
#pragma endregion



#pragma region 캠뷰 컴포넌트
protected:
	CPipelineComp& PipelineComp() { return (*m_pPipelineComp); }

private:
	CPipelineComp* m_pPipelineComp = { nullptr };
#pragma endregion



};

END


template<class T, typename>
T* CGameObject::Get_Component(const wstring& strName)
{
	auto iter = find_if(m_vecComponents.begin(), m_vecComponents.end(),
		[&strName](CGameObjectComp* pComp) {
			return pComp->Get_Name() == strName;
		});

	if (iter != m_vecComponents.end())
	{
		return DynCast<T*>(*iter);
	}

	return nullptr;
}