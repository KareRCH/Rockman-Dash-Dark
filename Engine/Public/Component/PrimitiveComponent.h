#pragma once

#include "BaseClass/GameObject_Define.h"
#include "Component/Component_Define.h"
#include "Component/Component.h"
#include "Component/Interface/ID3D11DeviceComp.h"

BEGIN(Engine)

class CGameObject;

/// <summary>
/// ���ӿ�����Ʈ�� �߰��� �� �ִ� ������Ʈ�� ����.
/// Primitive Ÿ���� Primitive���� �ʿ� ����ȴ�.
/// 
/// </summary>
class ENGINE_DLL CPrimitiveComponent abstract : public CComponent, public ID3D11DeviceComp
{
	DERIVED_CLASS(CComponent, CPrimitiveComponent)
protected:
	explicit CPrimitiveComponent() = default;
	explicit CPrimitiveComponent(const CPrimitiveComponent& rhs);
	virtual ~CPrimitiveComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr);
	virtual HRESULT Initialize(void* Arg = nullptr) PURE;
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CComponent*	Clone(void* Arg = nullptr) PURE;

protected:
	virtual void		Free();

#pragma region �׷��� ����̽�
	/*
	* �׷��� ����̽��� ������Ʈ�� �����˴ϴ�.
	* �׷��� ����̽� ������Ʈ�� �������Ⱑ ���˴ϴ�.
	* ��ü ����� ���� ���縦 �Ͽ� ���ϴ�. �̸� ���� �޸� ���� ������ ����ϴ�.
	* ��ü ������ �ٸ��� �׸�ŭ �� ������Ʈ�� �絵 �������ϴ�.
	* �׸� ũ�� �Ű澵 ������ �ƴմϴ�.
	* �̸� ���� �׷��� ����̽��� �ڵ������� �������� �û��մϴ�.
	* �׷��� ����̽� ������Ʈ�� �������̽��� �����Ǹ�, �̸� �����Ͽ� ����̽��� ��� ������ ������ �մϴ�.
	*/
public:
	inline virtual ID3D11Device* const D3D11Device() const { return m_pDeviceComp->Get_Device(); }
	inline virtual ID3D11DeviceContext* const D3D11Context() const { return m_pDeviceComp->Get_Context(); }

private:
	class CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion


#pragma region �⺻ �Ӽ�
public:
	_float Get_Priority(ECompTickType eType) { return m_fPriority[Cast_Enum(eType)]; }
	void Set_Priority(ECompTickType eType, _float fPriority) { m_fPriority[Cast_Enum(eType)] = fPriority; }

private:	// �⺻ �Ӽ�
	_float					m_fPriority[Cast_Enum(ECompTickType::Size)];		// �켱��
#pragma endregion


#pragma region ��������
public:
	// �ܺο����� ������ ���� �Ұ��� �������� �ּҸ� ����
	GETSET_2(CGameObject*, m_pOwnerObject, OwnerObject, GET_REF_C, SET__C)
	GETSET_2(CPrimitiveComponent*, m_pOwnerPrimComp, OwnerPrimComp, GET_REF_C, SET__C)

private:
	CGameObject*			m_pOwnerObject = { nullptr };			// �����ϰ� �ִ� ���ӿ�����Ʈ, �̴� �ݵ�� ������ �������־�� ��.
	CPrimitiveComponent*	m_pOwnerPrimComp = { nullptr };			// �����ϰ� �ִ� Primitive ������Ʈ, ������� �� ����

public:
	HRESULT Add_PrimComponent(const wstring& strCompKey, CPrimitiveComponent* pComp);	// Primitive ������Ʈ �߰�
	CPrimitiveComponent* Find_PrimComponent(const wstring& strCompKey);					// Primitive ������Ʈ �߰�

private:
	_unmap<wstring, CPrimitiveComponent*>	m_mapPrimComponent;		// Primitive ������Ʈ�� �����ϴ� �����̳�, CComponent�� ������ �ȵ˴ϴ�.
																	// Scene ������Ʈ�� ����� ���ÿ� ����˴ϴ�.
#pragma endregion


public:
	template<typename T, typename = enable_if_t<is_class<T>::value>>
	inline void Set_StateUpdate_Event(T* pObj, 
		function<void(T*, const CPrimitiveComponent* const, const ECompTickAuto&)> fn)
	{
		m_fnStateUpdate_Updated = [&fn, &pObj, this](const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue) {
			fn(pObj, this, bValue);
		};
	}

	template<typename T, typename = enable_if_t<is_class<T>::value>>
	inline void Set_StateLateUpdate_Event(T* pObj,
		function<void(T*, const CPrimitiveComponent* const, const ECompTickAuto&)> fn)
	{
		m_fnStateLateUpdate_Updated = [&fn, &pObj, this](const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue) {
			fn(pObj, this, bValue);
		};
	}

	template<typename T, typename = enable_if_t<is_class<T>::value>>
	inline void Set_StateRender_Event(T* pObj,
		function<void(T*, const CPrimitiveComponent* const, const ECompTickAuto&)> fn)
	{
		m_fnStateRender_Updated = [&fn, &pObj, this](const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue) {
			fn(pObj, this, bValue);
		};
	}

private:
	function<void(const CPrimitiveComponent* const, const ECompTickAuto)>		m_fnStateUpdate_Updated;
	function<void(const CPrimitiveComponent* const, const ECompTickAuto)>		m_fnStateLateUpdate_Updated;
	function<void(const CPrimitiveComponent* const, const ECompTickAuto)>		m_fnStateRender_Updated;

public:
	inline void Set_StateTick(const ECompTickAuto value);
	inline void Set_StateLateTick(const ECompTickAuto value);
	inline void Set_StateRender(const ECompTickAuto value);

private:
	ECompTickAuto	m_eStatePriorityTick		= ECompTickAuto::Manual;
	ECompTickAuto	m_eStateTick				= ECompTickAuto::Manual;
	ECompTickAuto	m_eStateLateTick			= ECompTickAuto::Manual;
	ECompTickAuto	m_eStateRender				= ECompTickAuto::Manual;
};



inline void CPrimitiveComponent::Set_StateTick(const ECompTickAuto value)
{
	m_eStateTick = value;

	if (m_fnStateUpdate_Updated)
		m_fnStateUpdate_Updated(this, m_eStateTick);
}

inline void CPrimitiveComponent::Set_StateLateTick(const ECompTickAuto value)
{
	m_eStateLateTick = value;

	if (m_fnStateLateUpdate_Updated)
		m_fnStateLateUpdate_Updated(this, m_eStateLateTick);
}

inline void CPrimitiveComponent::Set_StateRender(const ECompTickAuto value)
{
	m_eStateRender = value;

	if (m_fnStateRender_Updated)
		m_fnStateRender_Updated(this, m_eStateRender);
}

END


