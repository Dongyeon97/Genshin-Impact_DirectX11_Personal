#include "pch.h"
#include "PlayerStateMachine.h"

#include "ObjectManager.h"
#include "CameraManager.h"
#include "NavigationManager.h"
#include "SoundManager.h"

#include "Level.h"
#include "Layer.h"
#include "Actor.h"
#include "Player.h"
#include "EffectMeshActor.h"
#include "CaptureEffect.h"

#include "Navigation.h"
#include "Cell.h"
#include "AttackCollider.h"
#include "CameraComponent.h"

void CPlayerStateMachine::Initialize()
{
	CStateMachine::Initialize();
}

_int CPlayerStateMachine::PreUpdate(const _float& _fTimeDelta)
{
	return CStateMachine::PreUpdate(_fTimeDelta);
}

_int CPlayerStateMachine::Update(const _float& _fTimeDelta)
{
	return CStateMachine::Update(_fTimeDelta);
}

_int CPlayerStateMachine::LateUpdate(const _float& _fTimeDelta)
{
	return CStateMachine::LateUpdate(_fTimeDelta);
}

void CPlayerStateMachine::Render()
{
	CStateMachine::Render();
}

void CPlayerStateMachine::EndPlay()
{
	CStateMachine::EndPlay();
}

void CPlayerStateMachine::Release()
{
	CStateMachine::Release();
}

void CPlayerStateMachine::MakeHitEffect(_vector _vOffset)
{
	wstring wstrHitEffectName;

	// ���� ����Ʈ ����
	switch (rand() % 6)
	{
	case 0:
	{
		wstrHitEffectName = L"Sprite_Hit_One";
		break;
	}
	case 1:
	{
		wstrHitEffectName = L"Sprite_Hit_Two";
		break;
	}
	case 2:
	{
		wstrHitEffectName = L"Sprite_Hit_Three";
		break;
	}
	case 3:
	{
		wstrHitEffectName = L"Sprite_Hit_Five";
		break;
	}
	case 4:
	{
		wstrHitEffectName = L"Sprite_Hit_Seven";
		break;
	}
	case 5:
	{
		wstrHitEffectName = L"Sprite_Hit_Eight";
		break;
	}
	}

	shared_ptr<CActor> spActor = CObjectManager::GetInstance()->GetStaticLevel()->GetLayer(LAYER::LAYER_PARTICLE)->RecycleFromPool(wstrHitEffectName);

	if (nullptr != spActor && !m_wpOwnerActor.expired())
		spActor->SetActorTranslation_NoNavigation(m_wpOwnerActor.lock()->GetActorTranslation() + _vOffset);
}

void CPlayerStateMachine::MakeParticle(const wstring& _wstrParticleName, _vector _vOffset)
{
	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RecycleFromPool(_wstrParticleName);
	if (nullptr != spActor)
	{
		spActor->SetActorTranslation_NoNavigation(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
	}
}

void CPlayerStateMachine::MakeEffect(const wstring& _wstrEffectMeshName, _vector _vOffset)
{
	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->RecycleFromPool(_wstrEffectMeshName);

	if (nullptr == spActor)
		return;

	_float3 vOwnerRotation;
	XMStoreFloat3(&vOwnerRotation, XMVectorSetZ(XMVectorSetX(m_wpOwnerActor.lock()->GetRotation(), 0.f), 0.f));

	_float3 vRotation = dynamic_pointer_cast<CEffectMeshActor>(spActor)->GetEffectMeshDesc().vRotation;

	spActor->SetRotation(XMLoadFloat3(&vOwnerRotation) + XMLoadFloat3(&vRotation));
	spActor->SetActorTranslation_NoNavigation(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
}

void CPlayerStateMachine::PlayFlyingStartSound()
{
	switch (rand() % 5)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks11_1235.ogg", SOUND_TYPE::EFFECT_3, 0.1f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks11_1236.ogg", SOUND_TYPE::EFFECT_3, 0.1f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks11_1237.ogg", SOUND_TYPE::EFFECT_3, 0.1f);
		break;
	}
	case 3:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks11_1238.ogg", SOUND_TYPE::EFFECT_3, 0.1f);
		break;
	}
	case 4:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks11_1239.ogg", SOUND_TYPE::EFFECT_3, 0.1f);
		break;
	}
	}
}

void CPlayerStateMachine::NormalAttackColliderOnOff(_float _fOnTime, _float _fOffTime)
{
	if (_fOnTime <= m_fAttack_RunTime && m_fAttack_RunTime < _fOffTime)
		m_wpWeaponCollider.lock()->SetIsCollideOn(true);
	else
		m_wpWeaponCollider.lock()->SetIsCollideOn(false);
}

void CPlayerStateMachine::SkillColliderOnOff(_float _fOnTime, _float _fOffTime)
{
	if (m_wpSkillCollider.expired())
		return;

	if (_fOnTime <= m_fElemental_Skill_RunTime && m_fElemental_Skill_RunTime < _fOffTime)
		m_wpSkillCollider.lock()->SetIsCollideOn(true);
	else
		m_wpSkillCollider.lock()->SetIsCollideOn(false);
}

_float CPlayerStateMachine::CalculateActorRotation()
{
	// ī�޶� ���⺤�Ϳ��� y���� �����ϰ� ����ȭ
	_vector vCamLook = CCameraManager::GetInstance()->GetCurCamera()->GetLookVector();
	vCamLook = XMVector3Normalize(XMVectorSetY(vCamLook, 0.0f));

	// z�� ���⺤�Ϳ��� y���� �����ϰ� ����ȭ
	_vector vZ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// �����Ͽ� ���� ���� ����
	_float fAngle = acos(XMVector3Dot(vZ, vCamLook).m128_f32[0]) * (180.0f / DirectX::XM_PI);

	// Up ���Ϳ� vCamLook, vActorLook�� ������ ���͸� ��, ������ ���ٸ� ������ 180�� �����̴�.
	if (XMVectorGetY(XMVector3Cross(vCamLook, vZ)) > 0)
	{
		fAngle = 360.0f - fAngle;
	}

	return fAngle;
}

_vector CPlayerStateMachine::AutoTargetting()
{
	_float fDistance = 100000000.f;

	/* ���̵��� ���� ���̾ ��ȸ�ϸ鼭, ���� ����� �ָ� ã�´�. */
	const unordered_map<wstring, shared_ptr<CActor>>& umapActors = CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER::LAYER_MONSTER)->GetActors();
	const unordered_map<wstring, vector<shared_ptr<CActor>>>& umapActors_Pool = CObjectManager::GetInstance()->GetCurLevel()->GetLayer(LAYER::LAYER_MONSTER)->GetActors_Pool();

	shared_ptr<CActor> spPlayer = m_wpOwnerActor.lock();
	_vector vPlayerPos = spPlayer->GetActorTranslation();
	_vector vPlayerLook = XMVector3Normalize(spPlayer->GetLookVector());

	shared_ptr<CActor> spMonster = nullptr;
	_vector vMonsterToPlayer = {};

	// ��ȸ�ϸ鼭 �÷��̾�� ���� ������ �Ÿ��� ���Ѵ�.

	// �Ϲ�
	auto iter = find_if(umapActors.begin(), umapActors.end(), [&](pair<wstring, shared_ptr<CActor>> _spMonster)
		{
			if (!_spMonster.second->GetActorUse())
				return false;

			vMonsterToPlayer = vPlayerPos - _spMonster.second->GetActorTranslation();
			_float fMonsterToPlayerDistance = XMVector3Length(vMonsterToPlayer).m128_f32[0];

			// 1200�̻��� �Ÿ���� Ÿ������ ���� ����
			if (fMonsterToPlayerDistance > 1200.f)
				return false;

			if (fMonsterToPlayerDistance < fDistance)
			{
				fDistance = fMonsterToPlayerDistance;
				spMonster = _spMonster.second;
			}

			return false;
		});
	if (nullptr != spMonster)
	{
		// vPlayerToMonster���� y���� �����ϰ� ����ȭ
		_vector vPlayerToMonster = spMonster->GetActorTranslation() - vPlayerPos;
		vPlayerToMonster = XMVector3Normalize(vPlayerToMonster);

		// z�� ���⺤�Ϳ��� y���� �����ϰ� ����ȭ
		_vector vZ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		// �����Ͽ� ���� ���� ����
		_float fAngle = acos(XMVector3Dot(vZ, vPlayerToMonster).m128_f32[0]) * (180.0f / DirectX::XM_PI);

		// Up ���Ϳ� vCamLook, vActorLook�� ������ ���͸� ��, ������ ���ٸ� ������ 180�� �����̴�.
		if (XMVectorGetY(XMVector3Cross(vPlayerToMonster, vZ)) > 0)
			fAngle = 360.0f - fAngle;

		spPlayer->SetRotation(XMVectorSet(0.f, fAngle, 0.f, 0.f));
		spPlayer->CalculateWorldMatrix_Immediately();
	}

	// Ǯ
	for (auto& ActorPool : umapActors_Pool)
	{
		auto iter_Pool = find_if(ActorPool.second.begin(), ActorPool.second.end(), [&](shared_ptr<CActor> _spMonster)
			{
				if (!_spMonster->GetActorUse())
					return false;

				vMonsterToPlayer = vPlayerPos - _spMonster->GetActorTranslation();
				_float fMonsterToPlayerDistance = XMVector3Length(vMonsterToPlayer).m128_f32[0];

				// 1200�̻��� �Ÿ���� Ÿ������ ���� ����
				if (fMonsterToPlayerDistance > 1200.f)
					return false;

				if (fMonsterToPlayerDistance < fDistance)
				{
					fDistance = fMonsterToPlayerDistance;
					spMonster = _spMonster;
				}

				return false;
			});
	}
	if (nullptr != spMonster)
	{
		// vPlayerToMonster���� y���� �����ϰ� ����ȭ
		_vector vPlayerToMonster = spMonster->GetActorTranslation() - vPlayerPos;
		vPlayerToMonster = XMVector3Normalize(vPlayerToMonster);

		// z�� ���⺤�Ϳ��� y���� �����ϰ� ����ȭ
		_vector vZ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		// �����Ͽ� ���� ���� ����
		_float fAngle = acos(XMVector3Dot(vZ, vPlayerToMonster).m128_f32[0]) * (180.0f / DirectX::XM_PI);

		// Up ���Ϳ� vCamLook, vActorLook�� ������ ���͸� ��, ������ ���ٸ� ������ 180�� �����̴�.
		if (XMVectorGetY(XMVector3Cross(vPlayerToMonster, vZ)) > 0)
			fAngle = 360.0f - fAngle;

		spPlayer->SetRotation(XMVectorSet(0.f, fAngle, 0.f, 0.f));
		spPlayer->CalculateWorldMatrix_Immediately();
	}

	if (nullptr != spMonster)
		return spMonster->GetActorTranslation();

	return (vPlayerPos + vPlayerLook * 300.f);
}

_float CPlayerStateMachine::IsLand()
{
	// �÷��̾� �׺� �� �ε���
	shared_ptr<CPlayer> spPlayer = dynamic_pointer_cast<CPlayer>(m_wpOwnerActor.lock());

	_int& iCurCellIndex = spPlayer->GetCurNaviCellIndex();

	// �ٴڿ� ������� ������ �����Ѵ�.
	shared_ptr<CNavigation> spNavi = CNavigationManager::GetInstance()->GetCurNavigation();
	shared_ptr<CCell> spCurCell = spNavi->GetVecCells()[iCurCellIndex];

	spPlayer = dynamic_pointer_cast<CPlayer>(m_wpOwnerActor.lock());
	_vector vActorPos = spPlayer->GetActorTranslation();

	// ���� ������ ������ x,z�� �����Ͽ� y��ǥ�� ���, �װͺ��� ���Ͱ� �Ʒ��� ������ ������ �����Ѵ�.
	_float fPlaneY = spCurCell->GetPlaneY(XMVectorGetX(vActorPos), XMVectorGetZ(vActorPos));

	if (XMVectorGetY(vActorPos) <= fPlaneY)
		return fPlaneY;

	return 0.0f;
}
