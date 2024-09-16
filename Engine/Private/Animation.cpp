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

	/* �� �ִϸ��̼��� ��ü ����. */
	m_Duration = _pAIAnimation->Duration;

	/* �ʴ� ��� �ӵ� .*/
	m_TickPerSecond = _pAIAnimation->TickPerSecond;

	/* �� �ִϸ��̼��� �����ϴµ� �ʿ��� ���� ����. */
	m_iNumChannels = _pAIAnimation->iNumChannels;

	m_vecCurrentKeyFrames.resize(m_iNumChannels);

	/* �� �ִϸ��̼��� �����ϴµ� �ʿ��� ������ ������ �����Ѵ�.*/
	/* ������ ����? : */
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
	// ���� �ִϸ��̼��� ������ �����ٸ�
	if (m_TrackPosition >= m_Duration)
	{
		// ���� �ִϸ��̼��� �ƴ϶�� ������, Ʈ�� �������� 0���� �ʱ�ȭ�Ѵ�.
		if (false == m_bLoop)
		{
			m_bFinished = true;
			m_TrackPosition = 0.0;

			return;
		}
		// ���� �ִϸ��̼��̸� ��� ����.
		else
		{
			m_TrackPosition = 0.0;
		}
	}
	// ������ ��ġ�� �� ������ �����Ѵ�.
	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		// �Ű����� : ( Ʈ�� ������, i��° Ű ������, �� �迭 )
		m_vecChannels[i]->Invalidate_TransformationMatrix(_fTimeDelta, m_TrackPosition, &m_vecCurrentKeyFrames[i], _vecBones, _iLastIndex, _iCurIndex);
	}

	// [Ʈ�� ������]�� [ƽ �� ������]�� �����س�����.
	m_TrackPosition += m_TickPerSecond * _fTimeDelta;
}
