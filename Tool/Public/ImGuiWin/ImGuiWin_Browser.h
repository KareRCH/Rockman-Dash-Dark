#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

#include "Utility/DelegateTemplate.h"

BEGIN(Tool)


/// <summary>
/// 레벨 불러오기 (저장은 별개로), 오브젝트 배치를 하기 위해 아이템을 선택할 수 있는 상호작용을 제공한다.
/// </summary>
class CImGuiWin_Browser : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Browser)

protected:
	explicit CImGuiWin_Browser() = default;
	explicit CImGuiWin_Browser(const CImGuiWin_Browser& rhs) = default;
	virtual ~CImGuiWin_Browser() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Browser* Create();

private:
	virtual void	Free() override;

private:
	void Layout_Object(const _float& fTimeDelta);

public:
	// 배치 할지 안 할지
	enum class EMode { None, Place };

private:
	EMode	m_ePlaceMode = { EMode::None };
	

};

END