#pragma once


#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

BEGIN(Engine)

class CTerrain;

END

BEGIN(Tool)

class CImGuiWin_Terrain : public CImGuiWin
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
	void		Layout_TerrainCreate(const _float& fTimeDelta);
	void		Layout_TerrainSetting(const _float& fTimeDelta);

private:
	class CTerrain* m_pTerrain = { nullptr };

	_int			m_ivTerrainVertex_CountX = 1025;
	_int			m_ivTerrainVertex_CountZ = 1025;

	_int			m_ivTerrainWidth = 100;
	_int			m_ivTerrainHeight = 1;

	
};

END