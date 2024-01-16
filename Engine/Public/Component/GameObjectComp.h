#pragma once

#include "Component/Component.h"

#include "Component/D3D11DeviceComp.h"
#include "BaseClass/GameObject_Define.h"
#include "Component/Define/Component_Define.h"

#include "Utility/RapidJsonSerial.h"

BEGIN(Engine)

class CGameObject;

/// <summary>
/// ��ǥ����� ���� ������Ʈ Ŭ����
/// ���� ��ǥ����� �ʿ� ���� �Ļ�Ŭ������ ����մϴ�.
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
	// ������Ÿ�� ���ۿ� �Լ�
	virtual FSerialData SerializeData_Prototype();
	// Ŭ�δ� ���� �Լ�
	virtual FSerialData SerializeData();

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void Free() override;

public:
	GETSET_2(wstring, m_strPrototypeName, ProtoName, GET_C_REF, SET_C_REF)

protected:
	wstring	m_strPrototypeName = TEXT("");		// ������Ÿ�� ����� ���

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
protected:
	virtual ID3D11Device* const D3D11Device() const { return m_pDeviceComp->Get_Device(); }
	virtual ID3D11DeviceContext* const D3D11Context() const { return m_pDeviceComp->Get_Context(); }
	CD3D11DeviceComp& DeviceComp() const { return *m_pDeviceComp; }

private:
	class CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion

#pragma region �⺻ �Ӽ�
public:
	_float Get_Priority(ECompTickType eType) { return m_fPriority[ECast(eType)]; }
	void Set_Priority(ECompTickType eType, _float fPriority) { m_fPriority[ECast(eType)] = fPriority; }

private:	// �⺻ �Ӽ�
	_float					m_fPriority[ECast(ECompTickType::Size)];		// �켱��
#pragma endregion


#pragma region ��������
public:
	// �ܺο����� ������ ���� �Ұ��� �������� �ּҸ� ����
	GETSET_2(CGameObject*, m_pOwnerObject, OwnerObject, GET_REF_C, SET__C)
		GETSET_2(CGameObjectComp*, m_pOwnerPrimComp, OwnerPrimComp, GET_REF_C, SET__C)

private:
	CGameObject* m_pOwnerObject = { nullptr };			// �����ϰ� �ִ� ���ӿ�����Ʈ, �̴� �ݵ�� ������ �������־�� ��.
	CGameObjectComp* m_pOwnerPrimComp = { nullptr };			// �����ϰ� �ִ� Primitive ������Ʈ, ������� �� ����

public:
	HRESULT Add_PrimComponent(const wstring& strCompKey, CGameObjectComp* pComp);	// Primitive ������Ʈ �߰�
	CGameObjectComp* Find_PrimComponent(const wstring& strCompKey);					// Primitive ������Ʈ �߰�

private:
	_unmap<wstring, CGameObjectComp*>	m_mapPrimComponent;		// Primitive ������Ʈ�� �����ϴ� �����̳�, CComponent�� ������ �ȵ˴ϴ�.
	// Scene ������Ʈ�� ����� ���ÿ� ����˴ϴ�.
#pragma endregion



#pragma region �ڵ� ȣ�� ����
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