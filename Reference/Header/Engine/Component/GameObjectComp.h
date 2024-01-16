#pragma once

#include "Component/Component.h"

#include "Component/D3D11DeviceComp.h"
#include "BaseClass/GameObject_Define.h"
#include "Component/Define/Component_Define.h"

#include "Utility/RapidJsonSerial.h"

BEGIN(Engine)

class CGameObject;

/// <summary>
/// 좌표기능이 빠진 컴포넌트 클래스
/// 로컬 좌표기능이 필요 없는 파생클래스로 사용합니다.
/// </summary>
class ENGINE_DLL CGameObjectComp abstract : public CComponent, public ID3D11DeviceComp
{
	DERIVED_CLASS(CComponent, CGameObjectComp)
protected:
	explicit CGameObjectComp();
	explicit CGameObjectComp(const CGameObjectComp& rhs);
	virtual ~CGameObjectComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr);
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr) PURE;
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual void	Tick(const _float& fTimeDelta) PURE;
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual HRESULT	Render() PURE;

public:
	// 프로토타입 제작용 함수
	virtual FSerialData SerializeData_Prototype();
	// 클로닝 전용 함수
	virtual FSerialData SerializeData();

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void Free() override;

public:
	GETSET_2(wstring, m_strPrototypeName, ProtoName, GET_C_REF, SET_C_REF)

protected:
	wstring	m_strPrototypeName = TEXT("");		// 프로토타입 저장시 사용

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
protected:
	virtual ID3D11Device* const D3D11Device() const { return m_pDeviceComp->Get_Device(); }
	virtual ID3D11DeviceContext* const D3D11Context() const { return m_pDeviceComp->Get_Context(); }
	CD3D11DeviceComp& DeviceComp() const { return *m_pDeviceComp; }

private:
	class CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion

#pragma region 기본 속성
public:
	_float Get_Priority(ECompTickType eType) { return m_fPriority[ECast(eType)]; }
	void Set_Priority(ECompTickType eType, _float fPriority) { m_fPriority[ECast(eType)] = fPriority; }

private:	// 기본 속성
	_float					m_fPriority[ECast(ECompTickType::Size)];		// 우선도
#pragma endregion


#pragma region 계층구조
public:
	// 외부에서는 포인터 변경 불가를 조건으로 주소를 얻음
	GETSET_2(CGameObject*, m_pOwnerObject, OwnerObject, GET_REF_C, SET__C)
		GETSET_2(CGameObjectComp*, m_pOwnerPrimComp, OwnerPrimComp, GET_REF_C, SET__C)

private:
	CGameObject* m_pOwnerObject = { nullptr };			// 소유하고 있는 게임오브젝트, 이는 반드시 누군가 소유해주어야 함.
	CGameObjectComp* m_pOwnerPrimComp = { nullptr };			// 소유하고 있는 Primitive 컴포넌트, 비어있을 수 있음

public:
	HRESULT Add_PrimComponent(const wstring& strCompKey, CGameObjectComp* pComp);	// Primitive 컴포넌트 추가
	CGameObjectComp* Find_PrimComponent(const wstring& strCompKey);					// Primitive 컴포넌트 추가

private:
	_unmap<wstring, CGameObjectComp*>	m_mapPrimComponent;		// Primitive 컴포넌트를 저장하는 컨테이너, CComponent는 저장이 안됩니다.
	// Scene 컴포넌트를 저장시 동시에 저장됩니다.
#pragma endregion



#pragma region 자동 호출 관련
public:
	template<typename T, typename = enable_if_t<is_class<T>::value>>
	inline void Set_StateUpdate_Event(T* pObj,
		function<void(T*, const CGameObjectComp* const, const ECompTickAuto&)> fn)
	{
		m_fnStateUpdate_Updated = [&fn, &pObj, this](const CGameObjectComp* const pComp, const ECompTickAuto& bValue) {
			fn(pObj, this, bValue);
		};
	}

	template<typename T, typename = enable_if_t<is_class<T>::value>>
	inline void Set_StateLateUpdate_Event(T* pObj,
		function<void(T*, const CGameObjectComp* const, const ECompTickAuto&)> fn)
	{
		m_fnStateLateUpdate_Updated = [&fn, &pObj, this](const CGameObjectComp* const pComp, const ECompTickAuto& bValue) {
			fn(pObj, this, bValue);
		};
	}

	template<typename T, typename = enable_if_t<is_class<T>::value>>
	inline void Set_StateRender_Event(T* pObj,
		function<void(T*, const CGameObjectComp* const, const ECompTickAuto&)> fn)
	{
		m_fnStateRender_Updated = [&fn, &pObj, this](const CGameObjectComp* const pComp, const ECompTickAuto& bValue) {
			fn(pObj, this, bValue);
		};
	}

private:
	function<void(const CGameObjectComp* const, const ECompTickAuto)>		m_fnStateUpdate_Updated;
	function<void(const CGameObjectComp* const, const ECompTickAuto)>		m_fnStateLateUpdate_Updated;
	function<void(const CGameObjectComp* const, const ECompTickAuto)>		m_fnStateRender_Updated;

public:
	inline void Set_StateTick(const ECompTickAuto value);
	inline void Set_StateLateTick(const ECompTickAuto value);
	inline void Set_StateRender(const ECompTickAuto value);

private:
	ECompTickAuto	m_eStatePriorityTick = ECompTickAuto::Manual;
	ECompTickAuto	m_eStateTick = ECompTickAuto::Manual;
	ECompTickAuto	m_eStateLateTick = ECompTickAuto::Manual;
	ECompTickAuto	m_eStateRender = ECompTickAuto::Manual;
#pragma endregion




};


inline void CGameObjectComp::Set_StateTick(const ECompTickAuto value)
{
	m_eStateTick = value;

	if (m_fnStateUpdate_Updated)
		m_fnStateUpdate_Updated(this, m_eStateTick);
}

inline void CGameObjectComp::Set_StateLateTick(const ECompTickAuto value)
{
	m_eStateLateTick = value;

	if (m_fnStateLateUpdate_Updated)
		m_fnStateLateUpdate_Updated(this, m_eStateLateTick);
}

inline void CGameObjectComp::Set_StateRender(const ECompTickAuto value)
{
	m_eStateRender = value;

	if (m_fnStateRender_Updated)
		m_fnStateRender_Updated(this, m_eStateRender);
}

END