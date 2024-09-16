#pragma once

BEGIN(Engine)

class ENGINE_DLL CSoundManager final
{
	DECLARE_SINGLETON(CSoundManager)

public:
	HRESULT Initialize();
	void Release();

public:
	/* 혹시 모르니 남겨둠
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
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pFmodSystem;

	//FMOD::Syste

	// 사운드 리소스 정보를 갖는 객체 
	std::unordered_map<WCHAR*, FMOD_SOUND*> m_umapFmodSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pFmodChannel[(_int)SOUND_TYPE::SOUND_END];
};

END