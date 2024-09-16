#pragma once
#include "ModelComponent.h"

BEGIN(Engine)

class ENGINE_DLL CNonAnimModel : public CModelComponent
{
public:
	explicit CNonAnimModel() = default;
	virtual ~CNonAnimModel() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	void Render(_uint _iMeshIndex);
	virtual void EndPlay() override;
	virtual void Release() override;

/********************
	Methods
********************/
protected:
	// 모델의 구성 요소들을 준비합니다.
	virtual HRESULT ReadyMeshes();
	virtual HRESULT ReadyMaterials();

/********************
	Data Members
********************/
protected:

};

END