#pragma once


#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"
#include "Component/D3D11DeviceComp.h"

#include "Utility/DelegateTemplate.h"

BEGIN(Engine)
class CTerrain;
END

BEGIN(Tool)

/// <summary>
/// 터레인을 생성, 수정, 높이, 브러쉬를 통해 수정을 가할 수 있는 클래스
/// </summary>
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
	void		Layout_TerrainEdit(const _float& fTimeDelta);
	void		Layout_TerrainBrush(const _float& fTimeDelta);
	void		Layout_TerrainHeight(const _float& fTimeDelta);

	HRESULT		Terrain_SaveFile();

public:
	GETSET_1(CTerrain*, m_pPickedTerrain, Terrain, GET__C)

private:
	class CTerrain* m_pPickedTerrain = { nullptr };

	_int			m_ivTerrainVertex_CountX = 1025;
	_int			m_ivTerrainVertex_CountZ = 1025;

	_int			m_ivTerrainMaxWidth = 100;

public:
	_bool IsCanEdit_Terrain() { return (m_bIsEditing && nullptr != m_pPickedTerrain); }

public:
	// 편집모드, 생성, 편집, 브러쉬 모드가 있다.
	enum class EEditMode { Create, Edit, Brush, Height };
	// 브러시 모드
	enum class EBrushType { Circle, Rect };

private:
	_bool		m_bIsEditing = { false };
	EEditMode	m_eEditMode = { EEditMode::Create };
	EBrushType	m_eBrushType = { EBrushType::Circle };

private:	// 브러쉬 설정값들
	_float	m_fBrush_Diameter = 3.f;		// 브러쉬 사이즈, 지름 기준
	_float	m_fBrush_Strength = 1.f;		// 브러쉬 강한 정도
	
	
};

END