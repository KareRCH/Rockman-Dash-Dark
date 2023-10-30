#pragma once

#include "Base.h"
#include "BaseClass/BlackBoard.h"

BEGIN(Engine)

/// <summary>
/// 블랙보드 매니저
/// 블랙보드를 추가하고 블랙보드를 얻어와준다.
/// </summary>
class CBlackBoardMgr final : public CBase
{
	DERIVED_CLASS(CBase, CBlackBoardMgr)
private:
	explicit CBlackBoardMgr();
	virtual ~CBlackBoardMgr() = default;

public:
	HRESULT			Initialize();
	_int			Tick();

public:
	static CBlackBoardMgr* Create();

private:
	virtual	void	Free();

public:
	HRESULT			Add_BlackBoard(const wstring& strBoardName, CBlackBoard* pBlackBoard);
	HRESULT			Delete_BlackBoard(const wstring& strBoardName);
	CBlackBoard*	Get_BlackBoard(const wstring& strBoardName);
	

private:
	_unmap<wstring, CBlackBoard*>			m_mapBlackBoard;

};

END
