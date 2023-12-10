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
	explicit CLoader() = default;
	explicit CLoader(const CLoader& rhs) = delete;
	virtual ~CLoader() = default;

public:
	HRESULT Initialize();

public:
	static CLoader* Create();
protected:
	virtual void	Free() override;

#pragma region 디바이스
public:
	const ID3D11Device* D3D11Device() const { return m_pDeviceComp->Get_Device(); }
	const ID3D11DeviceContext* D3D11Context() const { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion

public:
	void Print_LoadingText();

public:
	HRESULT	Loading();
	HRESULT Loading_For_Logo_Level();
	HRESULT Loading_For_GamePlay_Level();


private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_CriticalSection;

private:
	_tchar	m_szLoadingText[MAX_PATH] = TEXT("");
	_bool	m_bIsFinished = { false };





};

END