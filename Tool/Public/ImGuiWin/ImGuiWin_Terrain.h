#pragma once


#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"
#include "Component/D3D11DeviceComp.h"

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

#pragma region 디바이스 컴포넌트
private:
	ID3D11Device* D3D11Device() const { return m_pDeviceComp->Get_Device(); }
	ID3D11DeviceContext* D3D11Context() const { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion

private:
	void		Layout_TerrainCreate(const _float& fTimeDelta);
	void		Layout_TerrainSetting(const _float& fTimeDelta);

	HRESULT		Terrain_SaveFile();

private:
	class CTerrain* m_pTerrain = { nullptr };

	_int			m_ivTerrainVertex_CountX = 1025;
	_int			m_ivTerrainVertex_CountZ = 1025;

	_int			m_ivTerrainWidth = 100;
	_int			m_ivTerrainHeight = 1;

	
};

END