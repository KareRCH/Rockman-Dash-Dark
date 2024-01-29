#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"
#include "Component/TeamAgentComp.h"
#include "Utility/LogicDeviceBasic.h"


BEGIN(Engine)
class CCommonModelComp;
END



BEGIN(Client)

/// <summary>
/// �������� ���� �κ����� ������ �������� �����ϴ� Ŭ����
/// </summary>
class CWeapon_LaserEmission : public CGameObject
{
	DERIVED_CLASS(CGameObject, CWeapon_LaserEmission)

protected:
	explicit CWeapon_LaserEmission();
	explicit CWeapon_LaserEmission(const CWeapon_LaserEmission& rhs);
	virtual ~CWeapon_LaserEmission() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype(const _float3 vPos);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(const _float3 vPos);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CWeapon_LaserEmission* Create();
	static CWeapon_LaserEmission* Create(const _float3 vPos);
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();

public:
	virtual FSerialData SerializeData_Prototype() override { return FSerialData(); };
	virtual FSerialData SerializeData() override;

private:
	CCommonModelComp* m_pModelComp = { nullptr };

#pragma region �� ������Ʈ
public:
	CTeamAgentComp& TeamAgentComp() const { return *m_pTeamAgentComp; }

private:
	CTeamAgentComp* m_pTeamAgentComp = { nullptr };
#pragma endregion


public:
	void EndShooting() { Set_Dead(); }


private:
	void Create_Laser();

private:
	class CWeapon_Laser*	m_pLaser = { nullptr };
	FGauge					m_fLaserCreateTime = FGauge(0.2f);

};

END