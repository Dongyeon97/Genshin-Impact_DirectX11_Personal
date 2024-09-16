#pragma once
#include "ClientLevel.h"

class CDungeonExit;

class CLevel_Stage3 : public CClientLevel
{
public:
	explicit CLevel_Stage3() = default;
	virtual ~CLevel_Stage3() = default;

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
	static std::shared_ptr<CLevel_Stage3> Create(_uint _iNumLayers)
	{
		shared_ptr<CLevel_Stage3> spLevel(make_shared<CLevel_Stage3>());
		spLevel->SetLevelIndex(LEVEL_STAGE3);
		spLevel->ReserveLayers(_iNumLayers);

		if (!spLevel)
			MSG_BOX("Failed to Initialize Stage1 at CLevel_Stage3");

		return spLevel;
	}

private:
	virtual HRESULT ReadyLights() override;
	HRESULT ReadyDungeonExit();

private:
	_uint CalculateActivatedMonsterNum();

/********************
	Data Members
********************/
private:
	weak_ptr<CDungeonExit> m_wpDungeonExit_Seal;
	weak_ptr<CDungeonExit> m_wpDungeonExit_Floor;

private:
	_float m_fStageLimitedTime = 480.0f;

	// юс╫ц
	_bool m_bTemp = false;
};

