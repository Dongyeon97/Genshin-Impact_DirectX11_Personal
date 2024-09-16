#pragma once

/* CObjectManager ※ [0번 레벨]은 Level_Static으로 약속합니다. ※ */
// 1. [레벨], [레이어]별로 오브젝트들을 [관리]하고 [업데이트]
// 2. [레벨]을 전환

BEGIN(Engine)

class CLevel;
class CActor;

class ENGINE_DLL CObjectManager final
{
	DECLARE_SINGLETON(CObjectManager)

public:
	HRESULT Initialize(_uint _iNumLevels, _uint _iNumLayers);
	_int PreUpdate(const _float& _fTimeDelta);
	_int Update(const _float& _fTimeDelta);
	_int LateUpdate(const _float& _fTimeDelta);
	void Render();
	void EndPlay();
	void Release();

public:
	// [ 레벨 전환 ]
	HRESULT OpenLevel(shared_ptr<CLevel> _spNewLevel);

	// [ 액터 생성 후 추가 ]
	HRESULT AddActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName, shared_ptr<CActor> _spActor);

	// [ (단순) 액터 추가 ]
	HRESULT InsertActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName, shared_ptr<CActor> _spActor);

	// [ 액터 검색 ]
	shared_ptr<CActor> FindActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName);

	// [ 레벨 오브젝트 정리 ]
	void ClearCurrentScene(_uint _iLevelIndex);

public:
	// [ 스태틱 레벨 반환 ]
	shared_ptr<CLevel> GetStaticLevel() {
		if (nullptr == m_vecLevels[0])
			return nullptr;

		return m_vecLevels[0];
	}

	// [ 현재 레벨 반환 ]
	shared_ptr<CLevel> GetCurLevel() {
		if (nullptr == m_vecLevels[m_iCurLevelIndex])
			return nullptr;

		return m_vecLevels[m_iCurLevelIndex];
	}

private:
	// [ 레벨 초기화 ]
	HRESULT ReserveLevels(_uint _iNumLevels, _uint _iNumLayers);

	// [ 레벨 생성 ]
	shared_ptr<CLevel> CreateLevel(_uint _iLevelIndex, _uint _iNumLayers);

	// [ 레벨 검색 ]
	shared_ptr<CLevel> FindLevel(_uint _iLevelIndex);

public:
	// [ 필드위의 현재 플레이어 ]
	void SetCurPlayer(weak_ptr<CActor> _wpPlayer) { m_wpCurPlayer = _wpPlayer; }
	shared_ptr<CActor> GetCurPlayer() { return m_wpCurPlayer.lock(); }

	// [ 로딩중 ]
	void SetIsLoading(_bool _bLoading) { m_bLoading = _bLoading; }
	_bool GetIsLoading() { return m_bLoading; }

private:
	// [ 지금 로딩 중인지 ]
	_bool m_bLoading = false;

	// [ 현재 레벨 ]
	shared_ptr<CLevel> m_spCurLevel;

	// [ 현재 플레이어 ]
	weak_ptr<CActor> m_wpCurPlayer;

	// [ 현재 레벨 인덱스 ]
	_uint m_iCurLevelIndex = 0;

	// [ 레벨 개수 ]
	_uint m_iNumLevels = 0;

	// [ 레이어 개수 ]
	_uint m_iNumLayers = 0;

	// [ 레벨 벡터 ]
	vector<shared_ptr<CLevel>> m_vecLevels;
};

END