#pragma once

BEGIN(Engine)

class CAnimModel;
class CBone;

class ENGINE_DLL CChannel final
{
public:
	explicit CChannel() = default;
	virtual ~CChannel() = default;

/********************
	Framework
********************/
public:
	static shared_ptr<CChannel> Create(const MY_NODEANIM* _pAIChannel, shared_ptr<CAnimModel> _spModel);

public:
	HRESULT Initialize(const MY_NODEANIM* _pAIChannel, shared_ptr<CAnimModel> _spModel);
	void Release();

/********************
	Methods
********************/
public:
	void Invalidate_TransformationMatrix(_float _fTimeDelta, _double _TrackPosition, _uint* _pCurrentKeyFrame, vector<shared_ptr<CBone>>& vecBones, _uint* _iLastIndex, _uint* _iCurIndex);

private:
	void Invalidate_AnimToAnim(_float _fTimeDelta, _double _TrackPosition, _uint* _pCurrentKeyFrame, vector<shared_ptr<CBone>>& vecBones, _uint* _iLastIndex, _uint* _iCurIndex);
	void Invalidate_FrameToFrame(_float _fTimeDelta, _double _TrackPosition, _uint* _pCurrentKeyFrame, vector<shared_ptr<CBone>>& vecBones);

private:
	_vector CalculateAnimRotationToActorDir(_vector _vTranslation);

	void MoveActorToAnimDir(_vector _vAnimDir);
	void SavePrevKeyFrame(_vector _vScale, _vector _vRotation, _vector _vTranslation, vector<shared_ptr<CBone>>& vecBones);

	void Interpolation(_matrix* TransformationMatrix, _vector _vScale, _vector _vRotation, _vector _vTranslation, vector<shared_ptr<CBone>>& vecBones);

/********************
	Getter / Setter
********************/
public:
	void SetIsRootMotion(_bool _bIsRootMotion) { m_bIsRootMotion = _bIsRootMotion; }
	void SetRootMotionRatio(_float3 _vRatio) 
	{ 
		m_fRootRatioX = _vRatio.x;
		m_fRootRatioY = _vRatio.y;
		m_fRootRatioZ = _vRatio.z;
	}
	
	void SetInterpolationDuration(_float _fDuration) { m_fInterpolationDuration = _fDuration; }
	void SetInterpolationRatio(_float _fRatio) { m_fInterpolationRatio = _fRatio; }

	_uint GetBoneIndex() { return m_iBoneIndex; }

	KEYFRAME& GetPrevKeyFrame() { return m_PrevKeyFrame; }

/********************
	Data Members
********************/
private:
	// [ 애님 모델 ]
	weak_ptr<CAnimModel> m_wpAnimModel;

	// [ 뼈 이름 ]
	_char m_szName[MAX_PATH] = "";

	// [ 전체 뼈 중에서의 인덱스 ]
	_uint m_iBoneIndex = 0;

	// [ 키프레임 수 ]
	_uint m_iNumKeyFrames = 0;

	// [ 키프레임 ]
	vector<KEYFRAME> m_vecKeyFrames;

// == 애니메이션 보간용 ==
	
	// [ 루트모션 적용 여부 ]
	_bool m_bIsRootMotion = true;

	// [ 루트모션 적용 비율 ]
	_float m_fRootRatioX = 1.0f;
	_float m_fRootRatioY = 1.0f;
	_float m_fRootRatioZ = 1.0f;

	// [ 수행된 마지막 키프레임 ]
	KEYFRAME m_PrevKeyFrame = {};

	// [ 보간 시간 ] - 전체
	_float m_fInterpolationDuration = 0.1f; // 기본 값 0.1f;

	// [ 보간 시간 ] - 누적
	_float m_fInterpolationTime = 0.0f;

	// [ 보간 강도 ]
	_float m_fInterpolationRatio = 1.0f; // 기본 값 1.0f;
};

END