#pragma once

BEGIN(Engine)

class ENGINE_DLL CBone final
{
public:
	explicit CBone() = default;
	virtual ~CBone() = default;

/********************
	Framework
********************/
public:
	HRESULT Initialize(const MY_AINODE* pAIBone, _int iParentBoneIndex, _fmatrix PivotMatrix);
	void Release();
	
/********************
	Methods
********************/
public:
	static shared_ptr<CBone> Create(const MY_AINODE* pAIBone, _int iParentBoneIndex, _fmatrix PivotMatrix);

public:
	void InvalidateCombinedTransformationMatrix(const vector<shared_ptr<CBone>>& _vecBones);

/********************
	Getter / Setter
********************/
public:
	const _char* GetBoneName() const { return m_szName; }

	const _float4x4* GetCombinedTransformationMatrix() { return &m_CombinedTransformationMatrix; }

	void SetTranslationMatrix(_fmatrix _TranslationMatrix) { XMStoreFloat4x4(&m_TransformationMatrix, _TranslationMatrix); }

	_fmatrix GetTranslationMatrix() { return XMLoadFloat4x4(&m_TransformationMatrix); }

	const KEYFRAME& GetPrevKeyFrame() { return m_PrevKeyFrame; }
	void SetPrevKeyFrame(const KEYFRAME& _KeyFrame) { m_PrevKeyFrame = _KeyFrame; }

/********************
	Data Members
********************/
private:
	// [뼈의 이름]
	_char			m_szName[MAX_PATH] = "";

	// [부모를 기준으로 표현된 나만의 상태 행렬]
	_float4x4		m_TransformationMatrix;

	// [나의 TransformationMatrix * 부모`s m_CombinedTransformationMatrix]
	_float4x4		m_CombinedTransformationMatrix;

	// [총 뼈에서의 부모 뼈의 인덱스]
	_int			m_iParentBoneIndex = { 0 };

	// [ 이전에 시행되었던 키프레임 ]
	KEYFRAME m_PrevKeyFrame = {};
};

END