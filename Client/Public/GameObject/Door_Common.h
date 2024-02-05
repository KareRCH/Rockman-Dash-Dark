#pragma once

#include "Client_Define.h"
#include "BaseClass/CollisionObject.h"

#include "GameObject/GameObjectFactory.h"
#include "Utility/LogicDeviceBasic.h"


BEGIN(Engine)
class CCommonModelComp;
END

BEGIN(Client)

class CDoor_Common : public CCollisionObject
{
	DERIVED_CLASS(CCollisionObject, CDoor_Common)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::Door_Common);

protected:
	explicit CDoor_Common();
	explicit CDoor_Common(const CDoor_Common& rhs);
	virtual ~CDoor_Common() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	virtual void BeginPlay() override;

public:
	static CDoor_Common* Create();
	static CDoor_Common* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr);
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void			Free() override;

protected:
protected:
	// 프로토타입 제작용 함수
	virtual FSerialData SerializeData_Prototype();
	// 클로닝 전용 함수
	virtual FSerialData SerializeData();

protected:
	HRESULT Initialize_Component();
	HRESULT Initialize_Component(FSerialData& InputData);


protected: // 충돌
	virtual void	OnCollision(CGameObject* pDst, const FContact* const pContact);
	virtual void	OnCollisionEntered(CGameObject* pDst, const FContact* const pContact);
	virtual void	OnCollisionExited(CGameObject* pDst);

private:
	CCommonModelComp* m_pModelComp = { nullptr };

public:
	// 다른 오브젝트가 이 문을 열려고 할 때 쓰는 것.
	void OpenDoor();

public:
	enum class EActionKey : _uint { Open, Close, Size };

private:
	ACTION_SET<EActionKey>	m_ActionKey;
	_bool					m_bCanControl = { true };

public:
	enum class EState_Act { Idle, Open, Close };

	void Register_State();

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

	FGauge			m_fDoorClose = FGauge(3.f);

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_Open(const _float& fTimeDelta);
	void ActState_Close(const _float& fTimeDelta);

private:
	void TransitionLevel();

private:
	wstring m_strTransitionLevel = { TEXT("") };	// 옮겨갈 레벨
	_float3 m_vStartPos = {};						// 시작시 위치
	_float3 m_vStartLook = { 0.f, 0.f, 1.f };		// 시작시 보는 방향
	_bool	m_bIsTransitioning = { false };

};

template <>
struct TObjectExtTrait<CDoor_Common::g_ClassID>
{
	using Class = CDoor_Common;
};

END