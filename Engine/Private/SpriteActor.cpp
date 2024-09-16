#include "Engine_pch.h"
#include "SpriteActor.h"

void CSpriteActor::Initialize()
{
	m_vMinUV = { 0.f, 0.f };
	m_vMaxUV = { 1.f / (_float)m_iRow, 1.f / (_float)m_iCol };

	m_vUVAddValue.x = 1.f / (_float)m_iRow;
	m_vUVAddValue.y = 1.f / (_float)m_iCol;

	CParticleActor::Initialize();
}

HRESULT CSpriteActor::Initialize(const wstring& _wstrJsonFileName)
{
	m_vMinUV = { 0.f, 0.f };
	m_vMaxUV = { 1.f / (_float)m_iRow, 1.f / (_float)m_iCol };

	m_vUVAddValue.x = 1.f / (_float)m_iRow;
	m_vUVAddValue.y = 1.f / (_float)m_iCol;

	return CParticleActor::Initialize(_wstrJsonFileName);
}

void CSpriteActor::RecycleBegin()
{
	m_vMinUV = { 0.f, 0.f };
	m_vMaxUV = { 1.f / (_float)m_iRow, 1.f / (_float)m_iCol };

	CParticleActor::RecycleBegin();
}

_int CSpriteActor::PreUpdate(const _float& _fTimeDelta)
{
	return CParticleActor::PreUpdate(_fTimeDelta);
}

_int CSpriteActor::Update(const _float& _fTimeDelta)
{
	if(!GetPause())
		ProgressSprite(_fTimeDelta);

	return CParticleActor::Update(_fTimeDelta);
}

_int CSpriteActor::LateUpdate(const _float& _fTimeDelta)
{
	return CParticleActor::LateUpdate(_fTimeDelta);
}

void CSpriteActor::Render()
{
	if (FAILED(BindSpriteShaderResources()))
	{
		MSG_BOX("Failed To BindSpriteShaderResources : CSpriteActor");
		return;
	}

	CParticleActor::Render();
}

void CSpriteActor::EndPlay()
{
	CParticleActor::EndPlay();
}

void CSpriteActor::Release()
{
	CParticleActor::Release();
}

HRESULT CSpriteActor::BindSpriteShaderResources()
{
	if (FAILED(m_wpParticle_Point.lock()->BindRawValue("g_vMinUV", &m_vMinUV, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_wpParticle_Point.lock()->BindRawValue("g_vMaxUV", &m_vMaxUV, sizeof(_float2))))
		return E_FAIL;

	return S_OK;
}

void CSpriteActor::ProgressSprite(const _float& _fTimeDelta)
{
	_float fParticleTimeAcc = m_wpParticle_Point.lock()->GetTimeAcc();

	if (fParticleTimeAcc < m_InstanceDesc.vLifeTime.x)
		return;

	m_fSpriteTimeAcc += _fTimeDelta;

	if (m_fSpriteTimeAcc > m_fSpeedRatio)
	{
		m_fSpriteTimeAcc = 0.f;

		m_vMinUV.x += m_vUVAddValue.x;
		m_vMaxUV.x += m_vUVAddValue.x;

		// 1. 한 행을 모두 진행했다면
		if (1.f < m_vMaxUV.x)
		{
			// 아래로 한 칸 이동
			m_vMinUV.x = 0.f;
			m_vMinUV.y += m_vUVAddValue.y;

			m_vMaxUV.x = m_vUVAddValue.x;
			m_vMaxUV.y += m_vUVAddValue.y;

			if (1.f < m_vMaxUV.y)
				int a = 0;
		}

		// 2. 근데 마지막 행 이였다면
		if (1.f < m_vMaxUV.y)
			ResetUV();
	}
}

void CSpriteActor::ResetUV()
{
	m_vMinUV = { 0.f, 0.f };
	m_vMaxUV = { 1.f / (_float)m_iRow, 1.f / (_float)m_iCol };
	m_bUse = false;
}
