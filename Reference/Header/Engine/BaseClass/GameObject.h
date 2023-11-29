#pragma once

#include "Base.h"
#include "Component/TransformComponent.h"
#include "Component/CamViewComp.h"

#include "Component/Define/Component_Define.h"
#include "BaseClass/GameObject_Define.h"

BEGIN(Engine)

class CPrimitiveComponent;

/// <summary>
/// ���� �߰��Ǿ� ���Ǵ� ��ǥ�� �⺻������ ž���ϴ� ������Ʈ Ŭ����
/// ��ǥ�� �ʿ���� Ŭ������ ���� �ƴ� ������ �ý����� ����մϴ�.
/// </summary>
class ENGINE_DLL CGameObject abstract : public CBase, public ITransform
{
	DERIVED_CLASS(CBase, CGameObject)
	friend class CObjectMgr;
protected:
	explicit CGameObject() = default;
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	virtual CGameObject* Clone(void* Arg) PURE;

protected:
	virtual void	Free() override;





#pragma region �⺻ �Ӽ�
public:
	GETSET_1(_uint, m_iID, ID, GET_C_REF)
	GETSET_2(wstring, m_strName, Name, GET_C_REF, SET_C_REF)

	void		Add_Tag(const EGObjTag eTagType, const wstring& strTag) { m_setTag[ECast(eTagType)].emplace(strTag); }
	_bool		Has_Tag(const EGObjTag eTagType, const wstring& strTag) { return (m_setTag[ECast(eTagType)].find(strTag) != m_setTag[ECast(eTagType)].end()); }
	void		Delete_Tag(const EGObjTag eTagType, const wstring& strTag);
	_uint		Tag_Size(const EGObjTag eTagType) { return Cast<_uint>(m_setTag[ECast(eTagType)].size()); }

	_bool		IsDead() { return m_iStateFlag & ECast(EGObjectState::Dead); }
	void		Set_Dead() { m_iStateFlag |= ECast(EGObjectState::Dead); }

	_float		Get_Priority(_uint iIndex) { return m_fPriority[iIndex]; }

	void		TurnOn_State(const EGObjectState value) { m_iStateFlag |= ECast(value); }
	void		TurnOff_State(const EGObjectState value) { m_iStateFlag &= ~ECast(value); }
	void		Toggle_State(const EGObjectState value) { m_iStateFlag ^= ECast(value); }
	_bool		IsState(const EGObjectState value) { return (m_iStateFlag & ECast(value)); }

private:	// �⺻ �Ӽ�
	_uint				m_iID = 0U;							// �ĺ��� ID, ������Ʈ ������ ���ȴ�.
	wstring				m_strName = L"";					// ������Ʈ �̸�, �ν��Ͻ��� �ĺ��ϱ� ���� ���ȴ�.
	_uint				m_iStateFlag = 0U;					// ���� �÷���, 32������ ���¸� �����Ѵ�. �ý��ۿ��� ���δ�.
	_unset<wstring>		m_setTag[ECast(EGObjTag::Size)];	// �з� �±�. �ý��۰� ���� ������ ���ȴ�.


	_float				m_fPriority[ECast(EGObjTickPriority::Size)];	// �켱��

public: // �� ������Ʈ�� �ڽ� ������Ʈ�� ���� �� ����
	GETSET_2(CGameObject*, m_pOwner, Owner, GET_REF_C, SET__C)

private:
	CGameObject* m_pOwner = { nullptr };
#pragma endregion





#pragma region ������Ʈ
public:
	// �˻���
	HRESULT Add_Component(const wstring& strName, CPrimitiveComponent* pComponent);
	CPrimitiveComponent* Get_Component(const _uint iIndex)
	{
		return m_vecComponent[iIndex];
	}
	CPrimitiveComponent* Get_Component(const wstring& strName);

public:		// ������Ʈ�� ���� ����� �ڵ����� �������ֱ� ���� �̺�Ʈ �Լ�
	void OnStateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue);
	void OnStateLateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue);
	void OnStateRender_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue);

private:	// ������Ʈ �Ӽ�
	vector<CPrimitiveComponent*>			m_vecComponent;				// ������Ʈ ���� �����̳�
	_unmap<wstring, CPrimitiveComponent*>	m_mapPrimComp;				// ������Ʈ ���� ������Ʈ ���� �����̳�

	using list_comp =						list<CPrimitiveComponent*>;
	list_comp								m_listUpdateComp[ECast(ECompTickType::Size)];	// ������Ʈ ������Ʈ ���� ���ҽ�
#pragma endregion





#pragma region Ʈ������ ������Ʈ
	// Ʈ������ ������Ʈ�� ���� �Լ� ����
public:
	// �ݵ�� �ʱ�ȭ�� �Ǿ��� �� ����ؾ� ��.
	inline virtual CTransformComponent& Transform() override { return (*m_pTransformComp); }
	inline virtual void Release_Transform() override { Safe_Release(m_pTransformComp); }

private:	// ���� ������Ʈ �⺻ ���� ������Ʈ
	CTransformComponent* m_pTransformComp = nullptr;
#pragma endregion



#pragma region ķ�� ������Ʈ
protected:
	CCamViewComp& CamViewComp() { return (*m_pCamViewComp); }

private:
	CCamViewComp* m_pCamViewComp = { nullptr };
#pragma endregion



};

END