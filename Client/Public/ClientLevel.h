#pragma once
#include "Level.h"

/* 본 클래스는, 툴에서 넘어온 몬스터 타입에 맞게 클라에서 정의된 몬스터 클래스를 생성하기 위해, */
/* LoadLevelFromFile 함수를 오버라이드 하기 위하여 생성하였음. -01.31- */

class CClientLevel : public CLevel
{
public:
	explicit CClientLevel() = default;
	virtual ~CClientLevel() = default;

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

/********************
	Methods
********************/
private:
	virtual void LoadSpawn(const wstring& _wstrLevelName) override;

protected:
	void InitializeNavigationIndex();

/********************
	Data Members
********************/
private:

};

