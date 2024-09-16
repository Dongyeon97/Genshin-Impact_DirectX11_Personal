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
	// [ �ִϸ��̼� ��� ]
	void PlayAnimation(_float _fTimeDelta, _uint* _iLastIndex, _uint* _iCurIndex);

public:
	// [ �޽ÿ� �� ���ε� ]
	HRESULT BindBoneMatrices(const _char* _pConstantName, _uint _iMeshIndex);

protected:
	// [ ���� ���� ��� �غ� ]
	virtual HRESULT ReadyMeshes();
	virtual HRESULT ReadyMaterials();

protected:
	HRESULT ReadyBones(MY_AINODE* _pNode, _int iParentBoneIndex);
	HRESULT ReadyAnimations(MY_AISCENE* _pAIScene);

/********************
	Getter / Setter
********************/
public:
	// [ �ִϸ��̼� ���� ]
	vector<shared_ptr<CAnimation>>& GetAnimVector() { return m_vecAnimations; }

	// [ �ִϸ��̼� ���� ]
	_uint GetNumAnimations() { return m_iNumAnimations; }

	// [ �ִϸ��̼� �ε��� ]
	void SetAnimationIndex(_uint _iAnimIndex);
	_uint GetAnimationIndex() { return m_iCurrentAnimationIndex; }

	// [ �ִϸ��̼� Ȱ��ȭ ]
	void SetIsAnimate(_bool _bAnimate) { m_bAnimate = _bAnimate; }
	_bool GetIsAnimate() { return m_bAnimate; }

	// [ �ִϸ��̼� ���� Ȱ��ȭ ]
	void SetIsLoop(_bool _bLoop, _uint _iAnimationIndex);
	_bool GetIsLoop(_uint _iAnimationIndex);

	// [ ��Ʈ ��� �ɼ� ]
	void SetIsRootMotion(_bool _bRootMotionEnable, _uint _iAnimIndex);
	void SetRootMotionRatio(_float3 _vRatio, _uint _iAnimIndex);

	// [ ���� �ɼ� ]
	void SetInterPolationDuration(_float _fDuration, _uint _iAnimIndex);
	void SetInterPolationRatio(_float _fRatio, _uint _iAnimIndex);
	
	// [ �� ]
	_int GetBoneIndex(const _char* _pBoneName) const;
	const _float4x4* GetCombinedBoneMatrixPointer(const _char* _pBoneName);

/********************
	Data Members
********************/
protected:
	// [ ���� ��� ���� ]
	vector<shared_ptr<CBone>> m_vecBones;

	// [ �ִϸ��̼� ]
	vector<shared_ptr<CAnimation>> m_vecAnimations;

	// [ �ִϸ��̼� ���� ]
	_uint m_iNumAnimations = 0;

	// [ ���� �ִϸ��̼� �ε��� ]
	_uint m_iCurrentAnimationIndex = 0;

	// [ ���� �ִϸ��̼� �ε��� ]
	_uint m_iLastAnimationIndex = 0;

	// [ �ִϸ��̼� ��� ���� ]
	_bool m_bAnimate = true;
};

END