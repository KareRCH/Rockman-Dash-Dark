#pragma once

#include "Client_Define.h"
#include "BaseClass/CollisionObject.h"
#include "GameObject/GameObjectFactory.h"

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

public:		// 충돌 이벤트
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);

private:
	CCommonModelComp* m_pModelComp = { nullptr };

public:
	// 트랩 타입에 따라
	enum class ETrapType : _uint { Linear, Circle, Rect };

private:
	ETrapType	m_eTrapType = { ETrapType::Circle };
	_float		m_fRadiusRange = { 5.f };		// 트랩이 움직이는 범위 반경

};

template <>
struct TObjectExtTrait<CPillarTrap::g_ClassID>
{
	using Class = CPillarTrap;
};

END