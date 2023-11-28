#pragma once

#include "Base.h"
#include "BaseClass/CloudStation.h"

BEGIN(Engine)

/// <summary>
/// ������ �Ŵ���
/// �����带 �߰��ϰ� �����带 �����ش�.
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
