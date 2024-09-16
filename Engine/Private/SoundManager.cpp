#include "Engine_pch.h"
#include "SoundManager.h"

IMPLEMENT_SINGLETON(CSoundManager)

HRESULT CSoundManager::Initialize()
{
	// 사운드를 담당하는 대표객체를 생성하는 함수
	FMOD_System_Create(&m_pFmodSystem, FMOD_VERSION);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pFmodSystem, 32, FMOD_INIT_NORMAL, NULL);

	//LoadSoundFile();
	LoadSoundFiles("../../Resources/Level_Static/Sounds");

	return S_OK;
}
void CSoundManager::Release()
{
	for (auto& Mypair : m_umapFmodSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_umapFmodSound.clear();

	FMOD_System_Release(m_pFmodSystem);
	FMOD_System_Close(m_pFmodSystem);
	m_upInstance.reset();
}

void CSoundManager::PlaySoundEx(std::string _strSoundKey, SOUND_TYPE _eSoundType, float _fVolume)
{
	std::string strSoundKey = _strSoundKey;

	WCHAR tSoundKey[128] = L"";    // 변환시킬 문자열을 담음
	size_t sLength = 0, sConvertedChars = 0; // 원래 문자열 길이, 변환된 문자열 길이

	sLength = strlen(strSoundKey.c_str()) + 1;  // 변환시킬 문자열의 길이를 구함
	mbstowcs_s(&sConvertedChars, tSoundKey, sLength, strSoundKey.c_str(), _TRUNCATE); // 변환 후, 변환된 _strSoundKey를 WCHAR tSoundKey[128] = L""; 여기에 집어 넣어줌

	// 위에는 해당 키값을 WCHAR 형으로 변환 시켜주는 과정

	std::unordered_map<WCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_umapFmodSound.begin(), m_umapFmodSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(tSoundKey, iter.first);
		});
	if (iter == m_umapFmodSound.end()) { return; }

	FMOD_System_PlaySound(m_pFmodSystem, iter->second, FALSE, FALSE, &m_pFmodChannel[(_int)_eSoundType]);
	FMOD_Channel_SetVolume(m_pFmodChannel[(_int)_eSoundType], _fVolume);
	FMOD_System_Update(m_pFmodSystem);
}

void CSoundManager::PlayBGMLoop(std::string _strSoundKey, float _fVolume)
{
	std::string strSoundKey = _strSoundKey;

	WCHAR tSoundKey[128] = L"";    // 변환시킬 문자열을 담음
	size_t sLength = 0, sConvertedChars = 0; // 원래 문자열 길이, 변환된 문자열 길이

	sLength = strlen(strSoundKey.c_str()) + 1;  // 변환시킬 문자열의 길이를 구함
	mbstowcs_s(&sConvertedChars, tSoundKey, sLength, strSoundKey.c_str(), _TRUNCATE); // 변환 후, 변환된 _pSoundKey를 TCHAR tSoundKey[128] = L""; 여기에 집어 넣어줌

	// 위에는 해당 키값을 WCHAR 형으로 변환 시켜주는 과정

	std::unordered_map<WCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_umapFmodSound.begin(), m_umapFmodSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(tSoundKey, iter.first);
		});
	if (iter == m_umapFmodSound.end()) { return; }


	FMOD_System_PlaySound(m_pFmodSystem, iter->second, FALSE, FALSE, &m_pFmodChannel[(_int)SOUND_TYPE::BGM]);
	FMOD_Channel_SetMode(m_pFmodChannel[(_int)SOUND_TYPE::BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pFmodChannel[(_int)SOUND_TYPE::BGM], _fVolume);
	FMOD_System_Update(m_pFmodSystem);
}

void CSoundManager::StopSound(SOUND_TYPE _eSoundType)
{
	FMOD_Channel_Stop(m_pFmodChannel[(_int)_eSoundType]);
}

void CSoundManager::StopAll()
{
	for (_int i = 0; i < (_int)SOUND_TYPE::SOUND_END; ++i)
		FMOD_Channel_Stop(m_pFmodChannel[i]);
}

void CSoundManager::SetChannelVolume(SOUND_TYPE _eSoundType, float _fVolume)
{
	FMOD_Channel_SetVolume(m_pFmodChannel[(_int)_eSoundType], _fVolume);
	FMOD_System_Update(m_pFmodSystem);
}

void CSoundManager::LoadSoundFiles(std::string _strPath)
{
	_finddata_t fd;
	// 주어진 경로에서 일치하는 파일이 있는지 없는지 찾기 
	intptr_t handle = _findfirst((_strPath + "/*.*").c_str(), &fd);

	// 조건에 맞지 않다면 -1이 리턴
	if (handle == -1) { return; }

	_int iResult = 0;

	while (iResult != -1)
	{
		// attrib은 파일 또는 디렉토리의 속성을 나타내며, _A_SUBDIR은 디렉토리를 나타냅니다. 즉, fd.attrib가 디렉토리이면 true
		if (fd.attrib & _A_SUBDIR)
		{
			if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0)
			{
				// 기존 경로 + 새로운 폴더 이름
				std::string strNewPath = _strPath + "/" + fd.name;
				// 재귀적으로 폴더 탐색
				LoadSoundFiles(strNewPath.c_str());
			}
		}
		else
		{
			std::string strFullPath = _strPath + "/" + fd.name;
			FMOD_SOUND* pSound = nullptr;
			FMOD_RESULT eRes = FMOD_System_CreateSound(m_pFmodSystem, strFullPath.c_str(), FMOD_DEFAULT, 0, &pSound);

			if (eRes == FMOD_OK)
			{
				size_t iLength = strlen(fd.name) + 1;
				WCHAR* pSoundKey = new WCHAR[(_int)iLength];
				ZeroMemory(pSoundKey, sizeof(WCHAR) * (_int)iLength);
				MultiByteToWideChar(CP_ACP, 0, fd.name, (_int)iLength, pSoundKey, (_int)iLength);
				m_umapFmodSound.emplace(pSoundKey, pSound);
			}
		}
		iResult = _findnext(handle, &fd);
	}
	FMOD_System_Update(m_pFmodSystem);

	_findclose(handle);
}
