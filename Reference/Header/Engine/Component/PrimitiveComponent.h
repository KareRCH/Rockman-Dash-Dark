#pragma once

#include "Base.h"
#include "BaseClass/GameObject_Define.h"
#include "Component/Component_Define.h"

BEGIN(Engine)

class CGameObject;

/// <summary>
/// ������Ʈ�� ����
/// </summary>
class ENGINE_DLL CPrimitiveComponent abstract : public CBase
{
	DERIVED_CLASS(CBase, CPrimitiveComponent)
protected:
	explicit CPrimitiveComponent(const DX11DEVICE_T tDevice);
	explicit CPrimitiveComponent(const CPrimitiveComponent& rhs);
	virtual ~CPrimitiveComponent() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CPrimitiveComponent*	Clone(void* Arg) PURE;

protected:
	virtual void					Free();

protected:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };		// ���� ��ġ
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext = { nullptr };	// ���� ��ġ ���ؽ�Ʈ
	_bool							m_bClone;

public:
	GETSET_1(wstring,	m_strName, Name, GET_C_REF)

private:	// �⺻ �Ӽ�
	wstring				m_strName;											// ������Ʈ �̸�
	_float				m_fPriority[Cast_EnumDef(ECompTickType::Size)];	// �켱��

public:
	// �ܺο����� ������ ���� �Ұ��� �������� �ּҸ� ����
	GETSET_2(CGameObject*, m_pOwner, Owner, GET_REF_C, SET__C)

private:
	CGameObject*		m_pOwner = nullptr;

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


