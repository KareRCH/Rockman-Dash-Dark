#pragma once

#include "Base.h"


struct ImGuiIO;

BEGIN(Engine)

class CImGuiWin;

/// <summary>
/// ImGui â�� ������ �����ϱ� ���� Ŭ����
/// ImGui�� ���� ������Ʈ Tick�� �ٸ��� ���ư���.
/// �̴� 
/// </summary>
class ENGINE_DLL_DBG CImGuiMgr final : public CBase
{
	DERIVED_CLASS(CBase, CImGuiMgr)

private:
	explicit CImGuiMgr();
	virtual ~CImGuiMgr() = default;

public:
	HRESULT	Initialize();
	void Tick(const _float fTimeDelta);
	HRESULT Render();

public:
	static CImGuiMgr* Create();

private:
	virtual void	Free();


public:
	// ����̽��� �ػ� ����� ȣ��Ǵ� �Լ��Դϴ�.
	HRESULT ResetResolution();



public:
	HRESULT Add_ImGuiWin(const wstring& strKey, CImGuiWin* pImGuiWin);


private:
	ImGuiIO*	m_pIO = { nullptr };

private:
	map<wstring, CImGuiWin*>		m_mapImGuiWin;		// �̸� �˻� �ý��ۿ�, ���� ���� �� �̸� �˻�����.

private:
	vector<CImGuiWin*>				m_vecRootWins;		// ������ ����Ǵ� UI��, �׷��� �ʴٸ� Win�� �ڽ� ��ü�� �����Ѵ�.

};

END