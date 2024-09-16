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
	// �ʱ� ��ȯ������ �����ϰ�, �� ������ �ݰ��� �̸� ���س��´�.
	// ���Ͱ� ��ȯ�� ���� ��ó ������ ����� ���ϰ� �ϱ� ����
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
	// ���Ͱ� Look ���⿡�� y���� �����ϰ� ����ȭ
	_vector vMonsterDir = XMVector3Normalize(XMVectorSetY(_vMonsterLook, 0.0f));

	// ���� -> �÷��̾� ���⺤�Ϳ��� y���� �����ϰ� ����ȭ
	_vector vMonsterToPlayer = XMVector3Normalize(XMVectorSetY(_vMonsterToPlayer, 0.0f));

	// �����Ͽ� ���� ���� ����
	_float fAngle = acos(XMVector3Dot(vMonsterToPlayer, vMonsterDir).m128_f32[0]) * (180.0f / DirectX::XM_PI);

	// Up ���Ϳ� vCamLook, vActorLook�� ������ ���͸� ��, ������ ���ٸ� ������ 180�� �����̴�.
	if (XMVectorGetY(XMVector3Cross(vMonsterDir, vMonsterToPlayer)) > 0)
	{
		fAngle = 360.0f - fAngle;
	}

	return fAngle;
}

void CMonsterStateMachine::MakeHitEffect()
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
	
	// ���� �÷��̾��� ��ġ�� �޾ƿ´�.(���Ϳ� �÷��̾� ���̿� ����Ʈ�� �����ϱ� ����)
	shared_ptr<CActor> spPlayer = CObjectManager::GetInstance()->GetCurPlayer();
	_vector vPlayerPos = spPlayer->GetActorTranslation();

	// ������ ��ġ�� �޾ƿ´�.
	_vector vMonsterPos = m_wpOwnerActor.lock()->GetActorTranslation();

	// ����->�÷��̾� ���⺤�� ����
	_vector vMonsterToPlayer = XMVector3Normalize(vPlayerPos - vMonsterPos);

	// ���� ��ġ�� ���Ѵ�.
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

	// 1. ���Ӽ� ������ ���
	if (NONE_ELEMENTAL == _AttackDesc.eElementalType)
	{
		// ���� �������� �״�� ������.
		spChractor->SetCurHp(fCurHp - (_fDamage - fDef));
		MakeDamageFont(_AttackDesc, vRandomSpawnPos);

		return _uint(_fDamage - fDef);
	}
	// 2. ���� ť�� ����ִ� ���
	else if (m_queElemental.empty())
	{
		// ���� �������� �״�� ������, Que�� ���Ҹ� �ִ´�.
		spChractor->SetCurHp(fCurHp - (_fDamage - fDef));
		MakeDamageFont(_AttackDesc, vRandomSpawnPos);

		m_queElemental.push(_AttackDesc.eElementalType);

		return _uint(_fDamage - fDef);
	}
	// 3. ���Ӽ� ���ݵ� �ƴϰ�, ���� ť�� ���Ұ� ����ִ� ���
	else
	{
		wstring wstrReaction = CalculateElementalReaction(m_queElemental.back(), _AttackDesc);

		if (L"������" == wstrReaction) // ���� ���� ���ҿ� �ǰݴ��� ���Ұ� ���� ������ ���
		{
			fFinalDamage = _fDamage;
		}
		else if (L"����" == wstrReaction)
		{
			// �ڡڡڡ� ���⼭ CElementalReaction�� RecycleFromPool ���ָ� �� �� �մϴ�. (�Լ� �ϳ� �ļ�) �ڡڡڡ�
			fFinalDamage = _fDamage * 2.f;
			MakeElementalReactionFont(L"����", _AttackDesc, vRandomSpawnPos);
		}
		else if (L"����" == wstrReaction)
		{
			// �ڡڡڡ� ���⼭ CElementalReaction�� RecycleFromPool ���ָ� �� �� �մϴ�. �ڡڡڡ�
			fFinalDamage = _fDamage * 3.f;
			MakeElementalReactionFont(L"����", _AttackDesc, vRandomSpawnPos);
		}
		else if (L"����" == wstrReaction)
		{
			// �ڡڡڡ� ���⼭ CElementalReaction�� RecycleFromPool ���ָ� �� �� �մϴ�. �ڡڡڡ�
			fFinalDamage = _fDamage * 1.5f;
			MakeElementalReactionFont(L"����", _AttackDesc, vRandomSpawnPos);
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
	// ��
	case PYRO:
	{
		// �ǰ� ���� ����
		switch (_Dst.eElementalType)
		{
			// ��
		case PYRO:		return L"������";
			// ��
		case HYDRO:		return L"����";
			// �ٶ�
		case ANEMO:		return L"(��)Ȯ��";
			// ����
		case ELECTRO:	return L"������";
			// Ǯ
		case DENDRO:	return L"����";
			// ����
		case CRYO:		return L"����";
			// ����
		case GEO:		return L"����";
		default:
			return L"";
		}
		break;
	}
	// ��
	case HYDRO:
	{
		// �ǰ� ���� ����
		switch (_Dst.eElementalType)
		{
			// ��
		case PYRO:		return L"����";
			// ��
		case HYDRO:		return L"������";
			// �ٶ�
		case ANEMO:		return L"(��)Ȯ��";
			// ����
		case ELECTRO:	return L"����";
			// Ǯ
		case DENDRO:	return L"��ȭ";
			// ����
		case CRYO:		return L"����";
			// ����
		case GEO:		return L"����";
		default:
			return L"";
		}
		break;
	}
	// �ٶ�
	case ANEMO:
	{
		// �ǰ� ���� ����
		switch (_Dst.eElementalType)
		{
			// ��
		case PYRO:		return L"(��)Ȯ��";
			// ��
		case HYDRO:		return L"(��)Ȯ��";
			// �ٶ�
		case ANEMO:		return L"������";
			// ����
		case ELECTRO:	return L"(����)Ȯ��";
			// Ǯ
		case DENDRO:	return L"������";
			// ����
		case CRYO:		return L"(����)Ȯ��";
			// ����
		case GEO:		return L"������";
		default:
			return L"";
		}
		break;
	}
	// ����
	case ELECTRO:
	{
		// �ǰ� ���� ����
		switch (_Dst.eElementalType)
		{
			// ��
		case PYRO:		return L"������";
			// ��
		case HYDRO:		return L"����";
			// �ٶ�
		case ANEMO:		return L"(����)Ȯ��";
			// ����
		case ELECTRO:	return L"������";
			// Ǯ
		case DENDRO:	return L"Ȱ��";
			// ����
		case CRYO:		return L"������";
			// ����
		case GEO:		return L"����";
		default:
			return L"";
		}
		break;
	}
	// Ǯ
	case DENDRO:
	{
		// �ǰ� ���� ����
		switch (_Dst.eElementalType)
		{
			// ��
		case PYRO:		return L"����";
			// ��
		case HYDRO:		return L"��ȭ";
			// �ٶ�
		case ANEMO:		return L"������";
			// ����
		case ELECTRO:	return L"Ȱ��";
			// Ǯ
		case DENDRO:	return L"������";
			// ����
		case CRYO:		return L"������";
			// ����
		case GEO:		return L"������";
		default:
			return L"";
		}
		break;
	}
	// ����
	case CRYO:
	{
		// �ǰ� ���� ����
		switch (_Dst.eElementalType)
		{
			// ��
		case PYRO:		return L"����";
			// ��
		case HYDRO:		return L"����";
			// �ٶ�
		case ANEMO:		return L"(����)Ȯ��";
			// ����
		case ELECTRO:	return L"������";
			// Ǯ
		case DENDRO:	return L"������";
			// ����
		case CRYO:		return L"������";
			// ����
		case GEO:		return L"����";
		default:
			return L"";
		}
		break;
	}
	// ����
	case GEO:
	{
		// �ǰ� ���� ����
		switch (_Dst.eElementalType)
		{
			// ��
		case PYRO:		return L"����";
			// ��
		case HYDRO:		return L"����";
			// �ٶ�
		case ANEMO:		return L"������";
			// ����
		case ELECTRO:	return L"����";
			// Ǯ
		case DENDRO:	return L"������";
			// ����
		case CRYO:		return L"����";
			// ����
		case GEO:		return L"������";
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
