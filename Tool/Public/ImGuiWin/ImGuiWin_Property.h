#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

BEGIN(Tool)

/// <summary>
/// ��ü�� ������ �� �ִ� �Ӽ����� �����ִ� â�̴�.
/// �⺻������ ��ġ, ȸ��, ũ�⸦ �� �� �ְ�.
/// �߰������� �����Ǵ� Ŭ������ ���� �Ӽ����� �ִٸ�, �װ��� ������ ���� �ִ�.
/// </summary>
class CImGuiWin_Property : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Property)

protected:
	explicit CImGuiWin_Property() = default;
	explicit CImGuiWin_Property(const CImGuiWin_Property& rhs) = default;
	virtual ~CImGuiWin_Property() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Property* Create();

private:
	virtual void	Free() override;
};

END