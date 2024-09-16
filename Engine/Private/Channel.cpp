#include "Engine_pch.h"
#include "Channel.h"

#include "CameraManager.h"

#include "Layer.h"
#include "Actor.h"

#include "AnimModel.h"
#include "Animation.h"
#include "Bone.h"
#include "CameraComponent.h"

shared_ptr<CChannel> CChannel::Create(const MY_NODEANIM* _pAIChannel, shared_ptr<CAnimModel> _spModel)
{
	shared_ptr<CChannel> spInstance = make_shared<CChannel>();

	if (FAILED(spInstance->Initialize(_pAIChannel, _spModel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		spInstance.reset();
		return nullptr;
	}

	return spInstance;
}

HRESULT CChannel::Initialize(const MY_NODEANIM* _pAIChannel, shared_ptr<CAnimModel> _spModel)
{
	m_wpAnimModel = _spModel;

	strcpy_s(m_szName, _pAIChannel->szName);

	m_iBoneIndex = _spModel->GetBoneIndex(m_szName);

	m_iNumKeyFrames = max(_pAIChannel->iNumScalingKeys, _pAIChannel->iNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, _pAIChannel->iNumPositionKeys);

	_float3			vScale;
	_float4			vRotation;
	_float3			vPosition;

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		KEYFRAME		KeyFrame = {};

		if (_pAIChannel->iNumScalingKeys > i)
		{
			memcpy(&vScale, &_pAIChannel->vecScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = _pAIChannel->vecScalingKeys[i].mTime;
		}

		if (_pAIChannel->iNumRotationKeys > i)
		{
			vRotation.x = _pAIChannel->vecRotationKeys[i].mValue.x;
			vRotation.y = _pAIChannel->vecRotationKeys[i].mValue.y;
			vRotation.z = _pAIChannel->vecRotationKeys[i].mValue.z;
			vRotation.w = _pAIChannel->vecRotationKeys[i].mValue.w;
			KeyFrame.Time = _pAIChannel->vecRotationKeys[i].mTime;
		}

		if (_pAIChannel->iNumPositionKeys > i)
		{
			memcpy(&vPosition, &_pAIChannel->vecPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = _pAIChannel->vecPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;

		m_vecKeyFrames.push_back(KeyFrame);
	}

	return S_OK;
}

// �� ������ ȣ��
// �Ű�����( [Ʈ�� ������], [�� ���� ���� Ű ������ �ε���], [��ü �� �迭])
void CChannel::Invalidate_TransformationMatrix(_float _fTimeDelta, _double _TrackPosition, _uint* _pCurrentKeyFrame, vector<shared_ptr<CBone>>& vecBones, _uint* _iLastIndex, _uint* _iCurIndex)
{
	if (*_iCurIndex != *_iLastIndex)
	{
		Invalidate_AnimToAnim(_fTimeDelta, _TrackPosition, _pCurrentKeyFrame, vecBones, _iLastIndex, _iCurIndex);
	}
	else
	{
		m_fInterpolationTime = 0.0f;

		Invalidate_FrameToFrame(_fTimeDelta, _TrackPosition, _pCurrentKeyFrame, vecBones);
	}
}

void CChannel::Invalidate_FrameToFrame(_float _fTimeDelta, _double _TrackPosition, _uint* _pCurrentKeyFrame, vector<shared_ptr<CBone>>& vecBones)
{
	if (0.0 == _TrackPosition)
	{
		*_pCurrentKeyFrame = 0;
	}

	// �� �ִϸ��̼��� ������ Ű ������
	KEYFRAME		LastKeyFrame = m_vecKeyFrames.back();
	_matrix			TransformationMatrix;
	_vector			vScale, vRotation, vTranslation;

	if (_TrackPosition >= LastKeyFrame.Time)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vPosition), 1.f);
	}
	else
	{
		while (_TrackPosition >= m_vecKeyFrames[*_pCurrentKeyFrame + 1].Time)
			++*_pCurrentKeyFrame;

		_double		Ratio = (_TrackPosition - m_vecKeyFrames[*_pCurrentKeyFrame].Time) /
			(m_vecKeyFrames[*_pCurrentKeyFrame + 1].Time - m_vecKeyFrames[*_pCurrentKeyFrame].Time);

		vScale = XMVectorLerp(XMLoadFloat3(&m_vecKeyFrames[*_pCurrentKeyFrame].vScale), XMLoadFloat3(&m_vecKeyFrames[*_pCurrentKeyFrame + 1].vScale), (_float)Ratio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_vecKeyFrames[*_pCurrentKeyFrame].vRotation), XMLoadFloat4(&m_vecKeyFrames[*_pCurrentKeyFrame + 1].vRotation), (_float)Ratio);
		vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_vecKeyFrames[*_pCurrentKeyFrame].vPosition), XMLoadFloat3(&m_vecKeyFrames[*_pCurrentKeyFrame + 1].vPosition), (_float)Ratio), 1.f);
	}

	Interpolation(&TransformationMatrix, vScale, vRotation, vTranslation, vecBones);
}

void CChannel::Invalidate_AnimToAnim(_float _fTimeDelta, _double _TrackPosition, _uint* _pCurrentKeyFrame, vector<shared_ptr<CBone>>& vecBones, _uint* _iLastIndex, _uint* _iCurIndex)
{
	// �� �ִϸ��̼� ������
	if (0.0 == _TrackPosition)
	{
		*_pCurrentKeyFrame = 0;
		m_fInterpolationTime = 0.0f;
	}

	// ���� �ð� ����
	if (m_fInterpolationTime > m_fInterpolationDuration)
	{
		m_fInterpolationTime = 0.0f;
		*_iLastIndex = *_iCurIndex;

		return;
	}

	m_fInterpolationTime += m_fInterpolationRatio * _fTimeDelta;

	_matrix			TransformationMatrix;
	_vector			vScale, vRotation, vTranslation;
	_double			Ratio;

	KEYFRAME PrevKeyFrame = vecBones[m_iBoneIndex]->GetPrevKeyFrame();

	Ratio = m_fInterpolationTime / m_fInterpolationDuration;

	vScale = XMVectorLerp(XMLoadFloat3(&PrevKeyFrame.vScale), XMLoadFloat3(&m_vecKeyFrames[*_pCurrentKeyFrame].vScale), (_float)Ratio);
	vRotation = XMQuaternionSlerp(XMLoadFloat4(&PrevKeyFrame.vRotation), XMLoadFloat4(&m_vecKeyFrames[*_pCurrentKeyFrame].vRotation), (_float)Ratio);
	vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&PrevKeyFrame.vPosition), XMLoadFloat3(&m_vecKeyFrames[*_pCurrentKeyFrame].vPosition), (_float)Ratio), 1.f);

	Interpolation(&TransformationMatrix, vScale, vRotation, vTranslation, vecBones);
}

void CChannel::Interpolation(_matrix* TransformationMatrix, _vector _vScale, _vector _vRotation, _vector _vTranslation, vector<shared_ptr<CBone>>& vecBones)
{
	_uint iLayerIndex = m_wpAnimModel.lock()->GetOwnerActor().lock()->GetLayer().lock()->GetLayerIndex();

	// 1. �÷��̾� ĳ���� -> ��Ʈ ����� ���
	if (0 == iLayerIndex && 0 == strcmp("Bip001", vecBones[m_iBoneIndex]->GetBoneName()))
	{
		*TransformationMatrix = XMMatrixAffineTransformation(_vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), _vRotation, _vTranslation);
		vecBones[m_iBoneIndex]->SetTranslationMatrix(*TransformationMatrix);

		SavePrevKeyFrame(_vScale, _vRotation, _vTranslation, vecBones);
		return;
	}

	// 2. ����, ��Ʈ ����� ���
	if (0 != iLayerIndex && (0 == strcmp("Bone_Root", vecBones[m_iBoneIndex]->GetBoneName()) || 0 == strcmp("Bip001", vecBones[m_iBoneIndex]->GetBoneName())))
	{
		// 2.1 ��Ʈ����� �������� ���, �������� ������ �� �����Ѵ�.
		if (m_bIsRootMotion)
		{
			_vector vRatioTranslate = XMVectorSet(XMVectorGetX(_vTranslation) * m_fRootRatioX, XMVectorGetY(_vTranslation) * m_fRootRatioY, XMVectorGetZ(_vTranslation) * m_fRootRatioZ, 1.0f);

			*TransformationMatrix = XMMatrixAffineTransformation(_vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), _vRotation, vRatioTranslate);
			vecBones[m_iBoneIndex]->SetTranslationMatrix(*TransformationMatrix);

			SavePrevKeyFrame(_vScale, _vRotation, vRatioTranslate, vecBones);
			return;
		}
		// 2.2 ��Ʈ����� �������� ���, ������ ���� �������� �����Ѵ�. -> Default = 0
		else
		{
			*TransformationMatrix = XMMatrixAffineTransformation(_vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), _vRotation, XMVectorSet(0.0f, m_fRootRatioY, 0.0f, 1.0f));
			vecBones[m_iBoneIndex]->SetTranslationMatrix(*TransformationMatrix);

			SavePrevKeyFrame(_vScale, _vRotation, XMVectorSet(0.0f, m_fRootRatioY, 0.0f, 1.0f), vecBones);
			return;
		}
	}

	// 2. ��Ʈ��尡 �ƴ� ���
	*TransformationMatrix = XMMatrixAffineTransformation(_vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), _vRotation, _vTranslation);
	vecBones[m_iBoneIndex]->SetTranslationMatrix(*TransformationMatrix);

	SavePrevKeyFrame(_vScale, _vRotation, _vTranslation, vecBones);
}

void CChannel::MoveActorToAnimDir(_vector _vTranslation)
{
	// ���� ������ ���ؼ�
	_vector vActorPos = m_wpAnimModel.lock()->GetOwnerActor().lock()->GetActorTranslation();

	// �ִϸ��̼��� �÷��̾��� ���⺤�͸� ���ϰԲ� �ϰ�
	_vector vOriginalDir = CalculateAnimRotationToActorDir(_vTranslation);

	// ���͸� �� �������� �����̰Բ� �Ѵ�.
	m_wpAnimModel.lock()->GetOwnerActor().lock()->SetActorTranslation(vActorPos + vOriginalDir / (_float)m_iNumKeyFrames, &m_wpAnimModel.lock()->GetOwnerActor().lock()->GetCurNaviCellIndex());
}

_vector CChannel::CalculateAnimRotationToActorDir(_vector _vTranslation)
{
	// �ִϸ��̼� �̵����⺤��	
	_vector vOriginalDir = XMVectorSet(XMVectorGetX(_vTranslation), XMVectorGetZ(_vTranslation), XMVectorGetY(_vTranslation), 0.0f);
	_vector vOriginalDirNormal;
	vOriginalDirNormal = XMVector4Normalize(XMVectorSetY(vOriginalDir, 0.0f));

	// �÷��̾� ���⺤�� ��������, y���� �����ϰ� ����ȭ
	_vector vActorDir = m_wpAnimModel.lock()->GetOwnerActor().lock()->GetLookVector();
	vActorDir = XMVector4Normalize(XMVectorSetY(vActorDir, 0.0f));

	// �����Ͽ� ���� ���� ����
	_float fAngle = acos(XMVector4Dot(vOriginalDirNormal, vActorDir).m128_f32[0]) * (180.0f / DirectX::XM_PI);

	// ������ ������ ���ٸ� ������ 180�� �����̴�.
	if (XMVectorGetY(XMVector3Cross(vActorDir, vOriginalDirNormal)) > 0)
	{
		fAngle = 360.0f - fAngle;
	}

	// �ִϸ��̼��� ���� ������ �÷��̾� Look�������� ������.
	_matrix matRotateYToActorDir = XMMatrixRotationY(fAngle);
	vOriginalDir = XMVector4Transform(vOriginalDir, matRotateYToActorDir);

	return vOriginalDir;
}

void CChannel::SavePrevKeyFrame(_vector _vScale, _vector _vRotation, _vector _vTranslation, vector<shared_ptr<CBone>>& vecBones)
{
	XMStoreFloat3(&m_PrevKeyFrame.vScale, _vScale);
	XMStoreFloat4(&m_PrevKeyFrame.vRotation, _vRotation);
	XMStoreFloat3(&m_PrevKeyFrame.vPosition, _vTranslation);

	vecBones[m_iBoneIndex]->SetPrevKeyFrame(m_PrevKeyFrame);
}

void CChannel::Release()
{
}