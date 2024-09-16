#include "pch.h"
#include "Seal.h"

#include "ObjectManager.h"
#include "CameraManager.h"

#include "Player.h"
#include "Monster.h"

#include "CameraComponent.h"

void CSeal::Initialize()
{
	CEffectMeshActor::Initialize();
}

HRESULT CSeal::Initialize(const wstring& _wstrJsonFileName)
{
	return CEffectMeshActor::Initialize(_wstrJsonFileName);
}

void CSeal::RecycleBegin()
{
	CEffectMeshActor::RecycleBegin();
}

_int CSeal::PreUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::PreUpdate(_fTimeDelta);
}

_int CSeal::Update(const _float& _fTimeDelta)
{
	if (!m_wpOwnerActor.expired() && m_bUse)
	{
		shared_ptr<CPlayer> spPlayer = dynamic_pointer_cast<CPlayer>(CObjectManager::GetInstance()->GetCurPlayer());

		if (m_fLifeTimeAcc + _fTimeDelta >= m_EffectMeshDesc.vLifeTime.y)
		{
			_float fAtk = spPlayer->GetAtk();
			ATTACK_DESC AttackDesc = { L"Player_Nahida", L"Player_Nahida Skill2", NONE_ELEMENTAL, STRONG,	false,	fAtk * 5.0f };

			dynamic_pointer_cast<CMonster>(m_wpOwnerActor.lock())->GetStateMachine()->RequestChangeState(MONSTER_STATE::HIT2, REQUEST_DIR::DIR_NONE, AttackDesc);
		}

		XMStoreFloat4(&m_vTranslate, m_wpOwnerActor.lock()->GetActorTranslation());

		// ī�޶� ���⺤�Ϳ��� y���� �����ϰ� ����ȭ
		_vector vCamLook = CCameraManager::GetInstance()->GetCurCamera()->GetLookVector();
		vCamLook = XMVector3Normalize(XMVectorSetY(vCamLook, 0.0f));

		// z�� ���⺤�Ϳ��� y���� �����ϰ� ����ȭ
		_vector vZ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		// �����Ͽ� ���� ���� ����
		_float fAngle = acos(XMVector3Dot(vZ, vCamLook).m128_f32[0]) * (180.0f / DirectX::XM_PI);

		// Up ���Ϳ� vCamLook, vActorLook�� ������ ���͸� ��, ������ ���ٸ� ������ 180�� �����̴�.
		if (XMVectorGetY(XMVector3Cross(vCamLook, vZ)) > 0)
			fAngle = 360.0f - fAngle;

		XMStoreFloat4(&m_vRotation, XMVectorSet(0.f, fAngle, 0.f, 0.f));
	}

	return CEffectMeshActor::Update(_fTimeDelta);
}

_int CSeal::LateUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::LateUpdate(_fTimeDelta);
}

void CSeal::Render()
{
	CEffectMeshActor::Render();
}

void CSeal::EndPlay()
{
	CEffectMeshActor::EndPlay();
}

void CSeal::Release()
{
	CEffectMeshActor::Release();
}
