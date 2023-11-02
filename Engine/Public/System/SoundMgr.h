#pragma once

#include "Base.h"

#include <mutex>
#include <future>
#include <thread>

BEGIN(Engine)

class ENGINE_DLL_DBG FSoundData final : public CBase
{
	DERIVED_CLASS(CBase, FSoundData)
private:
	FSoundData() {}
	virtual ~FSoundData() {}

public:
	static FSoundData* Create()
	{
		ThisClass* pInstance = new ThisClass;

		return pInstance;
	}
	virtual void Free()
	{
		mapSound.clear();
	}

public:
	using map_sound = _unmap<wstring, FMOD_SOUND*>;
	GETSET_1(map_sound, mapSound, MapSound, GET_REF)

private:
	map_sound mapSound;
};


/// <summary>
/// ���� ��� Ŭ����
/// </summary>
class CSoundMgr final : public CBase
{
	DERIVED_CLASS(CBase, CSoundMgr)

private:
	CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	HRESULT				Initialize();

public:
	static CSoundMgr*	Create();

private:
	virtual void		Free() override;

public:
	// ���� ���
	void Play_Sound(_tchar* pCategoryKey, _tchar* pSoundKey, CHANNELID eID, float fVolume);
	// ��� ���
	void Play_BGM(_tchar* pCategoryKey, _tchar* pSoundKey, float fVolume);
	// ���� ����
	void Stop_Sound(CHANNELID eID);
	// ��� ���� ����
	void StopAll();
	// ä���� ���� ����
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:
	// ���������� �ε��� ���� ���� ������ ������ �ε��ϴ� �Լ�
	void LoadSoundFile(_tchar* pCategoryKey, const char* pPath);
	// ���� ������ �׷� ������ �񵿱� �ε��� �� ���̴� �Լ�
	void LoadSoundFile_GroupAsync(_tchar* pGroupKey, const char* pPath);
	// �񵿱� ó���� ��ٸ��� �Լ�
	void Wait_GroupAsync();
	// ���� ���� ���Ϸ� �񵿱� �ε��� �� ���̴� �Լ���
	FMOD_RESULT LoadSoundFile_Async(const char* pPath, const char* pFileName, FMOD_RESULT& hResult, FMOD_SOUND** pSound);

private:
	// ���� ���ҽ� ������ ���� ��ü 
	_unmap<wstring, FSoundData*>	m_mapSound;
	mutex							m_mtxSound;
	vector<future<void>>			m_vecAsyncSoundGroup;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL*		m_pChannelArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM*		m_pSystem = nullptr;

	FMOD_CHANNELGROUP*	m_pChannelGroup[MAX_CHANNEL_GROUP];
	FMOD_CHANNELGROUP*	m_pMasterChanelGroup;
};

END