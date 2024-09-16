#pragma once

BEGIN(Engine)

class CAnimModel;
class CBone;
class CChannel;

class ENGINE_DLL CAnimation final
{
public:
	explicit CAnimation() = default;
	virtual ~CAnimation() = default;

public:
	static shared_ptr<CAnimation> Create(const MY_ANIMATION* _pAIAnimation, shared_ptr<CAnimModel> _spModel);

	HRESULT Initialize(const MY_ANIMATION* _pAIAnimation, shared_ptr<CAnimModel> _spAnimModel);
	void Release();

public:
	vector<shared_ptr<CChannel>>& GetChannelVector() { return m_vecChannels; }

	void SetTrackPosition(_double _TrackPosition) { m_TrackPosition = _TrackPosition; };

	void SetIsRootMotion(_bool _bIsRootMotion);
	void SetRootMotionRatio(_float3 _vRatio);

	void SetInterpolationDuration(_float _fDuration);
	void SetInterpolationRatio(_float _fRatio);

	void SetIsFinished(_bool _bFinished) { m_bFinished = _bFinished; }
	_bool GetIsFinished() { return m_bFinished; }

	void SetIsLoop(_bool _bLoop) { m_bLoop = _bLoop; }
	_bool GetIsLoop() { return m_bLoop; }

public:
	void Invalidate_TransformationMatrix(_float _fTimeDelta, vector<shared_ptr<CBone>>& _vecBones, _uint *_iLastIndex, _uint *_iCurIndex);

private:
	// [ 애니메이션 주인 ]
	weak_ptr<CAnimModel> m_wpAnimModel;

	// [ 애니메이션 이름 ]
	_char m_szName[MAX_PATH] = "";

	// [ 전체 실행 시간 ]
	_double m_Duration = 0;

	// [ 초당 재생 속도 ]
	_double m_TickPerSecond = 0;

	// [ 현재 진행중인 시간 ] += m_TickPerSecond
	_double m_TrackPosition = 0;

	// [ 애니메이션이 사용하는 뼈 ]
	_uint m_iNumChannels = 0;
	vector<shared_ptr<CChannel>> m_vecChannels = {};

	// [ 이 애니메이션이 사용하는 뼈 가 몇 번째 키 프레임을 현재 진행중인지 ]
	vector<_uint> m_vecCurrentKeyFrames;

	// [ 루프 여부 ]
	_bool m_bLoop = false;

	// [ 종료 여부 ]
	_bool m_bFinished = false;
};

END