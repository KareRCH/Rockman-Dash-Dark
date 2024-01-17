#pragma once

#include "Client_Define.h"
#include "BaseClass/CollisionObject.h"
#include "GameObject/GameObjectFactory.h"


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
};

template <>
struct TObjectExtTrait<CItem_Deflector::g_ClassID>
{
	using Class = CItem_Deflector;
};

END