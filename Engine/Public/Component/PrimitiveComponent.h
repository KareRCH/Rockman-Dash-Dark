#pragma once

#include "BaseClass/GameObject_Define.h"
#include "Component/Component_Define.h"
#include "Component/Component.h"
#include "Component/Interface/ID3D11DeviceComp.h"

BEGIN(Engine)

class CGameObject;

/// <summary>
/// 게임오브젝트에 추가할 수 있는 컴포넌트의 원형.
/// Primitive 타입은 Primitive전용 맵에 저장된다.
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

#pragma region 그래픽 디바이스
	/*
	* 그래픽 디바이스는 컴포넌트로 제공됩니다.
	* 그래픽 디바이스 컴포넌트는 돌려쓰기가 허용됩니다.
	* 객체 복사시 얕은 복사를 하여 씁니다. 이를 통해 메모리 상의 이점을 얻습니다.
	* 객체 유형이 다르면 그만큼 이 컴포넌트의 양도 많아집니다.
	* 그리 크게 신경쓸 사항은 아닙니다.
	* 이를 통해 그래픽 디바이스는 자동적으로 관리됨을 시사합니다.
	* 그래픽 디바이스 컴포넌트의 인터페이스가 제공되며, 이를 구현하여 디바이스를 얻어 쓰도록 만들어야 합니다.
	*/
public:
	inline virtual ID3D11Device* const D3D11Device() const { return m_pDeviceComp->Get_Device(); }
	inline virtual ID3D11DeviceContext* const D3D11Context() const { return m_pDeviceComp->Get_Context(); }

private:
	class CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion


#pragma region 기본 속성
public:
	_float Get_Priority(ECompTickType eType) { return m_fPriority[Cast_Enum(eType)]; }
	void Set_Priority(ECompTickType eType, _float fPriority) { m_fPriority[Cast_Enum(eType)] = fPriority; }

private:	// 기본 속성
	_float					m_fPriority[Cast_Enum(ECompTickType::Size)];		// 우선도
#pragma endregion


#pragma region 계층구조
public:
	// 외부에서는 포인터 변경 불가를 조건으로 주소를 얻음
	GETSET_2(CGameObject*, m_pOwnerObject, OwnerObject, GET_REF_C, SET__C)
	GETSET_2(CPrimitiveComponent*, m_pOwnerPrimComp, OwnerPrimComp, GET_REF_C, SET__C)

private:
	CGameObject*			m_pOwnerObject = { nullptr };			// 소유하고 있는 게임오브젝트, 이는 반드시 누군가 소유해주어야 함.
	CPrimitiveComponent*	m_pOwnerPrimComp = { nullptr };			// 소유하고 있는 Primitive 컴포넌트, 비어있을 수 있음

public:
	HRESULT Add_PrimComponent(const wstring& strCompKey, CPrimitiveComponent* pComp);	// Primitive 컴포넌트 추가
	CPrimitiveComponent* Find_PrimComponent(const wstring& strCompKey);					// Primitive 컴포넌트 추가

private:
	_unmap<wstring, CPrimitiveComponent*>	m_mapPrimComponent;		// Primitive 컴포넌트를 저장하는 컨테이너, CComponent는 저장이 안됩니다.
																	// Scene 컴포넌트를 저장시 동시에 저장됩니다.
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


