#pragma once

BEGIN(Engine)

class ENGINE_DLL CSoundManager final
{
	DECLARE_SINGLETON(CSoundManager)

public:
	HRESULT Initialize();
	void Release();

public:
	/* Ȥ�� �𸣴� ���ܵ�
	//void PlaySound(TCHAR* _pSoundKey, ESoundType _eSoundType, float _fVolume);
	//void PlayBGMLoop(TCHAR* _pSoundKey, float _fVolume);
	*/
	void PlaySoundEx(string _strSoundKey, SOUND_TYPE _eSoundType, float _fVolume);
	void PlayBGMLoop(string _strSoundKey, float _fVolume);
	void StopSound(SOUND_TYPE _eSoundType);
	void StopAll();
	void SetChannelVolume(SOUND_TYPE _eSoundType, float _fVolume);

private:
	//void LoadSoundFile();
	void LoadSoundFiles(std::string _strPath);

private:
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pFmodSystem;

	//FMOD::Syste

	// ���� ���ҽ� ������ ���� ��ü 
	std::unordered_map<WCHAR*, FMOD_SOUND*> m_umapFmodSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pFmodChannel[(_int)SOUND_TYPE::SOUND_END];
};

END