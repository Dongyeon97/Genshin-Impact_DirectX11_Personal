#pragma once
#include "ModelComponent.h"

BEGIN(Engine)

class CBone;
class CAnimation;

class ENGINE_DLL CAnimModel: public CModelComponent
{
public:
	explicit CAnimModel() = default;
	virtual ~CAnimModel() = default;

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
public:
	// [ 애니메이션 재생 ]
	void PlayAnimation(_float _fTimeDelta, _uint* _iLastIndex, _uint* _iCurIndex);

public:
	// [ 메시에 본 바인딩 ]
	HRESULT BindBoneMatrices(const _char* _pConstantName, _uint _iMeshIndex);

protected:
	// [ 모델의 구성 요소 준비 ]
	virtual HRESULT ReadyMeshes();
	virtual HRESULT ReadyMaterials();

protected:
	HRESULT ReadyBones(MY_AINODE* _pNode, _int iParentBoneIndex);
	HRESULT ReadyAnimations(MY_AISCENE* _pAIScene);

/********************
	Getter / Setter
********************/
public:
	// [ 애니메이션 벡터 ]
	vector<shared_ptr<CAnimation>>& GetAnimVector() { return m_vecAnimations; }

	// [ 애니메이션 개수 ]
	_uint GetNumAnimations() { return m_iNumAnimations; }

	// [ 애니메이션 인덱스 ]
	void SetAnimationIndex(_uint _iAnimIndex);
	_uint GetAnimationIndex() { return m_iCurrentAnimationIndex; }

	// [ 애니메이션 활성화 ]
	void SetIsAnimate(_bool _bAnimate) { m_bAnimate = _bAnimate; }
	_bool GetIsAnimate() { return m_bAnimate; }

	// [ 애니메이션 루프 활성화 ]
	void SetIsLoop(_bool _bLoop, _uint _iAnimationIndex);
	_bool GetIsLoop(_uint _iAnimationIndex);

	// [ 루트 모션 옵션 ]
	void SetIsRootMotion(_bool _bRootMotionEnable, _uint _iAnimIndex);
	void SetRootMotionRatio(_float3 _vRatio, _uint _iAnimIndex);

	// [ 보간 옵션 ]
	void SetInterPolationDuration(_float _fDuration, _uint _iAnimIndex);
	void SetInterPolationRatio(_float _fRatio, _uint _iAnimIndex);
	
	// [ 본 ]
	_int GetBoneIndex(const _char* _pBoneName) const;
	const _float4x4* GetCombinedBoneMatrixPointer(const _char* _pBoneName);

/********************
	Data Members
********************/
protected:
	// [ 모델의 모든 뼈들 ]
	vector<shared_ptr<CBone>> m_vecBones;

	// [ 애니메이션 ]
	vector<shared_ptr<CAnimation>> m_vecAnimations;

	// [ 애니메이션 개수 ]
	_uint m_iNumAnimations = 0;

	// [ 현재 애니메이션 인덱스 ]
	_uint m_iCurrentAnimationIndex = 0;

	// [ 이전 애니메이션 인덱스 ]
	_uint m_iLastAnimationIndex = 0;

	// [ 애니메이션 재생 여부 ]
	_bool m_bAnimate = true;
};

END