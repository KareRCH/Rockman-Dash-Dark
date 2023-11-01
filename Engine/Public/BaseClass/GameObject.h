#pragma once

#include "Base.h"
#include "Component/TransformComponent.h"

BEGIN(Engine)

class CPrimitiveComponent;

enum class EGOBJECT_STATE : _uint
{
	DEAD		= EBIT_FLAG32_0,
	PAUSE		= EBIT_FLAG32_1,
	RENDER		= EBIT_FLAG32_2,
};
using EGOBJ_STATE = EGOBJECT_STATE;

/// <summary>
/// ���� �߰��Ǿ� ���Ǵ� ��ǥ�� �⺻������ ž���ϴ� ������Ʈ Ŭ����
/// ��ǥ�� �ʿ���� Ŭ������ ���� �ƴ� ������ �ý����� ����մϴ�.
/// </summary>
class ENGINE_DLL CGameObject abstract : public CBase
{
	DERIVED_CLASS(CBase, CGameObject)
protected:
	explicit CGameObject(ID3D11Device* const pDevice);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render(ID3D11DeviceContext* const pDeviceContext);

protected:
	virtual void	Free();

public:
	GETSET_2(wstring, m_strName, Name, GET_C_REF, SET_C_REF)

	void		Add_Tag(const wstring& strTag) { m_setTag.emplace(strTag); }
	void		Delete_Tag(const wstring& strTag);

	_bool		IsDead() { return m_iStateFlag & Cast_EnumDef(EGOBJ_STATE::DEAD); }
	void		Set_Dead() { m_iStateFlag |= Cast_EnumDef(EGOBJ_STATE::DEAD); }
	
	
	_float		Get_Priority(_uint iIndex) { return m_fPriority[iIndex]; }

private:	// �⺻ �Ӽ�
	_uint_64					m_iID = 0ULL;			// �ĺ��� ID
	wstring						m_strName = L"";		// ������Ʈ �̸�
	_unset<wstring>				m_setTag;				// �з�, ����� �±�
	_uint						m_iStateFlag = 0U;		// ���� �÷���
	
	_float						m_fPriority[Cast_EnumDef(EUPDATE_T::SIZE)];	// �켱��

protected:
	ID3D11Device*				m_pDevice = nullptr;

public: // �� ������Ʈ�� �ڽ� ������Ʈ�� ���� �� ����
	GETSET_2(CGameObject*, m_pOwner, Owner, GET_REF_C, SET__C)

private:
	CGameObject* m_pOwner = nullptr;



#pragma region ������Ʈ

public:
	HRESULT Add_Component(const wstring& strName, CPrimitiveComponent* pComponent);
	CPrimitiveComponent* Get_Component(const _uint iIndex)
	{
		return m_vecComponent[iIndex];
	}
	CPrimitiveComponent* Get_Component(const wstring& strName);

private:
	HRESULT Initialize_Component();

public:		// ������Ʈ�� ���� ����� �ڵ����� �������ֱ� ���� �̺�Ʈ �Լ�
	void OnStateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue);
	void OnStateLateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue);
	void OnStateRender_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue);

private:	// ������Ʈ �Ӽ�
	vector<CPrimitiveComponent*>			m_vecComponent;				// ������Ʈ ���� �����̳�

	using list_comp =						list<CPrimitiveComponent*>;
	list_comp								m_listUpdateComp[Cast_EnumDef(EUPDATE_T::SIZE)];	// ������Ʈ ������Ʈ ���� ���ҽ�


public:		// Ʈ������ ������Ʈ�� ���� �Լ� ����
	const _float3	Get_Position() const { return m_pTransformComp->Get_Position(); }
	void		Set_Position(const _float3& value) { m_pTransformComp->Set_Position(value); }

	const _float3	Get_Rotation() const { return m_pTransformComp->Get_Rotation(); }
	void		Set_Rotation(const _float3& value) { m_pTransformComp->Set_Rotation(value); }

	const _float3	Get_Scale() const { return m_pTransformComp->Get_Scale(); }
	void		Set_Scale(const _float3& value) { m_pTransformComp->Set_Scale(value); }

	void		Calculate_Transform() { m_pTransformComp->Calculate_Transform(); }
	void		Calculate_TransformFromParent(const _matrix& matTransform) 
					{ m_pTransformComp->Calculate_TransformFromParent(matTransform); }
	const _matrix& Get_Transform() const { return m_pTransformComp->Get_Transform(); }

	const _float3 Get_RightFromTransform() const { return m_pTransformComp->Get_RightFromTransform(); }
	const _float3 Get_UpFromTransform() const { return m_pTransformComp->Get_UpFromTransform(); }
	const _float3 Get_LookFromTransform() const { return m_pTransformComp->Get_LookFromTransform(); }

private:	// ���� ������Ʈ �⺻ ���� ������Ʈ
	CTransformComponent*					m_pTransformComp = nullptr;

#pragma endregion

};

END