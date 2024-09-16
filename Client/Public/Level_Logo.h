#pragma once
#include "Level.h"

class CLevel_Logo final : public CLevel
{
public:
	explicit CLevel_Logo() = default;
	virtual ~CLevel_Logo() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize(_uint _iNumLayers) override;
	virtual _int PreUpdate(const float& _fTimeDelta) override;
	virtual _int Update(const float& _fTimeDelta) override;
	virtual _int LateUpdate(const float& _fTimeDelta) override;
	virtual void Render() override;
	virtual void EndPlay() override;
	virtual void Release() override;

	static std::shared_ptr<CLevel_Logo> Create(_uint _iNumLayers)
	{
		shared_ptr<CLevel_Logo> spLevel(make_shared<CLevel_Logo>());
		spLevel->SetLevelIndex(LEVEL_LOGO);
		spLevel->ReserveLayers(_iNumLayers);

		if (!spLevel)
			MSG_BOX("Failed to Initialize Logo at CLevel_Logo");

		return spLevel;
	}
};