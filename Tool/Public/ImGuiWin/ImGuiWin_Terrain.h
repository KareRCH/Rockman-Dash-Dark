#pragma once


#include "Tool_Define.h"
#include "BaseClass/ImGuiWin.h"

BEGIN(Engine)

class CTerrain;

END

BEGIN(Tool)

class CImGuiWin_Terrain : public Engine::CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Terrain)

protected:
	explicit CImGuiWin_Terrain() = default;
	explicit CImGuiWin_Terrain(const CImGuiWin_Terrain& rhs) = default;
	virtual ~CImGuiWin_Terrain() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Terrain* Create();

private:
	virtual void	Free() override;

private:
	void		Layout_TerrainSetting(const _float& fTimeDelta);

private:
	_int			m_ivTerrainVertex_CountX = 0;
	_int			m_ivTerrainVertex_CountY = 0;

	_int			m_ivTerrainWidth = 0;
	_int			m_ivTerrainHeight = 0;

	class CTerrain* m_pTerrain = { nullptr };
};

END