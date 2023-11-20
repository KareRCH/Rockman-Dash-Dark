#pragma once

#include "BaseClass/GameObject_Define.h"
#include "Component/Component_Define.h"
#include "Component/Component.h"

BEGIN(Engine)

class CGameObject;

/// <summary>
/// 게임오브젝트에 추가할 수 있는 컴포넌트의 원형.
/// Primitive 타입은 Primitive전용 맵에 저장된다.
/// 
/// </summary>
class ENGINE_DLL CPrimitiveComponent abstract : public CComponent
{
	DERIVED_CLASS(CBase, CPrimitiveComponent)
protected:
	explicit CPrimitiveComponent(const DX11DEVICE_T tDevice);
	explicit CPrimitiveComponent(const CPrimitiveComponent& rhs);
	virtual ~CPrimitiveComponent() = default;

public:
	virtual HRESULT Initialize(void* Arg = nullptr) PURE;
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CComponent*	Clone(void* Arg = nullptr) PURE;

protected:
	virtual void					Free();

protected:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };		// 렌더 장치
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext = { nullptr };	// 렌더 장치 컨텍스트
	_bool							m_bClone;

private:	// 기본 속성
	_float				m_fPriority[Cast_EnumDef(ECompTickType::Size)];		// 우선도

public:
	// 외부에서는 포인터 변경 불가를 조건으로 주소를 얻음
	GETSET_2(CGameObject*, m_pOwnerObject, OwnerObject, GET_REF_C, SET__C)
	GETSET_2(CPrimitiveComponent*, m_pOwnerPrimComp, OwnerPrimComp, GET_REF_C, SET__C)

private:
	CGameObject*			m_pOwnerObject = { nullptr };			// 소유하고 있는 게임오브젝트
	CPrimitiveComponent*	m_pOwnerPrimComp = { nullptr };			// 부모 컴포넌트

public:
	HRESULT Add_PrimComponent(const wstring& strCompKey, CPrimitiveComponent* pComp);
	CPrimitiveComponent* Find_PrimComponent(const wstring& strCompKey);

private:
	_unmap<wstring, CPrimitiveComponent*>	m_mapPrimComponent;		// 원시 컴포넌트를 저장하는 맵

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


