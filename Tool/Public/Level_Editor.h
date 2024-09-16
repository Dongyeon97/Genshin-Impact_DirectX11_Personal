#pragma once
#include "Level.h"

class CInspector;
class CSelectedActorWindow;

class CLevel_Editor : public CLevel
{
public:
	explicit CLevel_Editor() = default;
	virtual ~CLevel_Editor() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize(_uint _iNumLayers);
	virtual _int PreUpdate(const _float& _fTimeDelta);
	virtual _int Update(const _float& _fTimeDelta);
	virtual _int LateUpdate(const _float& _fTimeDelta);
	virtual void Render();
	virtual void EndPlay();
	virtual void Release();

public:
	void InitializeObject();
	void ReleaseObject();


	static std::shared_ptr<CLevel_Editor> Create(_uint _iNumLayers)
	{
		shared_ptr<CLevel_Editor> spLevel(make_shared<CLevel_Editor>());
		spLevel->SetLevelIndex(1);
		spLevel->ReserveLayers(_iNumLayers);

		if (!spLevel)
			MSG_BOX("Failed to Initialize Stage1 at CLevel_Stage1");

		return spLevel;
	}

public:
	shared_ptr<CInspector> GetInspector() { return m_spInspector; }
	shared_ptr<CSelectedActorWindow> GetSelectedWindow() { return m_spSelectedActorWindow; }

private:
	virtual HRESULT ReadyLights() override;

protected:
	shared_ptr<CInspector> m_spInspector;
	shared_ptr<CSelectedActorWindow> m_spSelectedActorWindow;
};
