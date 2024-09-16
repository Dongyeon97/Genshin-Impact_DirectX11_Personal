#include "Engine_pch.h"
#include "MovingEffect.h"

void CMovingEffect::Initialize()
{
	m_RandomNumber = mt19937_64(m_RandomDevice());

	uniform_real_distribution<float>	SpeedRange(m_vSpeed.x, m_vSpeed.y);
	uniform_real_distribution<float>	RotationRange(-360.f, 360.f);
	uniform_real_distribution<float>	WidthRange(m_vWidthRange.x, m_vWidthRange.y);
	uniform_real_distribution<float>	HeightRange(m_vHeightRange.x, m_vHeightRange.y);
	uniform_real_distribution<float>	DepthRange(m_vDepthRange.x, m_vDepthRange.y);

	m_fSpeed = SpeedRange(m_RandomNumber);

	m_vMoveDir.x = WidthRange(m_RandomNumber);
	m_vMoveDir.y = HeightRange(m_RandomNumber);
	m_vMoveDir.z = DepthRange(m_RandomNumber);

	m_vRandomRotation.x = RotationRange(m_RandomNumber);
	m_vRandomRotation.y = RotationRange(m_RandomNumber);
	m_vRandomRotation.z = RotationRange(m_RandomNumber);

	CEffectMeshActor::Initialize();
}

HRESULT CMovingEffect::Initialize(const wstring& _wstrJsonFileName)
{
	m_RandomNumber = mt19937_64(m_RandomDevice());

	uniform_real_distribution<float>	SpeedRange(m_vSpeed.x, m_vSpeed.y);
	uniform_real_distribution<float>	RotationRange(-360.f, 360.f);
	uniform_real_distribution<float>	WidthRange(m_vWidthRange.x, m_vWidthRange.y);
	uniform_real_distribution<float>	HeightRange(m_vHeightRange.x, m_vHeightRange.y);
	uniform_real_distribution<float>	DepthRange(m_vDepthRange.x, m_vDepthRange.y);

	m_fSpeed = SpeedRange(m_RandomNumber);

	m_vMoveDir.x = WidthRange(m_RandomNumber);
	m_vMoveDir.y = HeightRange(m_RandomNumber);
	m_vMoveDir.z = DepthRange(m_RandomNumber);

	m_vRandomRotation.x = RotationRange(m_RandomNumber);
	m_vRandomRotation.y = RotationRange(m_RandomNumber);
	m_vRandomRotation.z = RotationRange(m_RandomNumber);

	return CEffectMeshActor::Initialize(_wstrJsonFileName);
}

void CMovingEffect::RecycleBegin()
{


	CEffectMeshActor::RecycleBegin();
}

_int CMovingEffect::PreUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::PreUpdate(_fTimeDelta);
}

_int CMovingEffect::Update(const _float& _fTimeDelta)
{
	if (m_fLifeTimeAcc >= m_EffectMeshDesc.vLifeTime.x)
		UpdateMove(_fTimeDelta);

	return CEffectMeshActor::Update(_fTimeDelta);
}

_int CMovingEffect::LateUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::LateUpdate(_fTimeDelta);
}

void CMovingEffect::Render()
{
	CEffectMeshActor::Render();
}

void CMovingEffect::EndPlay()
{
	CEffectMeshActor::EndPlay();
}

void CMovingEffect::Release()
{
	CEffectMeshActor::Release();
}

void CMovingEffect::UpdateMove(const _float& _fTimeDelta)
{
	if (XMVectorGetY(GetActorTranslation()) <= 0.f)
	{
		SetActorTranslation_NoNavigation(XMVectorSetY(GetActorTranslation(), 0.f));
		return;
	}

	if (m_bGravity)
	{
		_float fGravityMoveDistance = - m_fGravityValue * (m_fLifeTimeAcc - m_EffectMeshDesc.vLifeTime.x);
		
		SetActorTranslation_NoNavigation(GetActorTranslation() + XMVector3Normalize(XMLoadFloat3(&m_vMoveDir)) * m_fSpeed + XMVectorSet(0.0f, fGravityMoveDistance, 0.0f, 0.0f));
		SetRotation(XMVectorSet(
			XMVectorGetX(GetRotation()) + m_vRandomRotation.x * _fTimeDelta,
			XMVectorGetY(GetRotation()) + m_vRandomRotation.y * _fTimeDelta,
			XMVectorGetZ(GetRotation()) + m_vRandomRotation.z * _fTimeDelta,
			0.f));
	}
	else
	{
		SetActorTranslation_NoNavigation(GetActorTranslation() + XMVector3Normalize(XMLoadFloat3(&m_vMoveDir)) * m_fSpeed);
		SetRotation(XMVectorSet(
			XMVectorGetX(GetRotation()) + m_vRandomRotation.x * _fTimeDelta,
			XMVectorGetY(GetRotation()) + m_vRandomRotation.y * _fTimeDelta,
			XMVectorGetZ(GetRotation()) + m_vRandomRotation.z * _fTimeDelta,
			0.f));
	}
}
