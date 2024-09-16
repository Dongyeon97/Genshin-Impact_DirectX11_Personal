#pragma once
#include "NormalMonster.h"

class CHili_CrossBow;

class CHili_Ice final : public CNormalMonster
{
public:
	explicit CHili_Ice() = default;
	virtual ~CHili_Ice() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize();
	virtual _int PreUpdate(const _float& _fTimeDelta);
	virtual _int Update(const _float& _fTimeDelta);
	virtual _int LateUpdate(const _float& _fTimeDelta);
	virtual void Render();
	virtual void EndPlay();
	virtual void Release();

/********************
	Methods
********************/
public:
	virtual void SetActorTranslation(_fvector _vPosition, _int* _iCurNaviCellIndex) override;

public:
	void ReadyBoneMatrixToWeapon();


/********************
	Data Members
********************/
private:
	// [ ���� ������Ʈ - ���� ]
	weak_ptr<CHili_CrossBow> m_wpCrossBow;

	// [ ���Ⱑ ������ �� �� ]
	const _float4x4* m_pSocketMatrix_Arm = nullptr;
};
