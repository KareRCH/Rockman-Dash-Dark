#pragma once

#include "GameObject.h"
#include "Component/TerrainModelComp.h"

BEGIN(Engine)

/// <summary>
/// ���� ������ �������� ������ ���� ����� �������� ����� ������Ʈ
/// �ٸ����̼� ���� �� ��ü �ϳ��� ��� ����� �� �������� �����Ѵ�.
/// </summary>
class ENGINE_DLL CTerrain : public CGameObject
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

public:
	static CTerrain* Create();
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();

protected:
	CTerrainModelComp* m_pTerrainModelComp = { nullptr };
};

END