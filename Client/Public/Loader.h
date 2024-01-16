#pragma once

#include "Client_Define.h"
#include "Component/D3D11DeviceComp.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

/// <summary>
/// 로딩 쓰레드 역할을 하는 클래스
/// </summary>
class CLoader : public CBase
{
	DERIVED_CLASS(CBase, CLoader)

protected:
	explicit CLoader();
	explicit CLoader(const CLoader& rhs) = delete;
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(LEVEL eNextLevelID, const wstring& strParsedLevelDataPath);

public:
	static CLoader* Create(LEVEL eNextLevelID, const wstring& strParsedLevelDataPath);
protected:
	virtual void	Free() override;


public:
	void Print_LoadingText();

public:
	HRESULT	Loading();
	HRESULT Loading_For_Logo_Level();
	HRESULT Loading_For_GamePlay_Level();
	HRESULT Loading_For_Parsed_Level();

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_CriticalSection;


public:
	const _bool IsFinished() const { return m_bIsFinished; }

private:
	LEVEL	m_eNextLevelID = { LEVEL_END };
	_tchar	m_szLoadingText[MAX_PATH] = TEXT("");
	_bool	m_bIsFinished = { false };
	wstring m_strParsedLevelDataPath = TEXT("");

};

END