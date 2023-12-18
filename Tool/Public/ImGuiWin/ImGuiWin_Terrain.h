#pragma once


#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"
#include "Component/D3D11DeviceComp.h"
#include "Component/PipelineComp.h"

#include "Utility/DelegateTemplate.h"

BEGIN(Engine)
class CTerrain;
END

BEGIN(Tool)

typedef FastDelegate1<_bool> ModeSelectedDelegate;
typedef MulticastDelegate<ModeSelectedDelegate, _bool> ModeSeleectedBroadDelegate;

/// <summary>
/// �ͷ����� ����, ����, ����, �귯���� ���� ������ ���� �� �ִ� Ŭ����
/// </summary>
class CImGuiWin_Terrain : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Terrain)

protected:
	explicit CImGuiWin_Terrain();
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
	class CGameInstance* m_pGI = { nullptr };

#pragma region ����̽� ������Ʈ
private:
	ID3D11Device* D3D11Device() const { return m_pDeviceComp->Get_Device(); }
	ID3D11DeviceContext* D3D11Context() const { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion

#pragma region ���������� ������Ʈ
protected:
	CPipelineComp& PipelineComp() { return (*m_pPipelineComp); }

private:
	CPipelineComp* m_pPipelineComp = { nullptr };
#pragma endregion

private:
	void		Layout_TerrainCreate(const _float& fTimeDelta);
	void		Layout_TerrainEdit(const _float& fTimeDelta);
	void		Layout_TerrainDraw(const _float& fTimeDelta);
	void		Layout_TerrainHeight(const _float& fTimeDelta);

	HRESULT		Terrain_SaveFile();

public:
	GETSET_1(CTerrain*, m_pPickedTerrain, Terrain, GET__C)

private:
	class CTerrain*				m_pPickedTerrain = { nullptr };		// ���� �ͷ���

	_int						m_ivTerrainVertex_CountX = 129;
	_int						m_ivTerrainVertex_CountZ = 129;

	_int						m_ivTerrainMaxWidth = 100;

	class CTerrain*				m_pPrevPickedTerrain = { nullptr };	// ���� ������ ���� üũ�� ���� ���� �ͷ��� ���� ����
	vector<SHADER_VTX_NORM>		m_TerrainVertices;					// �ͷ��� ����
	_uint						m_iTerrainVertexCountX;		
	_uint						m_iTerrainVertexCountZ;
	vector<_uint>				m_TerrainIndices;					// �ͷ��� �ε��� ����

public:
	// �� ���� ����ϴ� �Լ�
	HRESULT Link_ToViewer();
	// �������� ���� ����ϴ� �Լ�
	void Link_ToBrowser();

private:
	_bool		m_bIsLinkedToViewer = { false };
	_bool		m_bIsLinkedToBrowser = { false };

public:
	void Handle_PickedObject(CGameObject* pGameObj);
	void Handle_PickedBrush(_float3 vPickedWorldPos);
	void Handle_PlaceModeSelected(_bool bIsSelected);
	_bool IsCanEdit_Terrain() { return (m_bIsEditing && nullptr != m_pPickedTerrain); }

	// �ܺο��� ��ε� ĳ��Ʈ ��������Ʈ�� �������߰�
	void Add_ModeSelectedListener(const ModeSelectedDelegate& Listener) { OnModeSelected.Add(Listener); }
	void Remove_ModeSelectedListener(const ModeSelectedDelegate& Listener) { OnModeSelected.Remove(Listener); }

public:
	// �������, ����, ����, �귯�� ��尡 �ִ�.
	enum class EEditMode { Create, Edit, Draw, Height };
	// �귯�� ���
	enum class EBrushType { Circle, Rect };

private:
	_bool		m_bIsEditing = { false };
	EEditMode	m_eEditMode = { EEditMode::Create };
	EBrushType	m_eBrushType = { EBrushType::Circle };

	ModeSeleectedBroadDelegate OnModeSelected;
	_bool						m_bIsClickedOnViewer = false;
	_float3						m_vPickedWorldPos = {};

private:	// �׸��� �귯�� ��������
	_float	m_fDrawBrush_Diameter = 3.f;		// �귯�� ������, ���� ����


private:
	void HeightBrush_Draw(const _float& fTimeDelta);

private:	// �귯�� ��������
	_bool	m_bIsSetting_HeightBrush = false;	// �귯�� ��������
	_float	m_fHeightBrush_Diameter = 3.f;		// �귯�� ������, ���� ����
	_float	m_fHeightBrush_Strength = 1.f;		// �귯�� ���� ����
	

	
};

END