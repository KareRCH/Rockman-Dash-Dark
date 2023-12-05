#pragma once

#include "Base.h"


struct ImGuiIO;

BEGIN(Engine)

class CImGuiWin;

/// <summary>
/// ImGui 창의 계층을 관리하기 위한 클래스
/// ImGui는 게임 오브젝트 Tick과 다르게 돌아간다.
/// 이는 
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
	// 디바이스의 해상도 변경시 호출되는 함수입니다.
	HRESULT ResetResolution();



public:
	HRESULT Add_ImGuiWin(const wstring& strKey, CImGuiWin* pImGuiWin);


private:
	ImGuiIO*	m_pIO = { nullptr };

private:
	map<wstring, CImGuiWin*>		m_mapImGuiWin;		// 이름 검색 시스템용, 저장 목적 및 이름 검색용임.

private:
	vector<CImGuiWin*>				m_vecRootWins;		// 상위에 노출되는 UI들, 그렇지 않다면 Win의 자식 객체로 존재한다.

};

END