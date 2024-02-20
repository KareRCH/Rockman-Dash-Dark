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
	explicit CSoundMgr();
	explicit CSoundMgr(const CSoundMgr& rhs) = delete;
	virtual ~CSoundMgr() = default;

public:
	HRESULT				Initialize(const string& strMainPath);

public:
	static CSoundMgr*	Create(const string& strMainPath);

private:
	virtual void		Free() override;

public:
	// ���� ���
	void Play_Sound(const wstring& strGroupKey, const wstring& strSoundKey, CHANNELID eID, float fVolume);
	// ��� ���
	void Play_BGM(const wstring& strGroupKey, const wstring& strSoundKey, float fVolume);
	void Play_BGM_LoopRange(const wstring& strGroupKey, const wstring& strSoundKey, float fVolume, _uint fStart, _uint fEnd);
	// ���� ����
	void Stop_Sound(CHANNELID eID);
	// ��� ���� ����
	void Stop_All();
	// ä���� ���� ����
	void Set_ChannelVolume(CHANNELID eID, float fVolume);

private:
	// ���������� �ε��� ���� ���� ������ ������ �ε��ϴ� �Լ�
	void Load_SoundFile(const wstring& pGroupKey, const string& pPath);
	// ���� ������ �׷� ������ �񵿱� �ε��� �� ���̴� �Լ�
	void Load_SoundFile_GroupAsync(const wstring& pGroupKey, const string& pPath);
	// �񵿱� ó���� ��ٸ��� �Լ�
	void Wait_GroupAsync();
	// ���� ���� ���Ϸ� �񵿱� �ε��� �� ���̴� �Լ���
	FMOD_RESULT LoadSoundFile_Async(const string& pPath, const string& pFileName, FMOD_RESULT& hResult, FMOD_SOUND** pSound);

private:
	string							m_strPainPath;

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