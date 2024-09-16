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
	// [ �ִϸ��̼� ���� ]
	weak_ptr<CAnimModel> m_wpAnimModel;

	// [ �ִϸ��̼� �̸� ]
	_char m_szName[MAX_PATH] = "";

	// [ ��ü ���� �ð� ]
	_double m_Duration = 0;

	// [ �ʴ� ��� �ӵ� ]
	_double m_TickPerSecond = 0;

	// [ ���� �������� �ð� ] += m_TickPerSecond
	_double m_TrackPosition = 0;

	// [ �ִϸ��̼��� ����ϴ� �� ]
	_uint m_iNumChannels = 0;
	vector<shared_ptr<CChannel>> m_vecChannels = {};

	// [ �� �ִϸ��̼��� ����ϴ� �� �� �� ��° Ű �������� ���� ���������� ]
	vector<_uint> m_vecCurrentKeyFrames;

	// [ ���� ���� ]
	_bool m_bLoop = false;

	// [ ���� ���� ]
	_bool m_bFinished = false;
};

END