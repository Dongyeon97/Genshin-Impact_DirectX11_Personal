#pragma once
#include "Level.h"

class CLoader;

/* 생성 시, 전달받은 index해당하는 레벨용 자원을 준비하기위한 로더객체를 생성한다. . */
/* 동시에 로딩화면을 보여준다. */
class CLevel_Loading final : public CLevel
{
public:
	explicit CLevel_Loading() = default;
	virtual ~CLevel_Loading() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize(_uint _iNumLayers) override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	virtual void Render() override;
	virtual void EndPlay() override;
	virtual void Release() override;

	static std::shared_ptr<CLevel_Loading> Create(LEVEL _eNextLevelIndex, _uint _iNumLayers)
	{
		shared_ptr<CLevel_Loading> spLevel(make_shared<CLevel_Loading>());
		spLevel->SetLevelIndex(LEVEL_LOADING);
		spLevel->ReserveLayers(_iNumLayers);
		spLevel->SetNextLevelIndex(_eNextLevelIndex);

		if (!spLevel)
			MSG_BOX("Failed to Initialize Loading at CLevel_Loading");

		return spLevel;
	}
/********************
	Methods
********************/
public:
	HRESULT Ready_Layer_BackGround();
	HRESULT Ready_Layer_UI();

public:
	void SetNextLevelIndex(LEVEL _eNextLevelIndex) { m_eNextLevelIndex = _eNextLevelIndex; }

/********************
	Data Members
********************/
private:
	// 로더
	std::shared_ptr<CLoader> m_spLoader;

	//
	static _uint g_iLoadingCount;

	// 로딩을 할 대상 레벨 이름
	LEVEL m_eNextLevelIndex = Client::LEVEL_END;
};
