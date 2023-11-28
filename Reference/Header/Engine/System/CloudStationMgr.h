#pragma once

#include "Base.h"
#include "BaseClass/CloudStation.h"

BEGIN(Engine)

/// <summary>
/// 블랙보드 매니저
/// 블랙보드를 추가하고 블랙보드를 얻어와준다.
/// </summary>
class ENGINE_DLL_DBG CCloudStationMgr final : public CBase
{
	DERIVED_CLASS(CBase, CCloudStationMgr)
private:
	explicit CCloudStationMgr();
	virtual ~CCloudStationMgr() = default;

public:
	HRESULT			Initialize();
	_int			Tick();

public:
	static CCloudStationMgr* Create();

private:
	virtual	void	Free();

public:
	HRESULT			Add_BlackBoard(const wstring& strBoardName, CCloudStation* pBlackBoard);
	HRESULT			Delete_BlackBoard(const wstring& strBoardName);
	CCloudStation*	Get_BlackBoard(const wstring& strBoardName);
	

private:
	_unmap<wstring, CCloudStation*>			m_mapBlackBoard;

};

END
