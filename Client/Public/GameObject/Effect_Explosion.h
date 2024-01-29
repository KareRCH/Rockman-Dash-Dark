#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"
#include "Utility/LogicDeviceBasic.h"


BEGIN(Engine)
class CCommonModelComp;
END

BEGIN(Client)

/// <summary>
/// ���� ����Ʈ. �޽��� ������
/// </summary>
class CEffect_Explosion : public CGameObject
{
	DERIVED_CLASS(CGameObject, CEffect_Explosion)

protected:
	explicit CEffect_Explosion();
	explicit CEffect_Explosion(const CEffect_Explosion& rhs);
	virtual ~CEffect_Explosion() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CEffect_Explosion* Create();
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void			Free() override;

public:
	virtual FSerialData SerializeData() override;

public:
	HRESULT Initialize_Component();

public:
	void Set_StartTime(_float fTime) { m_fStartTime.Readjust(fTime); }

private:
	CCommonModelComp* m_pModelComp = { nullptr };

	FGauge			m_fLifeTime = FGauge(0.7f);
	FGauge			m_fScale = FGauge(200.f);
	FGauge			m_fAlpha = FGauge(1.f, true);
	_float			m_fOffset = 0.f;
	_float			m_fAlphaMinTime = { 0.4f };		// �ִϸ��̼� ��� �ð��� ���� ���İ��� ����Ǵ� ����
	FGauge			m_fStartTime = FGauge(0.f);
};

END