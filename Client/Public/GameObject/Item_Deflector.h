#pragma once

#include "Client_Define.h"
#include "GameObject/Character_Common.h"
#include "GameObject/GameObjectFactory.h"


BEGIN(Engine)
class CCommonModelComp;
END

BEGIN(Client)

/// <summary>
/// 돈 획득 아이템
/// </summary>
class CItem_Deflector : public CCollisionObject
{
	DERIVED_CLASS(CCollisionObject, CItem_Deflector)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::Item_Deflector);

protected:
	explicit CItem_Deflector();
	explicit CItem_Deflector(const CItem_Deflector& rhs);
	virtual ~CItem_Deflector() = default;

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
	static	CItem_Deflector* Create();
	static	CItem_Deflector* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr);
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();
	// 시리얼 데이터로 초기화시 사용
	HRESULT	Initialize_Component(FSerialData& InputData);

public:		// 충돌 이벤트
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);

private:
	CCommonModelComp* m_pModelComp = { nullptr };

private:
	_bool m_bIsOnGround = { false };

	_float3		m_vMaxMoveSpeed = {};					// 최대 움직임 속도
	_float3		m_vMoveSpeed = {};						// 움직임 속도
	_float3		m_vVelocity = {};						// 속도
	_float3		m_vAcceleration = {};					// 가속도

private:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

public:
	enum class EActionKey : _uint { MoveForward, MoveBackward, TurnRight, TurnLeft, Size };

private:
	ACTION_SET<EActionKey>	m_ActionKey;

public:
	enum class EState_Act { Idle, Ready_Floor, Flooring };

	void Register_State();
	void Update_Move(const _float& fTimeDelta);

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_Ready_Floor(const _float& fTimeDelta);
	void ActState_Flooring(const _float& fTimeDelta);

public:
	enum EType : _uint { White, Yellow, Green, Blue, Purple };

public:
	void Obtain_Money(_uint& iMoney);
	void Set_Type(EType eType) { m_eType = eType; };

private:
	_uint m_eType = { EType::White };
	_uint m_iGiveMoney = { 100 };
};

template <>
struct TObjectExtTrait<CItem_Deflector::g_ClassID>
{
	using Class = CItem_Deflector;
};

END