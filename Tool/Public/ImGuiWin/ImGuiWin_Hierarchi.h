#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

BEGIN(Tool)

/// <summary>
/// �߰��� ������Ʈ�� ���� ������ �� �ִ� ����Ʈ�� ����ִ� Ŭ����.
/// ������Ʈ�翡 ��ȭ�� ����� �׿� ���� ����Ʈ�� ��ȭ�� �� �� �ִ�.
/// ���κ��� ��ŷ�� �ϸ� �׿� ���� ������ �޾�, � ��ü�� ���õǾ����� �����ش�.
/// �ݴ�� ���⼭ ������ ��ü�� �ٸ� �������� �̺�Ʈ�� �Ͼ ���õǾ��ٴ� ���� �����ش�.
/// ������ ��ü, �Ӽ�â�� �����ȴ�.
/// </summary>
class CImGuiWin_Hierarchi : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Hierarchi)

protected:
	explicit CImGuiWin_Hierarchi() = default;
	explicit CImGuiWin_Hierarchi(const CImGuiWin_Hierarchi& rhs) = default;
	virtual ~CImGuiWin_Hierarchi() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Hierarchi* Create();

private:
	virtual void	Free() override;
};

END