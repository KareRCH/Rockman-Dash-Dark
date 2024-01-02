#pragma once

#include "Tool_Define.h"
#include "ImGuiWin.h"

#include "Component/D3D11DeviceComp.h"
#include "Component/PipelineComp.h"

#include "Utility/DelegateTemplate.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Tool)

class CImGuiWin_Navigation final : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Navigation)

protected:
	explicit CImGuiWin_Navigation();
	explicit CImGuiWin_Navigation(const CImGuiWin_Navigation& rhs) = default;
	virtual ~CImGuiWin_Navigation() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Navigation* Create();

private:
	virtual void	Free() override;

private:
	class CGameInstance* m_pGI = { nullptr };


private:
	void		Layout_NavigationCreate(const _float& fTimeDelta);
	void		Layout_NavgationiEdit(const _float& fTimeDelta);

public:
	// �������, ����, ����, �귯�� ��尡 �ִ�.
	enum class EMode { Create, Edit };

private:
	EMode m_eMode = { EMode::Create };

private:
	CNavigation* m_pNavigation = { nullptr };

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

};

END