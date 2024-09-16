#pragma once

/* CObjectManager �� [0�� ����]�� Level_Static���� ����մϴ�. �� */
// 1. [����], [���̾�]���� ������Ʈ���� [����]�ϰ� [������Ʈ]
// 2. [����]�� ��ȯ

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
	// [ ���� ��ȯ ]
	HRESULT OpenLevel(shared_ptr<CLevel> _spNewLevel);

	// [ ���� ���� �� �߰� ]
	HRESULT AddActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName, shared_ptr<CActor> _spActor);

	// [ (�ܼ�) ���� �߰� ]
	HRESULT InsertActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName, shared_ptr<CActor> _spActor);

	// [ ���� �˻� ]
	shared_ptr<CActor> FindActor(_uint _iLevelIndex, _uint _iLayerIndex, const wstring& _wstrActorName);

	// [ ���� ������Ʈ ���� ]
	void ClearCurrentScene(_uint _iLevelIndex);

public:
	// [ ����ƽ ���� ��ȯ ]
	shared_ptr<CLevel> GetStaticLevel() {
		if (nullptr == m_vecLevels[0])
			return nullptr;

		return m_vecLevels[0];
	}

	// [ ���� ���� ��ȯ ]
	shared_ptr<CLevel> GetCurLevel() {
		if (nullptr == m_vecLevels[m_iCurLevelIndex])
			return nullptr;

		return m_vecLevels[m_iCurLevelIndex];
	}

private:
	// [ ���� �ʱ�ȭ ]
	HRESULT ReserveLevels(_uint _iNumLevels, _uint _iNumLayers);

	// [ ���� ���� ]
	shared_ptr<CLevel> CreateLevel(_uint _iLevelIndex, _uint _iNumLayers);

	// [ ���� �˻� ]
	shared_ptr<CLevel> FindLevel(_uint _iLevelIndex);

public:
	// [ �ʵ����� ���� �÷��̾� ]
	void SetCurPlayer(weak_ptr<CActor> _wpPlayer) { m_wpCurPlayer = _wpPlayer; }
	shared_ptr<CActor> GetCurPlayer() { return m_wpCurPlayer.lock(); }

	// [ �ε��� ]
	void SetIsLoading(_bool _bLoading) { m_bLoading = _bLoading; }
	_bool GetIsLoading() { return m_bLoading; }

private:
	// [ ���� �ε� ������ ]
	_bool m_bLoading = false;

	// [ ���� ���� ]
	shared_ptr<CLevel> m_spCurLevel;

	// [ ���� �÷��̾� ]
	weak_ptr<CActor> m_wpCurPlayer;

	// [ ���� ���� �ε��� ]
	_uint m_iCurLevelIndex = 0;

	// [ ���� ���� ]
	_uint m_iNumLevels = 0;

	// [ ���̾� ���� ]
	_uint m_iNumLayers = 0;

	// [ ���� ���� ]
	vector<shared_ptr<CLevel>> m_vecLevels;
};

END