#pragma once

#include "Client_Define.h"
#include "BaseClass/CollisionObject.h"
#include "GameObject/GameObjectFactory.h"
#include "Utility/LogicDeviceBasic.h"

BEGIN(Engine)
class CCommonModelComp;
END


BEGIN(Client)

class CPillarTrap : public CCollisionObject
{
	DERIVED_CLASS(CCollisionObject, CPillarTrap)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::PillarTrap);

protected:
	explicit CPillarTrap();
	explicit CPillarTrap(const CPillarTrap& rhs);
	virtual ~CPillarTrap() = default;

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
	static CPillarTrap* Create();
	static CPillarTrap* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr);
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();
	HRESULT	Initialize_Component(FSerialData& InputData);

public:		// �浹 �̺�Ʈ
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);

private:
	CCommonModelComp* m_pModelComp = { nullptr };


public:
	// Ʈ�� Ÿ�Կ� ���� �������� �ٸ�
	enum class ETrapType : _uint { Linear, Circle, Rect };

public:
	void MoveUpdate(const _float& fTimeDelta);

private:
	ETrapType	m_eTrapType = { ETrapType::Linear };
	_float		m_fRadiusRange = { 5.f };				// Ʈ���� �����̴� ���� �ݰ�
	_uint		m_iCurrentMoveIndex = { 0 };
	vector<_float3> m_vMovePoints;
	FGauge		m_fLerp = FGauge(1.f);
	_float		m_fMoveSpeed = { 4.f };					// ������ �ӵ�
	_float3		m_vOriginPos = {};
};

template <>
struct TObjectExtTrait<CPillarTrap::g_ClassID>
{
	using Class = CPillarTrap;
};

END