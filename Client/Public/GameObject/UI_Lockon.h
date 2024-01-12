#pragma once


#include "Client_Define.h"
#include "BaseClass/GameObject.h"

BEGIN(Engine)
class CPlaneModelComp;
END

BEGIN(Client)

/// <summary>
/// 록온 이미지를 띄우는 UI 오브젝트
/// </summary>
class CUI_Lockon :public CGameObject
{
	DERIVED_CLASS(CGameObject, CUI_Lockon)

protected:
	explicit CUI_Lockon();
	explicit CUI_Lockon(const CUI_Lockon& rhs);
	virtual ~CUI_Lockon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CUI_Lockon* Create();
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

public:
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();

private:
	CPlaneModelComp* m_pLockon_Round_Image = { nullptr };
	CPlaneModelComp* m_pLockon_Arrow_Image = { nullptr };

private:
	_float		m_fLockon_Lerp = { 0.f };
	_float		m_fLockon_Speed = { 0.1f };
	_float		m_fRotateSpeed = { 5.f };
	_float3		m_vLatest_LockonPos = {};

public:
	void Set_Target(class CCharacter_Common* pTarget) { m_pTarget = pTarget; Safe_AddRef(m_pTarget); }
	void Clear_Target();

private:
	class CCharacter_Common*	m_pTarget = { nullptr };

};

END