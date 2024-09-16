#pragma once
#include "Level.h"

/* �� Ŭ������, ������ �Ѿ�� ���� Ÿ�Կ� �°� Ŭ�󿡼� ���ǵ� ���� Ŭ������ �����ϱ� ����, */
/* LoadLevelFromFile �Լ��� �������̵� �ϱ� ���Ͽ� �����Ͽ���. -01.31- */

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

