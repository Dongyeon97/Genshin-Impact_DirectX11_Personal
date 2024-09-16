#include "Engine_pch.h"
#include "Animation.h"

#include "Actor.h"
#include "Channel.h"
#include "Bone.h"

shared_ptr<CAnimation> CAnimation::Create(const MY_ANIMATION* _pAIAnimation, shared_ptr<CAnimModel> _spModel)
{
	shared_ptr<CAnimation> spInstance = make_shared<CAnimation>();

	if (FAILED(spInstance->Initialize(_pAIAnimation, _spModel)))
	{
		MSG_BOX("Failed to Created : CAnimation");
		spInstance.reset();
		return nullptr;
	}

	return spInstance;
}

HRESULT CAnimation::Initialize(const MY_ANIMATION* _pAIAnimation, shared_ptr<CAnimModel> _spAnimModel)
{
	m_wpAnimModel = _spAnimModel;

	strcpy_s(m_szName, _pAIAnimation->szName);

	/* 내 애니메이션의 전체 길이. */
	m_Duration = _pAIAnimation->Duration;

	/* 초당 재생 속도 .*/
	m_TickPerSecond = _pAIAnimation->TickPerSecond;

	/* 이 애니메이션을 구동하는데 필요한 뼈의 갯수. */
	m_iNumChannels = _pAIAnimation->iNumChannels;

	m_vecCurrentKeyFrames.resize(m_iNumChannels);

	/* 이 애니메이션을 구동하는데 필요한 뼈들의 정보를 생성한다.*/
	/* 뼈들의 정보? : */
	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		shared_ptr<CChannel> spChannel = CChannel::Create(&_pAIAnimation->vecChannels[i], _spAnimModel);
		if (nullptr == spChannel)
			return E_FAIL;

		m_vecChannels.push_back(spChannel);
	}

 	return S_OK;
}

void CAnimation::Release()
{
	for (auto& spChannel : m_vecChannels)
	{
		spChannel->Release();
		spChannel.reset();
	}
	m_vecChannels.clear();
}

void CAnimation::SetIsRootMotion(_bool _bIsRootMotion)
{
	for (auto& channel : m_vecChannels)
	{
		channel->SetIsRootMotion(_bIsRootMotion);
	}
}

void CAnimation::SetRootMotionRatio(_float3 _vRatio)
{
	for (auto& channel : m_vecChannels)
	{
		channel->SetRootMotionRatio(_vRatio);
	}
}

void CAnimation::SetInterpolationDuration(_float _fDuration)
{
	for (auto& channel : m_vecChannels)
	{
		channel->SetInterpolationDuration(_fDuration);
	}
}

void CAnimation::SetInterpolationRatio(_float _fRatio)
{
	for (auto& channel : m_vecChannels)
	{
		channel->SetInterpolationRatio(_fRatio);
	}
}

void CAnimation::Invalidate_TransformationMatrix(_float _fTimeDelta, vector<shared_ptr<CBone>>& _vecBones, _uint* _iLastIndex, _uint* _iCurIndex)
{
	// 만약 애니메이션의 진행이 끝났다면
	if (m_TrackPosition >= m_Duration)
	{
		// 루프 애니메이션이 아니라면 끝내고, 트랙 포지션을 0으로 초기화한다.
		if (false == m_bLoop)
		{
			m_bFinished = true;
			m_TrackPosition = 0.0;

			return;
		}
		// 루프 애니메이션이면 계속 돈다.
		else
		{
			m_TrackPosition = 0.0;
		}
	}
	// 뼈들의 위치를 매 프레임 갱신한다.
	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		// 매개변수 : ( 트랙 포지션, i번째 키 프레임, 뼈 배열 )
		m_vecChannels[i]->Invalidate_TransformationMatrix(_fTimeDelta, m_TrackPosition, &m_vecCurrentKeyFrames[i], _vecBones, _iLastIndex, _iCurIndex);
	}

	// [트랙 포지션]에 [틱 퍼 세컨드]를 누적해나간다.
	m_TrackPosition += m_TickPerSecond * _fTimeDelta;
}
