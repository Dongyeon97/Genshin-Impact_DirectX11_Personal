#include "pch.h"
#include "MonsterStateMachine.h"

#include "ObjectManager.h"

#include "Level.h"
#include "Layer.h"

#include "DamageFont.h"
#include "ElementalReaction.h"

#include "AttackCollider.h"

void CMonsterStateMachine::Initialize()
{
	// 초기 소환지점을 지정하고, 그 곳에서 반경을 미리 구해놓는다.
	// 몬스터가 소환된 지점 근처 밖으로 벗어나지 못하게 하기 위해
	XMStoreFloat3(&m_vOriginSpawnPoint, GetOwnerActor().lock()->GetActorTranslation());

	m_RandomNumber = mt19937_64(m_RandomDevice());

	CStateMachine::Initialize();
}

_int CMonsterStateMachine::PreUpdate(const _float& _fTimeDelta)
{
	return CStateMachine::PreUpdate(_fTimeDelta);
}

_int CMonsterStateMachine::Update(const _float& _fTimeDelta)
{
	return CStateMachine::Update(_fTimeDelta);
}

_int CMonsterStateMachine::LateUpdate(const _float& _fTimeDelta)
{
	return CStateMachine::LateUpdate(_fTimeDelta);
}

void CMonsterStateMachine::Render()
{
	CStateMachine::Render();
}

void CMonsterStateMachine::EndPlay()
{
	CStateMachine::EndPlay();
}

void CMonsterStateMachine::Release()
{
	for (auto& AttackCollider : m_mapAttackColliders)
	{
		if (nullptr != AttackCollider.second)
		{
			AttackCollider.second->Release();
			AttackCollider.second.reset();
		}
	}

	CStateMachine::Release();
}

_float CMonsterStateMachine::CaculateMonsterRotation(_vector _vMonsterLook, _vector _vMonsterToPlayer)
{
	// 몬스터가 Look 방향에서 y성분 제거하고 정규화
	_vector vMonsterDir = XMVector3Normalize(XMVectorSetY(_vMonsterLook, 0.0f));

	// 몬스터 -> 플레이어 방향벡터에서 y성분 제거하고 정규화
	_vector vMonsterToPlayer = XMVector3Normalize(XMVectorSetY(_vMonsterToPlayer, 0.0f));

	// 내적하여 라디안 각도 도출
	_float fAngle = acos(XMVector3Dot(vMonsterToPlayer, vMonsterDir).m128_f32[0]) * (180.0f / DirectX::XM_PI);

	// Up 벡터와 vCamLook, vActorLook을 외적한 벡터를 비교, 방향이 같다면 각도는 180도 이하이다.
	if (XMVectorGetY(XMVector3Cross(vMonsterDir, vMonsterToPlayer)) > 0)
	{
		fAngle = 360.0f - fAngle;
	}

	return fAngle;
}

void CMonsterStateMachine::MakeHitEffect()
{
	wstring wstrHitEffectName;

	// 랜덤 이펙트 생성
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
	
	// 현재 플레이어의 위치를 받아온다.(몬스터와 플레이어 사이에 이펙트를 생성하기 위해)
	shared_ptr<CActor> spPlayer = CObjectManager::GetInstance()->GetCurPlayer();
	_vector vPlayerPos = spPlayer->GetActorTranslation();

	// 몬스터의 위치를 받아온다.
	_vector vMonsterPos = m_wpOwnerActor.lock()->GetActorTranslation();

	// 몬스터->플레이어 방향벡터 도출
	_vector vMonsterToPlayer = XMVector3Normalize(vPlayerPos - vMonsterPos);

	// 최종 위치를 구한다.
	_vector vHittedPos = vMonsterPos + vMonsterToPlayer * 30.f;

	shared_ptr<CActor> spHitEffect = CObjectManager::GetInstance()->GetStaticLevel()->GetLayer(LAYER::LAYER_PARTICLE)->RecycleFromPool(wstrHitEffectName);

	if (nullptr != spHitEffect && !m_wpOwnerActor.expired())
		spHitEffect->SetActorTranslation_NoNavigation(vHittedPos);
}

void CMonsterStateMachine::MakeDamageFont(ATTACK_DESC _AttackDesc, _vector _vOffset)
{
	shared_ptr<CActor> spActor = CObjectManager::GetInstance()->GetStaticLevel()->GetLayer(LAYER::LAYER_UI)->RecycleFromPool(L"DamageFont");
	if (nullptr == spActor)
		return;
	
	shared_ptr<CDamageFont> spDamageFont = dynamic_pointer_cast<CDamageFont>(spActor);

	uniform_real_distribution<float>	RandomDamageRatio(100.f, 250.f);
	ATTACK_DESC AttackDesc = _AttackDesc;

	if (_float(rand() % 100) < _AttackDesc.fCriticalRatio)
	{
		AttackDesc.eDamage = AttackDesc.eDamage * 1.8f + RandomDamageRatio(m_RandomNumber);
		spDamageFont->SetMinMaxScale({ 1.1f, 2.4f });
	}
	else
	{
		AttackDesc.eDamage = AttackDesc.eDamage + RandomDamageRatio(m_RandomNumber);
		spDamageFont->SetMinMaxScale({ 0.8f, 1.7f });
	}

	spDamageFont->SetAttackDesc(AttackDesc);
	spDamageFont->SetSpawnPos(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
	spDamageFont->SetActorTranslation_NoNavigation(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
}

void CMonsterStateMachine::MakeElementalReactionFont(const wstring& _wstrReactionName, ATTACK_DESC _AttackDesc, _vector _vOffset)
{
	shared_ptr<CActor> spActor = CObjectManager::GetInstance()->GetStaticLevel()->GetLayer(LAYER::LAYER_UI)->RecycleFromPool(L"ElementalReactionFont");
	if (nullptr == spActor)
		return;

	shared_ptr<CElementalReaction> spElementalReactionFont = dynamic_pointer_cast<CElementalReaction>(spActor);

	spElementalReactionFont->SetReactionName(_wstrReactionName);
	spElementalReactionFont->SetAttackDesc(_AttackDesc);
	spElementalReactionFont->SetSpawnPos(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
	spElementalReactionFont->SetActorTranslation_NoNavigation(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
}

_uint CMonsterStateMachine::Hitted(ATTACK_DESC _AttackDesc, _float _fDamage, _vector _vOffset)
{
	shared_ptr<CCharactor> spChractor = dynamic_pointer_cast<CCharactor>(m_wpOwnerActor.lock());

	_float fCurHp = spChractor->GetCurHp();
	_float fDef = spChractor->GetDef();

	_float fFinalDamage = 0.f;

	uniform_real_distribution<float>	PositionRangeX(50.f, 100.f);
	uniform_real_distribution<float>	PositionRangeY(50.f, 100.f);
	uniform_real_distribution<float>	PositionRangeZ(50.f, 100.f);

	_vector vRandomSpawnPos = _vOffset +
		XMVectorSet(PositionRangeX(m_RandomNumber), PositionRangeY(m_RandomNumber), PositionRangeZ(m_RandomNumber), 0.f);

	// 1. 무속성 공격인 경우
	if (NONE_ELEMENTAL == _AttackDesc.eElementalType)
	{
		// 들어온 데미지를 그대로 입힌다.
		spChractor->SetCurHp(fCurHp - (_fDamage - fDef));
		MakeDamageFont(_AttackDesc, vRandomSpawnPos);

		return _uint(_fDamage - fDef);
	}
	// 2. 원소 큐가 비어있는 경우
	else if (m_queElemental.empty())
	{
		// 들어온 데미지를 그대로 입히고, Que에 원소를 넣는다.
		spChractor->SetCurHp(fCurHp - (_fDamage - fDef));
		MakeDamageFont(_AttackDesc, vRandomSpawnPos);

		m_queElemental.push(_AttackDesc.eElementalType);

		return _uint(_fDamage - fDef);
	}
	// 3. 무속성 공격도 아니고, 원소 큐에 원소가 들어있는 경우
	else
	{
		wstring wstrReaction = CalculateElementalReaction(m_queElemental.back(), _AttackDesc);

		if (L"무반응" == wstrReaction) // 몸에 뭍은 원소와 피격당한 원소가 같은 종류인 경우
		{
			fFinalDamage = _fDamage;
		}
		else if (L"융해" == wstrReaction)
		{
			// ★★★★ 여기서 CElementalReaction을 RecycleFromPool 해주면 될 듯 합니다. (함수 하나 파서) ★★★★
			fFinalDamage = _fDamage * 2.f;
			MakeElementalReactionFont(L"융해", _AttackDesc, vRandomSpawnPos);
		}
		else if (L"연소" == wstrReaction)
		{
			// ★★★★ 여기서 CElementalReaction을 RecycleFromPool 해주면 될 듯 합니다. ★★★★
			fFinalDamage = _fDamage * 3.f;
			MakeElementalReactionFont(L"연소", _AttackDesc, vRandomSpawnPos);
		}
		else if (L"결정" == wstrReaction)
		{
			// ★★★★ 여기서 CElementalReaction을 RecycleFromPool 해주면 될 듯 합니다. ★★★★
			fFinalDamage = _fDamage * 1.5f;
			MakeElementalReactionFont(L"결정", _AttackDesc, vRandomSpawnPos);
		}

		vRandomSpawnPos = _vOffset +
			XMVectorSet(PositionRangeX(m_RandomNumber), PositionRangeY(m_RandomNumber), PositionRangeZ(m_RandomNumber), 0.f);

		spChractor->SetCurHp(fCurHp - (fFinalDamage - fDef));
		MakeDamageFont(_AttackDesc, vRandomSpawnPos);

		m_queElemental.pop();

		return _uint(fFinalDamage - fDef);
	}
}

const wstring CMonsterStateMachine::CalculateElementalReaction(ELEMENTAL_TYPE _Src, ATTACK_DESC _Dst)
{
	switch (_Src)
	{
	// 불
	case PYRO:
	{
		// 피격 당한 원소
		switch (_Dst.eElementalType)
		{
			// 불
		case PYRO:		return L"무반응";
			// 물
		case HYDRO:		return L"증발";
			// 바람
		case ANEMO:		return L"(불)확산";
			// 번개
		case ELECTRO:	return L"과부하";
			// 풀
		case DENDRO:	return L"연소";
			// 얼음
		case CRYO:		return L"융해";
			// 바위
		case GEO:		return L"결정";
		default:
			return L"";
		}
		break;
	}
	// 물
	case HYDRO:
	{
		// 피격 당한 원소
		switch (_Dst.eElementalType)
		{
			// 불
		case PYRO:		return L"증발";
			// 물
		case HYDRO:		return L"무반응";
			// 바람
		case ANEMO:		return L"(물)확산";
			// 번개
		case ELECTRO:	return L"감전";
			// 풀
		case DENDRO:	return L"개화";
			// 얼음
		case CRYO:		return L"빙결";
			// 바위
		case GEO:		return L"결정";
		default:
			return L"";
		}
		break;
	}
	// 바람
	case ANEMO:
	{
		// 피격 당한 원소
		switch (_Dst.eElementalType)
		{
			// 불
		case PYRO:		return L"(불)확산";
			// 물
		case HYDRO:		return L"(물)확산";
			// 바람
		case ANEMO:		return L"무반응";
			// 번개
		case ELECTRO:	return L"(번개)확산";
			// 풀
		case DENDRO:	return L"무반응";
			// 얼음
		case CRYO:		return L"(얼음)확산";
			// 바위
		case GEO:		return L"무반응";
		default:
			return L"";
		}
		break;
	}
	// 번개
	case ELECTRO:
	{
		// 피격 당한 원소
		switch (_Dst.eElementalType)
		{
			// 불
		case PYRO:		return L"과부하";
			// 물
		case HYDRO:		return L"감전";
			// 바람
		case ANEMO:		return L"(번개)확산";
			// 번개
		case ELECTRO:	return L"무반응";
			// 풀
		case DENDRO:	return L"활성";
			// 얼음
		case CRYO:		return L"초전도";
			// 바위
		case GEO:		return L"결정";
		default:
			return L"";
		}
		break;
	}
	// 풀
	case DENDRO:
	{
		// 피격 당한 원소
		switch (_Dst.eElementalType)
		{
			// 불
		case PYRO:		return L"연소";
			// 물
		case HYDRO:		return L"개화";
			// 바람
		case ANEMO:		return L"무반응";
			// 번개
		case ELECTRO:	return L"활성";
			// 풀
		case DENDRO:	return L"무반응";
			// 얼음
		case CRYO:		return L"무반응";
			// 바위
		case GEO:		return L"무반응";
		default:
			return L"";
		}
		break;
	}
	// 얼음
	case CRYO:
	{
		// 피격 당한 원소
		switch (_Dst.eElementalType)
		{
			// 불
		case PYRO:		return L"융해";
			// 물
		case HYDRO:		return L"빙결";
			// 바람
		case ANEMO:		return L"(얼음)확산";
			// 번개
		case ELECTRO:	return L"초전도";
			// 풀
		case DENDRO:	return L"무반응";
			// 얼음
		case CRYO:		return L"무반응";
			// 바위
		case GEO:		return L"결정";
		default:
			return L"";
		}
		break;
	}
	// 바위
	case GEO:
	{
		// 피격 당한 원소
		switch (_Dst.eElementalType)
		{
			// 불
		case PYRO:		return L"결정";
			// 물
		case HYDRO:		return L"결정";
			// 바람
		case ANEMO:		return L"무반응";
			// 번개
		case ELECTRO:	return L"결정";
			// 풀
		case DENDRO:	return L"무반응";
			// 얼음
		case CRYO:		return L"결정";
			// 바위
		case GEO:		return L"무반응";
		default:
			return L"";
		}
		break;
	}
	}
}

void CMonsterStateMachine::SetAllAttackColliderOn()
{
	for (auto& pair : m_mapAttackColliders)
		pair.second->SetIsCollideOn(true);
}

void CMonsterStateMachine::SetAllAttackColliderOff()
{
	for (auto& pair : m_mapAttackColliders)
		pair.second->SetIsCollideOn(false);
}

void CMonsterStateMachine::SetAttackColliderEnable(const wstring& _wstrColTag, _bool _bEnable)
{
	auto iter = m_mapAttackColliders.find(_wstrColTag);

	if (iter == m_mapAttackColliders.end())
		return;

	iter->second->SetIsCollideOn(_bEnable);
}

void CMonsterStateMachine::SetAttackDescToAttackCollider(const wstring& _wstrColTag, ATTACK_DESC _AttackDesc)
{
	auto iter = m_mapAttackColliders.find(_wstrColTag);

	if (iter == m_mapAttackColliders.end())
		return;

	iter->second->SetAttackDesc(_AttackDesc);
}
