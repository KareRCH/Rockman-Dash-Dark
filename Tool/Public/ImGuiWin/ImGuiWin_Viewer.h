#pragma once

#include "Tool_Define.h"
#include "BaseClass/ImGuiWin.h"

BEGIN(Tool)

class CImGuiWin_Viewer final : public Engine::CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Viewer)

protected:
	explicit CImGuiWin_Viewer() = default;
	explicit CImGuiWin_Viewer(const CImGuiWin_Viewer& rhs) = default;
	virtual ~CImGuiWin_Viewer() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Viewer* Create();

private:
	virtual void	Free() override;

public:
	void LoadTextureFromFIle(const string& strFileName);

private:
	ComPtr<ID3D11ShaderResourceView>	m_pSRV = { nullptr };
	ComPtr<ID3D11ShaderResourceView>	m_pPrevSRV = { nullptr };
};

END