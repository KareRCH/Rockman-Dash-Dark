#pragma once

#include "Base.h"
#include "BaseClass/GameObject_Enum.h"
#include "Component/Component_Enum.h"

BEGIN(Engine)

class CGameObject;

/// <summary>
/// 컴포넌트의 원형
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
	ID3D11Device*			m_pDevice = nullptr;		// 렌더 장치
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;	// 렌더 장치 컨텍스트
	_bool					m_bClone;

public:
	GETSET_1(wstring,	m_strName, Name, GET_C_REF)

private:	// 기본 속성
	wstring				m_strName;										// 컴포넌트 이름
	_float				m_fPriority[Cast_EnumDef(EUPDATE_TYPE::SIZE)];	// 우선도

public:
	// 외부에서는 포인터 변경 불가를 조건으로 주소를 얻음
	GETSET_2(CGameObject*, m_pOwner, Owner, GET_REF_C, SET__C)

private:
	CGameObject*		m_pOwner = nullptr;

public:
	template<typename T, typename = enable_if_t<is_class<T>::value>>
	inline void Set_StateUpdate_Event(T* pObj, 
		function<void(T*, const CPrimitiveComponent* const, const ECOMP_UPDATE_TYPE&)> fn)
	{
		m_fnStateUpdate_Updated = [&fn, &pObj, this](const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_TYPE& bValue) {
			fn(pObj, this, bValue);
		};
	}

	template<typename T, typename = enable_if_t<is_class<T>::value>>
	inline void Set_StateLateUpdate_Event(T* pObj,
		function<void(T*, const CPrimitiveComponent* const, const ECOMP_UPDATE_TYPE&)> fn)
	{
		m_fnStateLateUpdate_Updated = [&fn, &pObj, this](const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_TYPE& bValue) {
			fn(pObj, this, bValue);
		};
	}

	template<typename T, typename = enable_if_t<is_class<T>::value>>
	inline void Set_StateRender_Event(T* pObj,
		function<void(T*, const CPrimitiveComponent* const, const ECOMP_UPDATE_TYPE&)> fn)
	{
		m_fnStateRender_Updated = [&fn, &pObj, this](const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_TYPE bValue) {
			fn(pObj, this, bValue);
		};
	}

private:
	function<void(const CPrimitiveComponent* const, const ECOMP_UPDATE_TYPE)>		m_fnStateUpdate_Updated;
	function<void(const CPrimitiveComponent* const, const ECOMP_UPDATE_TYPE)>		m_fnStateLateUpdate_Updated;
	function<void(const CPrimitiveComponent* const, const ECOMP_UPDATE_TYPE)>		m_fnStateRender_Updated;

public:
	inline void Set_StateUpdate(const ECOMP_UPDATE_TYPE value);
	inline void Set_StateLateUpdate(const ECOMP_UPDATE_TYPE value);
	inline void Set_StateRender(const ECOMP_UPDATE_TYPE value);

private:
	ECOMP_UPDATE_TYPE	m_eStateUpdate		= ECOMP_UPDATE_TYPE::MANUAL;
	ECOMP_UPDATE_TYPE	m_eStateLateUpdate	= ECOMP_UPDATE_TYPE::MANUAL;
	ECOMP_UPDATE_TYPE	m_eStateRender		= ECOMP_UPDATE_TYPE::MANUAL;
};



inline void CPrimitiveComponent::Set_StateUpdate(const ECOMP_UPDATE_TYPE value)
{
	m_eStateUpdate = value;

	if (m_fnStateUpdate_Updated)
		m_fnStateUpdate_Updated(this, m_eStateUpdate);
}

inline void CPrimitiveComponent::Set_StateLateUpdate(const ECOMP_UPDATE_TYPE value)
{
	m_eStateLateUpdate = value;

	if (m_fnStateLateUpdate_Updated)
		m_fnStateLateUpdate_Updated(this, m_eStateLateUpdate);
}

inline void CPrimitiveComponent::Set_StateRender(const ECOMP_UPDATE_TYPE value)
{
	m_eStateRender = value;

	if (m_fnStateRender_Updated)
		m_fnStateRender_Updated(this, m_eStateRender);
}

END


