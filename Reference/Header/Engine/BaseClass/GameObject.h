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
/// ���� �߰��Ǿ� ���Ǵ� ��ǥ�� �⺻������ ž���ϴ� ������Ʈ Ŭ����
/// ��ǥ�� �ʿ���� Ŭ������ ���� �ƴ� ������ �ý����� ����մϴ�.
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
	// ���� ���ӿ� �߰��Ǿ��� �� �۵��ϴ� �̺�Ʈ �Լ�. ObjectMgr���� �۵���Ŵ
	virtual void OnCreate() {}
	// ���� ���ӿ��� �׾������ �� �۵��ϴ� �̺�Ʈ �Լ�. ObjectMgr���� �۵���Ŵ
	virtual void OnDead() {}

public:
	// ����ȭ ������ �̾Ƴ� �� ����ϴ� �Լ�. �� ������Ʈ ���� �̾Ƴ��� �����Ͱ� �ٸ��ϴ�.
	virtual FSerialData SerializeData_Prototype();
	virtual FSerialData SerializeData();

public:
	virtual CGameObject* Clone(void* Arg) PURE;
	virtual CGameObject* Clone(FSerialData& InputData) { return nullptr; }

protected:
	virtual void	Free() override;


#pragma region �⺻ �Ӽ�
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

private:	// �⺻ �Ӽ�
	_uint				m_iID = 0U;							// �ĺ��� ID, ������Ʈ ������ ���ȴ�.
	wstring				m_strName = L"";					// ������Ʈ �̸�, �ν��Ͻ��� �ĺ��ϱ� ���� ���ȴ�.
	_uint				m_iStateFlag = 0U;					// ���� �÷���, 32������ ���¸� �����Ѵ�. �ý��ۿ��� ���δ�.
	_unset<wstring>		m_setTag[ECast(EGObjTag::Size)];	// �з� �±�. �ý��۰� ���� ������ ���ȴ�.

	ERenderGroup		m_eRenderGroup = ERenderGroup::NonBlend;

	_float				m_fPriority[ECast(EGObjTickPriority::Size)] = {};	// �켱��

	wstring				m_strPrototypeName = TEXT("");					// ������Ÿ�� �̸�
	_float				m_fCullRadius = { 1.f };

public:		// �� ������Ʈ�� �ڽ� ������Ʈ�� ���� �� ����
	GETSET_2(CGameObject*, m_pOwner, Owner, GET_REF_C, SET__C)

private:
	CGameObject* m_pOwner = { nullptr };
#pragma endregion





#pragma region ������Ʈ
public:
	// �˻���
	HRESULT Add_Component(const wstring& strName, CGameObjectComp* pComponent);
	_uint	Get_NumComponents() { return m_iNumComponents; }
	template<class T, typename = enable_if_t<is_base_of_v<CGameObjectComp, T>>>
	T* Get_Component(const _uint iIndex) { return DynCast<T*>(m_vecComponents[iIndex]); }
	template<class T, typename = enable_if_t<is_base_of_v<CGameObjectComp, T>>>
	T* Get_Component(const wstring& strName);

public:		// ������Ʈ�� ���� ����� �ڵ����� �������ֱ� ���� �̺�Ʈ �Լ�
	void OnStateUpdate_Updated(const CGameObjectComp* const pComp, const ECompTickAuto& bValue);
	void OnStateLateUpdate_Updated(const CGameObjectComp* const pComp, const ECompTickAuto& bValue);
	void OnStateRender_Updated(const CGameObjectComp* const pComp, const ECompTickAuto& bValue);

private:	// ������Ʈ �Ӽ�
	_uint								m_iNumComponents = { 0 };	// ������Ʈ ����
	vector<CGameObjectComp*>			m_vecComponents;				// ������Ʈ ���� �����̳�
	_unmap<wstring, CGameObjectComp*>	m_mapPrimComp;				// ������Ʈ ���� ������Ʈ ���� �����̳�

	using list_comp = list<CGameObjectComp*>;
	list_comp		  m_listUpdateComp[ECast(ECompTickType::Size)];	// ������Ʈ ������Ʈ ���� ���ҽ�
#pragma endregion





#pragma region Ʈ������ ������Ʈ
	// Ʈ������ ������Ʈ�� ���� �Լ� ����
public:
	// �ݵ�� �ʱ�ȭ�� �Ǿ��� �� ����ؾ� ��.
	inline virtual CTransformComponent& Transform() override { return (*m_pTransformComp); }
	inline virtual void Release_Transform() override { Safe_Release(m_pTransformComp); }

private:	// ���� ������Ʈ �⺻ ���� ������Ʈ
	CTransformComponent* m_pTransformComp = { nullptr };
#pragma endregion



#pragma region ķ�� ������Ʈ
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