#pragma once

#include "Base.h"
#include "Component/TransformComponent.h"

BEGIN_NAME(Engine)

class CPrimitiveComponent;


/// <summary>
/// ���� �߰��Ǿ� ���Ǵ� ��ǥ�� �⺻������ ž���ϴ� ������Ʈ Ŭ����
/// ��ǥ�� �ʿ���� Ŭ������ ���� �ƴ� ������ �ý����� ����մϴ�.
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

private:	// �⺻ �Ӽ�
	_uint_64					m_iID = 0ULL;			// �ĺ��� ID
	wstring						m_strName = L"";		// ������Ʈ �̸�
	_unset<wstring>				m_setTag;				// �з�, ����� �±�
	_uint						m_iStateFlag = 0U;		// ���� �÷���
	
	_float						m_fPriority[Cast_EnumDef(EUPDATE_T::SIZE)];	// �켱��

public: // �� ������Ʈ�� �ڽ� ������Ʈ�� ���� �� ����
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

public:		// ������Ʈ�� ���� ����� �ڵ����� �������ֱ� ���� �̺�Ʈ �Լ�
	void OnStateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue);
	void OnStateLateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue);
	void OnStateRender_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue);

private:
	HRESULT Initialize_Component();

private:	// ������Ʈ �Ӽ�
	vector<CPrimitiveComponent*>			m_vecComponent;				// ������Ʈ ���� �����̳�

	using list_comp =						list<CPrimitiveComponent*>;
	list_comp								m_listUpdateComp[Cast_EnumDef(EUPDATE_T::SIZE)];	// ������Ʈ ������Ʈ ���� ���ҽ�


public:		// Ʈ������ ������Ʈ�� ���� �Լ� ����
	const _vec3	Get_Position() const { return m_pTransformComp->Get_Position(); }
	void		Set_Position(const _vec3& value) { m_pTransformComp->Set_Position(value); }

	const _vec3	Get_Rotation() const { return m_pTransformComp->Get_Rotation(); }
	void		Set_Rotation(const _vec3& value) { m_pTransformComp->Set_Rotation(value); }

	const _vec3	Get_Scale() const { return m_pTransformComp->Get_Scale(); }
	void		Set_Scale(const _vec3& value) { m_pTransformComp->Set_Scale(value); }

	void		Calculate_Transform() { m_pTransformComp->Calculate_Transform(); }
	void		Calculate_TransformFromParent(const _matrix& matTransform) 
					{ m_pTransformComp->Calculate_TransformFromParent(matTransform); }

private:	// ���� ������Ʈ �⺻ ���� ������Ʈ
	CTransformComponent*					m_pTransformComp = nullptr;
};

END_NAME