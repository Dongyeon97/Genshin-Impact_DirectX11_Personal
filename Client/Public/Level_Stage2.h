#pragma once
#include "ClientLevel.h"

class CDungeonExit;

class CLevel_Stage2 : public CClientLevel
{
public:
	explicit CLevel_Stage2() = default;
	virtual ~CLevel_Stage2() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize(_uint _iNumLayers) override;
	virtual _int PreUpdate(const _float& _fTimeDelte) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	virtual void Render() override;
	virtual void EndPlay() override;
	virtual void Release() override;

/********************
	Methods
********************/
public:
#pragma region Stage2
	HRESULT Stage2_SpawnWave1();
	HRESULT Stage2_SpawnWave2();

public:
	static std::shared_ptr<CLevel_Stage2> Create(_uint _iNumLayers)
	{
		shared_ptr<CLevel_Stage2> spLevel(make_shared<CLevel_Stage2>());
		spLevel->SetLevelIndex(LEVEL_STAGE2);
		spLevel->ReserveLayers(_iNumLayers);

		if (!spLevel)
			MSG_BOX("Failed to Initialize Stage1 at CLevel_Stage2");

		return spLevel;
	}

private:
	virtual HRESULT ReadyLights() override;
	HRESULT ReadyMonsterKey();
	HRESULT ReadyDungeonExit();

private:
	HRESULT ManagementSpawn();

	_uint CalculateActivatedMonsterNum();

/********************
	Methods
********************/
public:
	void SetWave1Spawned(_bool _bSpawned) { m_bWave1_Spawned = _bSpawned; }
	void SetWave2Spawned(_bool _bSpawned) { m_bWave2_Spawned = _bSpawned; }

/********************
	Data Members
********************/
private:
	weak_ptr<CDungeonExit> m_wpDungeonExit_Seal;
	weak_ptr<CDungeonExit> m_wpDungeonExit_Floor;

private:
	_float m_fStageLimitedTime = 480.0f;

private:
	_bool m_bWave1_Spawned = false;
	_bool m_bWave2_Spawned = false;


	_bool m_bDungeonExitSpawned = false;
};
