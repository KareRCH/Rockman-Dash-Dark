#pragma once

#include "GameObject.h"

BEGIN(Engine)

/// <summary>
/// 지형 정보를 바탕으로 지형에 대한 기능을 가지도록 설계된 오브젝트
/// </summary>
class ENGINE_DLL CTerrain final : public CGameObject
{
	DERIVED_CLASS(CGameObject, CTerrain)

protected:
	explicit CTerrain() = default;
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual _int	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual void	Render() override;

protected:
	virtual void	Free() override;
};

END