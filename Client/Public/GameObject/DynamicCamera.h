#pragma once

#include "BaseClass/Camera.h"
#include "Client_Define.h"

#include "Component/PivotComponent.h"
#include "GameObject/GameObjectFactory.h"

BEGIN(Client)

/// <summary>
/// 게임 내에서 유동적으로 쓰이는 카메라
/// </summary>
class CDynamicCamera final : public Engine::CCamera
{
	DERIVED_CLASS(CCamera, CDynamicCamera)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::DynamicCamera);

protected:
	explicit CDynamicCamera();
	explicit CDynamicCamera(const CDynamicCamera& rhs);
	virtual ~CDynamicCamera() = default;

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
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

public:
	static	CDynamicCamera* Create();
	static	CDynamicCamera* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr);
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

private:
	CPivotComponent* m_pPivotComp = { nullptr };
	CGameObject* m_pTarget = { nullptr };

private:
	_float		m_fHorizontalAngle = { 0.f };
	_float		m_fVerticalAngle = { 0.f };
	_float		m_fSpringLength = { 5.f };
};

template <>
struct TObjectExtTrait<CDynamicCamera::g_ClassID>
{
	using Class = CDynamicCamera;
};

END