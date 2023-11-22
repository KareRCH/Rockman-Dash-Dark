#pragma once

#include "Base.h"
#include "Component/TransformComponent.h"
#include "Component/Component_Define.h"
#include "Component/Interface/ITransform.h"

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

private:	// �⺻ �Ӽ�
	_uint_64					m_iID = 0ULL;			// �ĺ��� ID
	wstring						m_strName = L"";		// ������Ʈ �̸�
	_unset<wstring>				m_setTag;				// �з�, ����� �±�
	_uint						m_iStateFlag = 0U;		// ���� �÷���
	
	_float						m_fPriority[Cast_EnumDef(EGObjTickPriority::Size)];	// �켱��

public: // �� ������Ʈ�� �ڽ� ������Ʈ�� ���� �� ����
	GETSET_2(CGameObject*, m_pOwner, Owner, GET_REF_C, SET__C)

private:
	CGameObject* m_pOwner = nullptr;

#pragma region ������Ʈ
public:
	// �˻���
	HRESULT Add_Component(const wstring& strName, CPrimitiveComponent* pComponent);
	CPrimitiveComponent* Get_Component(const _uint iIndex)
	{
		return m_vecComponent[iIndex];
	}
	CPrimitiveComponent* Get_Component(const wstring& strName);

private:
	HRESULT Initialize_Component();

public:		// ������Ʈ�� ���� ����� �ڵ����� �������ֱ� ���� �̺�Ʈ �Լ�
	void OnStateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue);
	void OnStateLateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue);
	void OnStateRender_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue);

private:	// ������Ʈ �Ӽ�
	vector<CPrimitiveComponent*>			m_vecComponent;				// ������Ʈ ���� �����̳�
	_unmap<wstring, CPrimitiveComponent*>	m_mapPrimComp;				// ������Ʈ ���� ������Ʈ ���� �����̳�

	using list_comp =						list<CPrimitiveComponent*>;
	list_comp								m_listUpdateComp[Cast_EnumDef(ECompTickType::Size)];	// ������Ʈ ������Ʈ ���� ���ҽ�
#pragma endregion

#pragma region Ʈ������ ������Ʈ
public:		// Ʈ������ ������Ʈ�� ���� �Լ� ����
	// �ݵ�� �ʱ�ȭ�� �Ǿ��� �� ����ؾ� ��.
	inline virtual CTransformComponent& Transform() override { return (*m_pTransformComp); }
	inline virtual void Release_Transform() override { Safe_Release(m_pTransformComp); }

private:	// ���� ������Ʈ �⺻ ���� ������Ʈ
	CTransformComponent* m_pTransformComp = nullptr;
#pragma endregion



};

END