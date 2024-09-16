#include "pch.h"
#include "AndreusStateMachine.h"

#include "CameraManager.h"
#include "ObjectManager.h"
#include "SoundManager.h"

#include "Level.h"
#include "Layer.h"

#include "Boss_Andreus.h"
#include "EffectMeshActor.h"
#include "MovingEffect.h"
#include "MonsterAttackEffect.h"
#include "MonsterProjectileEffect.h"
#include "ParticleActor.h"

#include "AnimModel.h"
#include "Animation.h"
#include "ProductionCamera.h"

using namespace MONSTER_STATE;
using namespace BOSS_PAIZE;

void CAndreusStateMachine::Initialize()
{
	wstring wstrOwnerName = m_wpOwnerActor.lock()->GetActorName();
	_float	fAtk = dynamic_pointer_cast<CCharactor>(m_wpOwnerActor.lock())->GetAtk();

	m_Attack1_Desc = { wstrOwnerName, L"Andreus Attack1", NONE_ELEMENTAL, STRONG, false, fAtk * 1.0f };
	m_Attack2_Desc = { wstrOwnerName, L"Andreus Attack2", NONE_ELEMENTAL, STRONG, false, fAtk * 1.0f };
	m_Attack3_Desc = { wstrOwnerName, L"Andreus Attack3", NONE_ELEMENTAL, POWERFUL, false, fAtk * 2.5f };
	m_Attack4_Desc = { wstrOwnerName, L"Andreus Attack4", NONE_ELEMENTAL, POWERFUL, false, fAtk * 1.5f };
	m_Attack5_Desc = { wstrOwnerName, L"Andreus Attack5", NONE_ELEMENTAL, POWERFUL, false, fAtk * 0.5f };

	m_Pattern1_Desc = { wstrOwnerName, L"Andreus Pattern1", CRYO, STRONG, false, fAtk * 1.0f };
	m_Pattern2_Desc = { wstrOwnerName, L"Andreus Pattern2", ANEMO, POWERFUL, false, fAtk * 2.0f };
	m_Pattern3_Desc = { wstrOwnerName, L"Andreus Pattern3", CRYO, POWERFUL, false, fAtk * 2.0f };
	m_Pattern4_Desc = { wstrOwnerName, L"Andreus Pattern4", ANEMO, POWERFUL, false, fAtk * 0.5f };
	m_Pattern5_Desc = { wstrOwnerName, L"Andreus Pattern5", CRYO, STRONG, false, fAtk * 1.0f };

	// �����ϰ� ������Ʈ�� �����ϱ� ���� ���� ����̽� �ʱ�ȭ
	m_RandomNumber = mt19937_64(m_RandomDevice());

	CMonsterStateMachine::Initialize();
}

_int CAndreusStateMachine::PreUpdate(const _float& _fTimeDelta)
{
	return CMonsterStateMachine::PreUpdate(_fTimeDelta);
}

_int CAndreusStateMachine::Update(const _float& _fTimeDelta)
{
	ManagementActorCoolTime(_fTimeDelta);
	ManagementActorState(_fTimeDelta);

	/*m_fHitTimeAcc += _fTimeDelta;
	if (3.f < m_fHitTimeAcc && m_bHit)
	{
		m_bHit = false;
		m_fHitTimeAcc = 0.f;
		dynamic_pointer_cast<CBossMonster>(m_wpOwnerActor.lock())->SetHPBarEnable(false);
	}*/

	return CMonsterStateMachine::Update(_fTimeDelta);
}

_int CAndreusStateMachine::LateUpdate(const _float& _fTimeDelta)
{
	return CMonsterStateMachine::LateUpdate(_fTimeDelta);
}

void CAndreusStateMachine::Render()
{
	CMonsterStateMachine::Render();
}

void CAndreusStateMachine::EndPlay()
{
	CMonsterStateMachine::EndPlay();
}

void CAndreusStateMachine::Release()
{
	CMonsterStateMachine::Release();
}

void CAndreusStateMachine::ManagementActorState(const _float& _fTimeDelta)
{
#pragma region �ִϸ��̼� ���� ó��
	_uint iCurAnim = m_wpAnimModel.lock()->GetAnimationIndex();
	if (m_wpAnimModel.lock()->GetAnimVector()[iCurAnim]->GetIsFinished())
	{
		// �ִϸ��̼��� ������, ���� �ִϸ��̼��� �ƴ϶��, IDLE�� ���ƿ��� ��û�Ѵ�.
		RequestChangeState(MONSTER_STATE::IDLE);
		m_wpAnimModel.lock()->GetAnimVector()[iCurAnim]->SetIsFinished(false);
	}
#pragma endregion �ִϸ��̼� ���� ó��

#pragma region ������ ����ó���� �ʿ��� ����
	// �ڽ�(����) ������
	shared_ptr<CMonster> spMonster = dynamic_pointer_cast<CMonster>(m_wpOwnerActor.lock());

	// �ڽ�(����) ��ġ
	_vector vMonsterPos = spMonster->GetActorTranslation();

	// �ڽ�(����) �躤��
	_vector vMonsterLook = spMonster->GetLookVector();

	// �ڽ�(����) �׺� �� �ε���
	_int& iCurCellIndex = spMonster->GetCurNaviCellIndex();

	// �÷��̾� ��ġ
	_vector vPlayerPos = CObjectManager::GetInstance()->GetCurPlayer()->GetActorTranslation();

	// �ڽ�(����) -> �÷��̾�� ���� ���� �Ÿ�, y��ǥ ������ �÷��̾�� ���ϴ� ���⺤��(����ȭ)
	_vector vMonsterToPlayer = vPlayerPos - spMonster->GetActorTranslation();

	_float fMonsterToPlayerDistance = XMVector3Length(vMonsterToPlayer).m128_f32[0];

	_vector vMonsterToPlayer_Normal = XMVectorSetY(vMonsterToPlayer, 0.0f);
	vMonsterToPlayer_Normal = XMVector3Normalize(vMonsterToPlayer_Normal);

	// ���� �̵��ӵ�
	_float fMoveSpeed = spMonster->GetMoveSpeed();
#pragma endregion ������ ����ó���� �ʿ��� ����

#pragma region ȸ��

	_float fAngle = 0.0f;

#pragma endregion ȸ��

	// ��� ����
	if (spMonster->GetCurHp() <= 0.0f)
	{
		RequestChangeState(MONSTER_STATE::DIE1);
	}

	_float fHp = spMonster->GetCurHp();

	// 2������ ����
	if (spMonster->GetCurHp() <= spMonster->GetMaxHp() * 0.6f && 0 == m_iPaize)
	{
		shared_ptr<CBoss_Andreus> spAndreus = dynamic_pointer_cast<CBoss_Andreus>(m_wpOwnerActor.lock());

		_vector vAndreusPos = spAndreus->GetActorTranslation();
		_vector vAndreusRight = XMVector3Normalize(spAndreus->GetRightVector());
		_vector vAndreusUp = XMVector3Normalize(spAndreus->GetUpVector());
		_vector vAndreusLook = XMVector3Normalize(spAndreus->GetLookVector());

		_vector vStartCamPos = vAndreusPos + (vAndreusUp * 1500.f) + (vAndreusLook * 1500.f);
		_vector vEndCamPos = vAndreusPos + (vAndreusRight * 1000.f) + (vAndreusUp * 800.f) + (vAndreusLook * 1000.f);

		_vector vStartCamAt = vAndreusPos + (vAndreusLook * 200.f);
		_vector vEndCamAt = vAndreusPos + (vAndreusLook * 300.f) + (vAndreusUp * 300.f);

		RequestChangeState(MONSTER_STATE::DETECTED);
		ChangePaize(1, vStartCamPos, vEndCamPos, vStartCamAt, vEndCamAt, 6.0f);
		CSoundManager::GetInstance()->PlaySoundEx("Andrius_PaizeChange.mp3", EFFECT_10, 0.4f);

		// ������ ��ȯ ����Ʈ
		MakeEffect(L"Andrius_PaizeChange_AuraFloor", XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));
		MakeEffect(L"Andrius_PaizeChange_Tonado", XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f));

		MakeParticle(L"Sprite_Andrius_Thunder_One", XMVectorSet(500.0f, 200.0f, 0.0f, 0.0f));
		MakeParticle(L"Sprite_Andrius_Thunder_One", XMVectorSet(0.0f, 200.0f, 500.0f, 0.0f));
		MakeParticle(L"Sprite_Andrius_Thunder_One", XMVectorSet(500.0f, 200.0f, 500.0f, 0.0f));

		for (_uint i = 0; i < 3; ++i)
		{
			MakeEffect(L"Andrius_PaizeChange_Thunder_One", XMVectorSet(0.0f, 100.0f * (_float)i, 0.0f, 0.0f));
			MakeEffect(L"Andrius_PaizeChange_Thunder_Two", XMVectorSet(0.0f, 100.0f * (_float)i, 0.0f, 0.0f));
			MakeEffect(L"Andrius_PaizeChange_Thunder_Three", XMVectorSet(0.0f, 100.0f * (_float)i, 0.0f, 0.0f));
			MakeParticle(L"Particle_Andrius_PaizeChange", XMVectorSet(0.0f, 200.0f, 0.0f, 0.0f));
		}
	}

	/*if (spMonster->GetCurHp() <= spMonster->GetMaxHp() * 0.6f &&
		m_iPaize == IS_PAIZE1)
		ChangePaize(BOSS_PAIZE::IS_PAIZE2, m_fPaize1ProductionTime);*/

	// ���
	if (m_iCurrState & IS_DIE)
	{
		if (m_iCurrState & MONSTER_STATE::DIE1)
			RequestChangeState(MONSTER_STATE::DIE1);
		else if (m_iCurrState & MONSTER_STATE::DIE2)
			RequestChangeState(MONSTER_STATE::DIE2);
		else if (m_iCurrState & MONSTER_STATE::DIE3)
			RequestChangeState(MONSTER_STATE::DIE3);
	}
	// �ǰ�
	else if (m_iCurrState & IS_HIT)
	{
		// ������ �ǰ� ����� ����.
	}
	// ������
	else if (m_iCurrState & IS_MOVE)
	{
		_vector vZ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		fAngle = CaculateMonsterRotation(vMonsterToPlayer, vZ);

		if (m_fPrevAngle < fAngle)
			fAngle = lerp(m_fPrevAngle, fAngle, 0.05f);
		else
			fAngle = lerp(fAngle, m_fPrevAngle, 0.05f);

		if (m_iCurrState & MONSTER_STATE::TRACE)
		{
			spMonster->SetRotation(XMVectorSet(0.0f, fAngle, 0.0f, 0.0f));
			spMonster->SetActorTranslation(vMonsterPos + (XMVector3Normalize(vMonsterLook) * fMoveSpeed * _fTimeDelta), &iCurCellIndex);
			m_fPrevAngle = fAngle;
		}

		/* 3 ������ */
		if (m_iPaize == IS_PAIZE3)
		{
			if(FAILED(DoPattern_Paize3(fMonsterToPlayerDistance)))
				return;

			if (FAILED(DoPattern_Paize2(fMonsterToPlayerDistance)))
				return;

			if (FAILED(DoPattern_Paize1(fMonsterToPlayerDistance)))
				return;
		}
		/* 2 ������ */
		if (m_iPaize == IS_PAIZE2)
		{
			if (FAILED(DoPattern_Paize2(fMonsterToPlayerDistance)))
				return;

			if (FAILED(DoPattern_Paize1(fMonsterToPlayerDistance)))
				return;
		}
		/* 1 ������ */
		else if (m_iPaize == IS_PAIZE1)
		{
			if (FAILED(DoPattern_Paize1(fMonsterToPlayerDistance)))
				return;
		}
	}
	// ���� �� ����
	else if (m_iCurrState & IS_ATTACK)
	{
		if (m_iCurrState & IS_NORMALATTACK)
		{
			// [ ������ ]
			if (m_iCurrState & IS_ATTACK1)
			{
				if (m_iCurrState & MONSTER_STATE::ATTACK1_START)
				{
					if (1.4f <= m_tInfo.m_fAttack1CoolTime && m_tInfo.m_fAttack1CoolTime < 1.6f)
					{
						switch (m_wpAnimModel.lock()->GetAnimationIndex())
						{
						case ANDREUS_ANIM::SMASH_L:
						{
							SetAttackColliderEnable(L"LeftArm", true);
							break;
						}
						case ANDREUS_ANIM::SMASH_R:
						{
							SetAttackColliderEnable(L"RightArm", true);
							break;
						}
						}
					}
					else
					{
						SetAttackColliderEnable(L"LeftArm", false);
						SetAttackColliderEnable(L"RightArm", false);
					}
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK1_CYCLE)
				{

				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK1_END)
				{

				}
			}
			// [ ������ ����ġ�� ]
			else if (m_iCurrState & IS_ATTACK2)
			{
				if (m_iCurrState & MONSTER_STATE::ATTACK2_START)
				{
					if (1.0f <= m_tInfo.m_fAttack2CoolTime && m_tInfo.m_fAttack2CoolTime < 1.25f)
						SetAttackColliderEnable(L"Tail", true);
					else
						SetAttackColliderEnable(L"Tail", false);
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK2_CYCLE)
				{

				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK2_END)
				{

				}
			}
			// [ ���ƿö� ������� ]
			else if (m_iCurrState & IS_ATTACK3)
			{
				if (m_iCurrState & MONSTER_STATE::ATTACK3_START)
				{
					if (1.25f <= m_tInfo.m_fAttack3CoolTime)
						SetAttackColliderEnable(L"RightArm", false);
					else if(1.0f <= m_tInfo.m_fAttack3CoolTime)
						SetAttackColliderEnable(L"RightArm", true);

					_float	fRemainDistance = XMVector4Length(XMLoadFloat4(&m_vTargetPos) - vMonsterPos).m128_f32[0];
					_vector	vAttackDir = XMVector3Normalize(XMLoadFloat4(&m_vTargetPos) - vMonsterPos);

					if (fRemainDistance < 100.0f)
						fRemainDistance = 0.0f;

					spMonster->SetActorTranslation(vMonsterPos + vAttackDir * 25.0f * fMoveSpeed * (fRemainDistance / m_fJump_Attack_Distance) * _fTimeDelta, &iCurCellIndex);
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK3_CYCLE)
				{

				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK3_END)
				{

				}
			}
			// [ ���ڸ����� �ѹ��� ���� ]
			else if (m_iCurrState & IS_ATTACK4)
			{
				if (m_iCurrState & MONSTER_STATE::ATTACK4_START)
				{
					if (0.6f < m_tInfo.m_fAttack4CoolTime && !m_bAttack_Turn_SoundCompleted)
					{
						CSoundManager::GetInstance()->PlaySoundEx("Andrius_Attack_Turn.mp3", EFFECT_10, 0.4f);
						m_bAttack_Turn_SoundCompleted = true;
					}

					if (2.6f < m_tInfo.m_fAttack4CoolTime)
						SetAttackColliderEnable(L"Tail", false);
					else if (2.2f <= m_tInfo.m_fAttack4CoolTime)
						SetAttackColliderEnable(L"Tail", true);
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK4_CYCLE)
				{
					
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK4_END)
				{

				}
			}
			// [ (�÷��̾ �ڿ� ������?) �齺�� ]
			else if (m_iCurrState & IS_ATTACK5)
			{
				m_fJump_Back_RunTime += _fTimeDelta;

				if (m_iCurrState & MONSTER_STATE::ATTACK5_START)
				{
					if (m_fJump_Back_RunTime >= m_fJump_Back_MinTime)
						m_fJump_Back_RunTime = m_fJump_Back_MinTime;

					spMonster->SetActorTranslation(vMonsterPos - XMVector3Normalize(vMonsterLook) * 
						5.0f * fMoveSpeed * ((m_fJump_Back_MinTime - m_fJump_Back_RunTime) / m_fJump_Back_MinTime) * _fTimeDelta, &iCurCellIndex);
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK5_CYCLE)
				{

				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK5_END)
				{

				}
			}
		}
		else if (m_iCurrState & IS_PATTERN)
		{
			if (m_iCurrState & IS_PATTERN1)
			{

			}
			if (m_iCurrState & IS_PATTERN2)
			{

			}
			if (m_iCurrState & IS_PATTERN3)
			{

			}
			if (m_iCurrState & IS_PATTERN4)
			{

			}
			if (m_iCurrState & IS_PATTERN4)
			{

			}
		}
	}
	// ���̵�
	else if (m_iCurrState == MONSTER_STATE::IDLE)
	{
		// �÷��̾�� �Ÿ��� ���� �̻� ��������� DETECTED ���·� �����Ѵ�.
		if (fMonsterToPlayerDistance < m_tInfo.m_fDetectingRadius)
			RequestChangeState(MONSTER_STATE::DETECTED);
	}
	// �÷��̾� ����
	else if (m_iCurrState & MONSTER_STATE::DETECTED)
	{
		m_fDetectingProductionRunTime += _fTimeDelta;
		if (m_fDetectingProductionRunTime >= m_fDetectingProductionTime)
			RequestChangeState(MONSTER_STATE::IDLE);
	}
}

void CAndreusStateMachine::ManagementActorCoolTime(const _float& _fTimeDelta)
{
	/* Attack1 */
	m_tInfo.m_fAttack1CoolTime += _fTimeDelta;
	if (m_tInfo.m_fAttack1CoolTime >= m_tInfo.m_fAttack1MinTime)
		m_tInfo.m_fAttack1CoolTime = m_tInfo.m_fAttack1MinTime;

	/* Attack2 */
	m_tInfo.m_fAttack2CoolTime += _fTimeDelta;
	if (m_tInfo.m_fAttack2CoolTime >= m_tInfo.m_fAttack2MinTime)
		m_tInfo.m_fAttack2CoolTime = m_tInfo.m_fAttack2MinTime;

	/* Attack3 */
	m_tInfo.m_fAttack3CoolTime += _fTimeDelta;
	if (m_tInfo.m_fAttack3CoolTime >= m_tInfo.m_fAttack3MinTime)
		m_tInfo.m_fAttack3CoolTime = m_tInfo.m_fAttack3MinTime;

	/* Attack4 */
	m_tInfo.m_fAttack4CoolTime += _fTimeDelta;
	if (m_tInfo.m_fAttack4CoolTime >= m_tInfo.m_fAttack4MinTime)
		m_tInfo.m_fAttack4CoolTime = m_tInfo.m_fAttack4MinTime;

	/* Attack5 */
	m_tInfo.m_fAttack5CoolTime += _fTimeDelta;
	if (m_tInfo.m_fAttack5CoolTime >= m_tInfo.m_fAttack5MinTime)
		m_tInfo.m_fAttack5CoolTime = m_tInfo.m_fAttack5MinTime;

	// ============================================================= //

	/* Pattern1 */
	m_tInfo.m_fPattern1CoolTime += _fTimeDelta;
	if (m_tInfo.m_fPattern1CoolTime >= m_tInfo.m_fPattern1MinTime)
		m_tInfo.m_fPattern1CoolTime = m_tInfo.m_fPattern1MinTime;

	/* Pattern2 */
	m_tInfo.m_fPattern2CoolTime += _fTimeDelta;
	if (m_tInfo.m_fPattern2CoolTime >= m_tInfo.m_fPattern2MinTime)
		m_tInfo.m_fPattern2CoolTime = m_tInfo.m_fPattern2MinTime;

	/* Pattern3 */
	m_tInfo.m_fPattern3CoolTime += _fTimeDelta;
	if (m_tInfo.m_fPattern3CoolTime >= m_tInfo.m_fPattern3MinTime)
		m_tInfo.m_fPattern3CoolTime = m_tInfo.m_fPattern3MinTime;

	/* Pattern4 */
	m_tInfo.m_fPattern4CoolTime += _fTimeDelta;
	if (m_tInfo.m_fPattern4CoolTime >= m_tInfo.m_fPattern4MinTime)
		m_tInfo.m_fPattern4CoolTime = m_tInfo.m_fPattern4MinTime;

	/* Pattern5 */
	m_tInfo.m_fPattern5CoolTime += _fTimeDelta;
	if (m_tInfo.m_fPattern5CoolTime >= m_tInfo.m_fPattern5MinTime)
		m_tInfo.m_fPattern5CoolTime = m_tInfo.m_fPattern5MinTime;
}

void CAndreusStateMachine::ChangePaize(_uint _iPaizeIndex, _vector _vCamStartPos, _vector _vCamEndPos, _vector _vCamStartAt, _gvector _vCamEndAt, const _float& _fPaizeProductionTime)
{
	m_iPaize = _iPaizeIndex;
	m_bProduction = true;

	shared_ptr<CProductionCamera> spProductionCam = dynamic_pointer_cast<CProductionCamera>(CCameraManager::GetInstance()->GetCurCamera());

	spProductionCam->BeginProduction(_vCamStartPos, _vCamEndPos, _vCamStartAt, _vCamEndAt, _fPaizeProductionTime);
	spProductionCam->BeginShaking(3.5f, 5.f, 2.f);
	spProductionCam->SetProductionRatio(1.f, 1.0f, 1.0f);

	CSoundManager::GetInstance()->StopAll();
	CSoundManager::GetInstance()->PlayBGMLoop("Music21_32.ogg", 0.4f);
}

void CAndreusStateMachine::RequestChangeState(_ullong _iMonsterStateFlag, REQUEST_DIR _eDirection, ATTACK_DESC _AttackDesc)
{
#pragma region ������ ����ó���� �ʿ��� ����
	// �ڽ�(����) ������
	shared_ptr<CMonster> spMonster = dynamic_pointer_cast<CMonster>(m_wpOwnerActor.lock());

	// �ڽ�(����) ��ġ
	_vector vMonsterPos = spMonster->GetActorTranslation();

	// �ڽ�(����) �躤��
	_vector vMonsterLook = spMonster->GetLookVector();

	// �ڽ�(����) �׺� �� �ε���
	_int& iCurCellIndex = spMonster->GetCurNaviCellIndex();

	// �÷��̾� ��ġ
	_vector vPlayerPos = CObjectManager::GetInstance()->GetCurPlayer()->GetActorTranslation();

	// �ڽ�(����) -> �÷��̾�� ���� ���� �Ÿ�, y��ǥ ������ �÷��̾�� ���ϴ� ���⺤��(����ȭ)
	_vector vMonsterToPlayer = vPlayerPos - spMonster->GetActorTranslation();

	_float fMonsterToPlayerDistance = XMVector3Length(vMonsterToPlayer).m128_f32[0];

	_vector vMonsterToPlayer_Normal = XMVectorSetY(vMonsterToPlayer, 0.0f);
	vMonsterToPlayer_Normal = XMVector3Normalize(vMonsterToPlayer_Normal);

	// ���� �̵��ӵ�
	_float fMoveSpeed = spMonster->GetMoveSpeed();

	// �ʱ� �������� �������� ���� ������ ���⺤��, �ϴ� �ӽ÷� �׳� �����ϰ� �غ�
	_vector vRandomDir = XMVectorSet(_float(rand() % 1000), _float(rand() % 1000), _float(rand() % 1000), 0.f);
	vRandomDir = XMVector3Normalize(vRandomDir);
#pragma endregion ������ ����ó���� �ʿ��� ����

#pragma region ȸ��
	_float fAngle = 0.0f;

	if (!XMVectorIsNaN(vMonsterToPlayer).m128_f32[0])
	{
		_vector vZ = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		fAngle = CaculateMonsterRotation(vMonsterToPlayer, vZ);
	}
#pragma endregion ȸ��

	// ��û���� ���� ��ȭ�� ������ ���� ���¿� ���ٸ� return, ������ �ǰ������̶�� ���
	if ((_iMonsterStateFlag == m_iCurrState && _eDirection == m_eDirrection) && !(_iMonsterStateFlag & IS_HIT))
		return;

	// ��û���� ������ ���⼺
	if (_eDirection != REQUEST_DIR::DIR_NONE)
		m_eDirrection = _eDirection;

	shared_ptr<CBoss_Andreus> spAndreus = dynamic_pointer_cast<CBoss_Andreus>(m_wpOwnerActor.lock());

	_vector vAndreusPos = spAndreus->GetActorTranslation();
	_vector vAndreusRight = XMVector3Normalize(spAndreus->GetRightVector());
	_vector vAndreusUp = XMVector3Normalize(spAndreus->GetUpVector());
	_vector vAndreusLook = XMVector3Normalize(spAndreus->GetLookVector());

	// 1. ��û���� ���°� DIE��� -> �ֿ켱������ ���� ���¸� ó���Ѵ�.
	if (_iMonsterStateFlag & IS_DIE && dynamic_pointer_cast<CMonster>(m_wpOwnerActor.lock())->GetCurHp() <= 0.0f)
	{
		// �ػ�� ó�� ���߿� �� ��.��
		m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::HOWL);
		m_iCurrState = MONSTER_STATE::DIE1;

		CSoundManager::GetInstance()->StopAll();

		// ���� ����
		_vector vStartCamPos = XMVectorSet(0.f, 200.f, 500.f, 1.f);
		_vector vEndCamPos = XMVectorSet(0.f, 200.f, 500.f, 1.f);

		_vector vStartCamAt = XMVectorSet(0.f, 200.f, 1300.f, 1.f);
		_vector vEndCamAt = XMVectorSet(0.f, 200.f, 1300.f, 1.f);

		shared_ptr<CProductionCamera> spProductionCam = dynamic_pointer_cast<CProductionCamera>(CCameraManager::GetInstance()->GetCurCamera());

		spProductionCam->BeginProduction(vStartCamPos, vEndCamPos, vStartCamAt, vEndCamAt, 3.f);
		spProductionCam->BeginShaking(0.0f, 2.f, 1.5f);
		spProductionCam->SetProductionRatio(1.f, 1.0f, 1.0f);

		// �ݶ��̴� off
		SetAllAttackColliderOff();
	}
	// 2. ��û���� ���°� HIT��� -> HIT_TYPE�� �Ǵ��Ͽ� ���������� ó���Ѵ�.
	else if (_iMonsterStateFlag & IS_HIT)
	{
		// 2.1 ��¥�� �������� �� ������ ����, ���� �� �߿��� �ǰݴ����� �ʴ´�.
		if (m_iCurrState & IS_DIE || m_iCurrState & MONSTER_STATE::DETECTED)
			return;

		// 2.2 ������ �޾Ҵ� ������ ������, ���� �̸��� ���Ͽ� ������ return;
		if (m_HittedAttack_Desc.wstrOwnerName == _AttackDesc.wstrOwnerName &&
			m_HittedAttack_Desc.wstrAttackName == _AttackDesc.wstrAttackName &&
			false == _AttackDesc.bMultiHit)
			return;

		// 2.3 ������ �ǰ� ����� ����.
		// m_iCurrState = _iMonsterStateFlag;

		// HP�� ��´�.
		Hitted(_AttackDesc, _AttackDesc.eDamage, XMVectorSet(0.f, 350.f, 0.f, 0.f));
		MakeHitEffect();
		dynamic_pointer_cast<CBossMonster>(m_wpOwnerActor.lock())->SetHPBarEnable(true);
		m_bHit = true;
		m_fHitTimeAcc = 0.f;

		// ���� ATTACK_DESC�� ����Ѵ�.
		m_HittedAttack_Desc = _AttackDesc;
	}
	// 3. ��û���� ���°� �����ӵ��� �� �ϳ����
	else if (_iMonsterStateFlag & IS_MOVE)
	{
		// 3.1 ������ ������ �ֵ��� ������� ���� ���̴�.
		if (m_iCurrState & IS_DIE)
			return;

		// 3.2 �׷��� �ʴٸ� ���� ��ȭ�� ����Ѵ�.

		if (_iMonsterStateFlag & MONSTER_STATE::TRACE)
		{
			// ���� �÷��̾���� �Ÿ��� �����Ÿ� �ȿ� �ְ�, ��� ������ ��Ÿ�� �̶��
			/*if (1000.0f >= fMonsterToPlayerDistance)
			{
				switch (rand() % 2)
				{
				case 0:
				{
					m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::WALK_TO_);
					break;
				}
				case 1:
				{
					m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::WALK_TO_L);
					break;
				}
				}
			}*/
			m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::WALK);
			m_bAttack_Turn_SoundCompleted = false;
			m_bAttack_Jump_SoundCompleted = false;
		}

		m_iCurrState = _iMonsterStateFlag;

		// �ݶ��̴� off
		SetAllAttackColliderOff();
	}
	// 4. ��û���� ���°� ���ݵ��� �� �ϳ����
	else if (_iMonsterStateFlag & IS_ATTACK)
	{
		// 4.1 �׾����� ������ �� ����.
		if (m_iCurrState & IS_DIE )
			return;

		spAndreus->SetRotation(XMVectorSet(0.0f, fAngle, 0.0f, 0.0f));

		_matrix matUpdatedWorld = spAndreus->CalculateWorldMatrix_Immediately();

		_vector vAndreusRight = XMVector3Normalize(matUpdatedWorld.r[0]);
		_vector vAndreusUp = XMVector3Normalize(matUpdatedWorld.r[1]);
		_vector vAndreusLook = XMVector3Normalize(matUpdatedWorld.r[2]);

		// 4.2 �Ϲ� ����
		if (_iMonsterStateFlag & IS_NORMALATTACK)
		{
			if (_iMonsterStateFlag & IS_ATTACK_START)
			{
				switch (_iMonsterStateFlag)
				{
				case ATTACK1_START: // [ ������ ]
				{
					// ���� �ȵ巹�콺�� Look����, �÷��̾ ���ʿ� ���� ��
					_float fAngle = CaculateMonsterRotation(vMonsterLook, vMonsterToPlayer_Normal);
					if (180.0f < fAngle)
					{
						m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::SMASH_L);
						MakeEffect(L"Andrius_Attack_One_R", vAndreusUp * 200.f);
						MakeEffect(L"Andrius_Attack_Two_R", vAndreusUp * 200.f);
					}
					else
					{
						m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::SMASH_R);
						MakeEffect(L"Andrius_Attack_One_L", vAndreusUp * 200.f);
						MakeEffect(L"Andrius_Attack_Two_L", vAndreusUp * 200.f);
					}
					MakeParticle_Attack1(XMVectorSet(0.0f, 200.f, 0.0f, 0.0f));

					m_Attack1_Desc.wstrAttackName = L"Andreus Attack1" + to_wstring(rand());
					SetAttackDescToAttackCollider(L"LeftArm", m_Attack1_Desc);
					SetAttackDescToAttackCollider(L"RightArm", m_Attack1_Desc);

					// ���� ���
					CSoundManager::GetInstance()->PlaySoundEx("Andrius_Attack_One.mp3", EFFECT_10, 0.4f);

					break;
				}

				case ATTACK2_START: // [ ������ ����ġ�� ]
				{
					// ���� �ȵ巹�콺�� Look����, �÷��̾ ���ʿ� ���� ��
					_float fAngle = CaculateMonsterRotation(vMonsterLook, vMonsterToPlayer_Normal);
					if (180.0f < fAngle)
					{
						m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::FLAP_L);
						spAndreus->SetRotation(spAndreus->GetRotation() + XMVectorSet(0.0f, 90.f, 0.0f, 0.0f));
						MakeEffect(L"Andrius_Attack_Tail_L", vAndreusUp * 200.f);
					}
					else
					{
						m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::FLAP_R);
						spAndreus->SetRotation(spAndreus->GetRotation() + XMVectorSet(0.0f, -90.f, 0.0f, 0.0f));
						MakeEffect(L"Andrius_Attack_Tail_R", vAndreusUp * 200.f);
					}
					MakeParticle_Attack2(XMVectorSet(0.0f, 100.f, 0.0f, 0.0f));

					m_Attack2_Desc.wstrAttackName = L"Andreus Attack2" + to_wstring(rand());
					SetAttackDescToAttackCollider(L"Tail", m_Attack2_Desc);

					// ���� ���
					CSoundManager::GetInstance()->PlaySoundEx("Andrius_Attack_Tail.mp3", EFFECT_10, 0.4f);

					break;
				}

				case ATTACK3_START: // [ ���ƿö� ������� ]
				{
					m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::JUMP);
					m_Attack3_Desc.wstrAttackName = L"Andreus Attack3" + to_wstring(rand());
					SetAttackDescToAttackCollider(L"RightArm", m_Attack3_Desc);

					XMStoreFloat4(&m_vTargetPos, vPlayerPos);
					m_fJump_Attack_Distance = XMVector4Length(vMonsterToPlayer).m128_f32[0];

					MakeEffect(L"Andrius_Attack_Three_IceField_Floor", -vAndreusPos + vPlayerPos + XMVectorSet(0.0f, 1.f, 0.0f, 0.f));
					MakeEffect(L"Andrius_Attack_Three_IceField_Niddle", - vAndreusPos + vPlayerPos);
					MakeEffect(L"Andrius_Attack_Three_IceField_Aura", - vAndreusPos + vPlayerPos);
					MakeParticle(L"Particle_Andrius_Attack_Three", -vAndreusPos + vPlayerPos + XMVectorSet(0.0f, 200.f, 0.0f, 0.f));

					for (_uint i = 0; i < 10; ++i)
					{
						_vector vRandomDir = MakeRandomDir(_float2(-1.f, 1.f), _float2(0.f, 0.00001f), _float2(-1.f, 1.f));
						_float fRandomRadius = MakeRandomRadius(_float2(400.f, 500.f));

						MakeDebris(L"Andrius_WolfStone", -vAndreusPos + vPlayerPos + vRandomDir * fRandomRadius, _float2(1.f, 3.f));
						MakeDebris(L"Andrius_WolfStone_One", -vAndreusPos + vPlayerPos + vRandomDir * fRandomRadius, _float2(1.f, 3.f));
						MakeDebris(L"Andrius_WolfStone_Two", -vAndreusPos + vPlayerPos + vRandomDir * fRandomRadius, _float2(1.f, 3.f));
						MakeDebris(L"Andrius_WolfStone_Three", -vAndreusPos + vPlayerPos + vRandomDir * fRandomRadius, _float2(1.f, 3.f));
						MakeDebris(L"Andrius_WolfStone_Four", -vAndreusPos + vPlayerPos + vRandomDir * fRandomRadius, _float2(1.f, 3.f));
						MakeDebris(L"Andrius_WolfStone_Five", -vAndreusPos + vPlayerPos + vRandomDir * fRandomRadius, _float2(1.f, 3.f));
						MakeDebris(L"Andrius_WolfStone_Six", -vAndreusPos + vPlayerPos + vRandomDir * fRandomRadius, _float2(1.f, 3.f));
					}

					// ī�޶� ����ŷ ��û
					shared_ptr<CCameraComponent> spCam = CCameraManager::GetInstance()->GetCurCamera();
					if (nullptr == spCam)
						break;

					if (L"ShoulderViewCameraComponent" == spCam->GetComponentName())
						break;

					shared_ptr<CProductionCamera> spProductionCam = dynamic_pointer_cast<CProductionCamera>(spCam);
					spProductionCam->BeginShaking(1.f, 0.7f, 2.f);

					// ���� ���
					CSoundManager::GetInstance()->PlaySoundEx("Andrius_Attack_Jump.mp3", EFFECT_10, 0.4f);

					break;
				}

				case ATTACK4_START: // [ ���ڸ����� �ѹ��� ���� ]
				{
					_float fAngle = CaculateMonsterRotation(vMonsterLook, vMonsterToPlayer_Normal);
					if (180.0f < fAngle)
						m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::DRIFT_L);
					else
						m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::DRIFT_R);

					MakeEffect(L"Andrius_Attack_Three_IceField_Floor", XMVectorSet(0.0f, 3.f, 0.0f, 0.f));
					MakeEffect(L"Andrius_Attack_Three_IceField_Niddle", XMVectorSet(0.0f, 3.f, 0.0f, 0.f));
					MakeEffect(L"Andrius_Attack_Three_IceField_Aura", XMVectorSet(0.0f, 3.f, 0.0f, 0.f));
					MakeEffect(L"Andrius_Attack_Turn", vAndreusUp * 100.f);
					MakeParticle(L"Particle_Andrius_Attack_Turn_Two", XMVectorSet(0.0f, 150.f, 0.0f, 0.f));

					m_Attack4_Desc.wstrAttackName = L"Andreus Attack4" + to_wstring(rand());
					SetAttackDescToAttackCollider(L"Tail", m_Attack4_Desc);

					// ���� ���
					// CSoundManager::GetInstance()->PlaySoundEx("Andrius_Attack_Turn.mp3", EFFECT_10, 0.4f);

					break;
				}

				case ATTACK5_START: // [ �齺�� ]
				{
					m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::JUMP_BACK);
					break;
				}
				}
			}
			else if (_iMonsterStateFlag & IS_ATTACK_CYCLE)
			{

			}
			else if (_iMonsterStateFlag & IS_ATTACK_END)
			{
				switch (_iMonsterStateFlag)
				{
				case ATTACK1_END:
				{
					break;
				}
				case ATTACK2_END:
				{
					break;
				}
				case ATTACK3_END:
				{
					break;
				}

				case ATTACK4_END:
				{
					break;
				}
				}
			}
			m_iCurrState = _iMonsterStateFlag;
		}
		// 4.4 ���� ����
		else if (_iMonsterStateFlag & IS_PATTERN)
		{
			if (_iMonsterStateFlag & IS_ATTACK_START)
			{
				switch (_iMonsterStateFlag)
				{
				case PATTERN1_START:
				{
					
					break;
				}
				case PATTERN2_START:
				{
					m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::STAMP);
					MakeWindBlade(L"Andrius_Pattern_Two_WindBlade");

					// ���� ���
					CSoundManager::GetInstance()->PlaySoundEx("Andrius_WindBlade.mp3", EFFECT_10, 0.4f);

					break;
				}
				case PATTERN3_START:
				{
					m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::STAMP);
					MakeIceWave(L"Andrius_Pattern_Three_IceWave", vAndreusLook, vAndreusRight);

					// ���� ���
					CSoundManager::GetInstance()->PlaySoundEx("Andrius_Pattern_IceWave.mp3", EFFECT_10, 0.4f);

					break;
				}
				case PATTERN4_START:
				{

					break;
				}
				case PATTERN5_START:
				{

					break;
				}
				}
			}
			else if (_iMonsterStateFlag & IS_ATTACK_CYCLE)
			{

			}
			else if (_iMonsterStateFlag & IS_ATTACK_END)
			{
				switch (_iMonsterStateFlag)
				{
				case PATTERN1_END:
				{
					break;
				}
				case PATTERN2_END:
				{
					
					break;
				}
				case PATTERN3_END:
				{
					
					break;
				}

				case PATTERN4_END:
				{
					break;
				}
				}
			}

			m_iCurrState = _iMonsterStateFlag;
		}
	}
	// 5. ��û���� ���°� DETECTED���
	else if (_iMonsterStateFlag & IS_DETECTED)
	{
		if(_iMonsterStateFlag & MONSTER_STATE::DETECTED)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::HOWL_BS);
			m_iCurrState = MONSTER_STATE::DETECTED;

			// ���� ����
			_vector vStartCamPos = XMVectorSet(0.f, 350.f, 950.f, 1.f);
			_vector vEndCamPos = XMVectorSet(0.f, 350.f, 950.f, 1.f);

			_vector vStartCamAt = XMVectorSet(0.f, 350.f, 1300.f, 1.f);
			_vector vEndCamAt = XMVectorSet(0.f, 350.f, 1300.f, 1.f);

			shared_ptr<CProductionCamera> spProductionCam = dynamic_pointer_cast<CProductionCamera>(CCameraManager::GetInstance()->GetCurCamera());

			spProductionCam->BeginProduction(vStartCamPos, vEndCamPos, vStartCamAt, vEndCamAt, 5.f);
			spProductionCam->BeginShaking(1.7f, 4.f, 1.8f);
			spProductionCam->SetProductionRatio(1.f, 1.0f, 1.0f);

			CSoundManager::GetInstance()->PlaySoundEx("Andrius_PaizeChange.mp3", EFFECT_10, 0.4f);
		}
		return;
	}
	// 6. ��û���� ���°� IDLE�̶��
	else if (_iMonsterStateFlag == MONSTER_STATE::IDLE)
	{
		// 6.1 IDLE�� ���ư� �� �ִ� �������� üũ�Ѵ�.
		if (m_iCurrState & IS_DIE)
			return;

		// 6.2 ���Ͱ� ���� ���̿��ٸ�
		if (m_iCurrState & IS_ATTACK)
		{
			// [ �Ϲ� ���� ]
			if (m_iCurrState & IS_NORMALATTACK)
			{
				/* 1. ������ */
				if (m_iCurrState & IS_ATTACK1)
				{
					if (m_iCurrState & ATTACK1_START)
					{
						RequestChangeState(MONSTER_STATE::TRACE);
					}
					else if (m_iCurrState & ATTACK1_CYCLE)
					{

					}
					else if (m_iCurrState & ATTACK1_END)
					{

					}
					// m_iCurrState = MONSTER_STATE::TRACE;
					return;
				}
				/* 2. ������ ����ġ�� */
				else if (m_iCurrState & IS_ATTACK2)
				{
					if (m_iCurrState & ATTACK2_START)
					{
						RequestChangeState(MONSTER_STATE::TRACE);
					}
					else if (m_iCurrState & ATTACK2_CYCLE)
					{

					}
					else if (m_iCurrState & ATTACK2_END)
					{

					}
					// m_iCurrState = MONSTER_STATE::TRACE;
					return;
				}
				/* 3. ���ƿö� ������� */
				else if (m_iCurrState & IS_ATTACK3)
				{
					if (m_iCurrState & ATTACK3_START)
					{
						RequestChangeState(MONSTER_STATE::TRACE);
					}
					else if (m_iCurrState & ATTACK3_CYCLE)
					{

					}
					else if (m_iCurrState & ATTACK3_END)
					{

					}
					// m_iCurrState = MONSTER_STATE::TRACE;
					return;
				}
				/* 4. ���ڸ����� �ѹ��� ���� */
				else if (m_iCurrState & IS_ATTACK4)
				{
					if (m_iCurrState & ATTACK4_START)
					{
						RequestChangeState(MONSTER_STATE::TRACE);
					}
					else if (m_iCurrState & ATTACK4_CYCLE)
					{

					}
					else if (m_iCurrState & ATTACK4_END)
					{

					}
					// m_iCurrState = MONSTER_STATE::TRACE;
					return;
				}
				/* 5. �齺�� */
				else if (m_iCurrState & IS_ATTACK5)
				{
					if (m_iCurrState & ATTACK5_START)
					{
						RequestChangeState(MONSTER_STATE::TRACE);
						m_fJump_Back_RunTime = 0.0f;
					}
					else if (m_iCurrState & ATTACK5_CYCLE)
					{

					}
					else if (m_iCurrState & ATTACK5_END)
					{

					}
					// m_iCurrState = MONSTER_STATE::TRACE;
					return;
				}
			}
			// [ ���� ���� ]
			else if (m_iCurrState & IS_PATTERN)
			{
				if (m_iCurrState & IS_PATTERN1)
				{
					if (m_iCurrState & PATTERN1_START)
					{
						RequestChangeState(MONSTER_STATE::TRACE);
					}
					else if (m_iCurrState & PATTERN1_CYCLE)
					{

					}
					else if (m_iCurrState & PATTERN1_END)
					{

					}
				}
				else if (m_iCurrState & IS_PATTERN2)
				{
					if (m_iCurrState & PATTERN2_START)
					{
						RequestChangeState(MONSTER_STATE::TRACE);
					}
					else if (m_iCurrState & PATTERN2_CYCLE)
					{

					}
					else if (m_iCurrState & PATTERN2_END)
					{

					}
				}
				else if (m_iCurrState & IS_PATTERN3)
				{
					if (m_iCurrState & PATTERN3_START)
					{
						RequestChangeState(MONSTER_STATE::TRACE);
					}
					else if (m_iCurrState & PATTERN3_CYCLE)
					{

					}
					else if (m_iCurrState & PATTERN3_END)
					{

					}
				}
				else if (m_iCurrState & IS_PATTERN4)
				{
					if (m_iCurrState & PATTERN4_START)
					{
						RequestChangeState(MONSTER_STATE::TRACE);
					}
					else if (m_iCurrState & PATTERN4_CYCLE)
					{

					}
					else if (m_iCurrState & PATTERN4_END)
					{

					}
				}
				else if (m_iCurrState & IS_PATTERN5)
				{
					if (m_iCurrState & PATTERN5_START)
					{
						RequestChangeState(MONSTER_STATE::TRACE);
					}
					else if (m_iCurrState & PATTERN5_CYCLE)
					{

					}
					else if (m_iCurrState & PATTERN5_END)
					{

					}
				}
			}

			m_bAttack_Turn_SoundCompleted = false;
			m_bAttack_Jump_SoundCompleted = false;
		}
		else if (m_iCurrState & MONSTER_STATE::TRACE)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::WALK);
			m_iCurrState = MONSTER_STATE::TRACE;
		}
		// 6.4 ������ ��Ʈ ����� ����.
		else if (m_iCurrState & IS_HIT)
		{
			/*m_wpAnimModel.lock()->SetAnimationIndex(DEFENDER_ANIM::TRACE);
			m_iCurrState = MONSTER_STATE::TRACE;*/
		}
		// 6.5 ���Ͱ� DETECTED ��� �� �̿��ٸ�
		else if (m_iCurrState & IS_DETECTED)
		{
			if (m_iCurrState & DETECTED)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::HOWL);
				m_iCurrState = MONSTER_STATE::DETECTED_CYCLE;
			}
			else if (m_iCurrState & DETECTED_CYCLE)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::HOWL_AS);
				m_iCurrState = MONSTER_STATE::DETECTED_END;
			}
			else if (m_iCurrState & DETECTED_END)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANDREUS_ANIM::WALK);
				m_iCurrState = MONSTER_STATE::TRACE;
				m_bProduction = false;
			}
		}

		// �ݶ��̴� off
		SetAllAttackColliderOff();
	}
	m_fPrevAngle = fAngle;
}

void CAndreusStateMachine::MakeParticle(const wstring& _wstrParticleName, _vector _vOffset)
{
	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RecycleFromPool(_wstrParticleName);
	if (nullptr != spActor)
	{
		spActor->SetActorTranslation_NoNavigation(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
	}
}

void CAndreusStateMachine::MakeParticle_Attack1(_vector _vOffset)
{
	MakeParticle(L"Particle_Andrius_NormalAttack_One", _vOffset);
}
void CAndreusStateMachine::MakeParticle_Attack2(_vector _vOffset)
{
	MakeParticle(L"Particle_Andrius_Attack_Turn", _vOffset);
}
void CAndreusStateMachine::MakeParticle_Attack3(_vector _vOffset, _float2 _vLifeTime)
{
	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RecycleFromPool(L"Particle_Andrius_Pattern_Three_IceWave");
	if (nullptr != spActor)
	{
		dynamic_pointer_cast<CParticleActor>(spActor)->SetLifeTime(_vLifeTime);
		spActor->SetActorTranslation_NoNavigation(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
	}
}

void CAndreusStateMachine::MakeEffect(const wstring& _wstrEffectMeshName, _vector _vOffset)
{
	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->RecycleFromPool(_wstrEffectMeshName);
	if (nullptr != spActor)
	{
		_float3 vOwnerRotation;
		XMStoreFloat3(&vOwnerRotation, XMVectorSetZ(XMVectorSetX(m_wpOwnerActor.lock()->GetRotation(), 0.f), 0.f));

		_float3 vRotation = dynamic_pointer_cast<CEffectMeshActor>(spActor)->GetEffectMeshDesc().vRotation;

		spActor->SetRotation(XMLoadFloat3(&vOwnerRotation) + XMLoadFloat3(&vRotation));
		spActor->SetActorTranslation_NoNavigation(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
	}
}
void CAndreusStateMachine::MakeDebris(const wstring& _wstrEffectMeshName, _vector _vOffset, _float2 _vLifeTime)
{
	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->RecycleFromPool(_wstrEffectMeshName);
	if (nullptr != spActor)
		spActor->SetActorTranslation_NoNavigation(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);

	dynamic_pointer_cast<CMovingEffect>(spActor)->SetLifeTime(_vLifeTime);
}
void CAndreusStateMachine::MakeIceWave(const wstring& _wstrEffectMeshName, _vector _vMonLook, _vector _vMonRight)
{
	m_Pattern3_Desc.wstrAttackName = L"Andreus Pattern3" + to_wstring(rand());

	for (_uint i = 0; i < 10; ++i)
	{
		shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->RecycleFromPool(_wstrEffectMeshName);

		if (nullptr != spActor)
		{
			_vector vOwnerPos = GetOwnerActor().lock()->GetActorTranslation();
			_vector vOwnerRotation = GetOwnerActor().lock()->GetRotation();
			_vector vRightOffset;

			if (0 == i % 2)
			{
				vRightOffset = -_vMonRight;
				MakeEffect(L"Andrius_Pattern_Three_IceFloor", (400.f * (_float)i) * _vMonLook + XMVectorSet(0.f, 2.f, 0.f, 0.f));
				MakeParticle_Attack3(400.f * (_float)(i) * _vMonLook + 200.f * XMVectorSet(0.f, 1.f, 0.f, 0.f), _float2(1.2f + 0.1f * _float(i), 4.2f + 0.1f * _float(i)));
			}
			else
			{						
				vRightOffset = _vMonRight;
			}

			for (_uint j = 0; j < 2; ++j)
			{
				_vector vRandomDir = MakeRandomDir(_float2(-1.f, 1.f), _float2(0.f, 0.0000001f), _float2(-1.f, 1.f));
				_float fRandomRadius = MakeRandomRadius(_float2(100.f, 150.f));

				MakeDebris(L"Andrius_Pattern_Three_IceDebris", (300.f * (_float)i) * _vMonLook + 100.f * vRightOffset + vRandomDir * fRandomRadius, _float2(4.2f + 0.1f * _float(i), 6.2f + 0.1f * _float(i)));
				MakeDebris(L"Andrius_Pattern_Three_IceDebris_One", (300.f * (_float)i) * _vMonLook + 100.f * vRightOffset + vRandomDir * fRandomRadius, _float2(4.2f + 0.1f * _float(i), 6.2f + 0.1f * _float(i)));
				MakeDebris(L"Andrius_Pattern_Three_IceDebris_Two", (300.f * (_float)i) * _vMonLook + 100.f * vRightOffset + vRandomDir * fRandomRadius, _float2(4.2f + 0.1f * _float(i), 6.2f + 0.1f * _float(i)));
				MakeDebris(L"Andrius_Pattern_Three_IceDebris_Three", (300.f * (_float)i) * _vMonLook + 100.f * vRightOffset + vRandomDir * fRandomRadius, _float2(4.2f + 0.1f * _float(i), 6.2f + 0.1f * _float(i)));
				MakeDebris(L"Andrius_Pattern_Three_IceDebris_Four", (300.f * (_float)i) * _vMonLook + 100.f * vRightOffset + vRandomDir * fRandomRadius, _float2(4.2f + 0.1f * _float(i), 6.2f + 0.1f * _float(i)));
				MakeDebris(L"Andrius_Pattern_Three_IceDebris_Five", (300.f * (_float)i) * _vMonLook + 100.f * vRightOffset + vRandomDir * fRandomRadius, _float2(4.2f + 0.1f * _float(i), 6.2f + 0.1f * _float(i)));
				MakeDebris(L"Andrius_Pattern_Three_IceDebris_Six", (300.f * (_float)i) * _vMonLook + 100.f * vRightOffset + vRandomDir * fRandomRadius, _float2(4.2f + 0.1f * _float(i), 6.2f + 0.1f * _float(i)));
			}

			_float3 vRotation = dynamic_pointer_cast<CEffectMeshActor>(spActor)->GetEffectMeshDesc().vRotation;

			spActor->SetRotation(vOwnerRotation + XMLoadFloat3(&vRotation));
			spActor->SetActorTranslation_NoNavigation(vOwnerPos + (300.f * (_float)i) * _vMonLook + 200.f * vRightOffset);

			dynamic_pointer_cast<CMonsterAttackEffect>(spActor)->SetLifeTime(_float2(1.2f + 0.1f * _float(i), 4.2f + 0.1f * _float(i)));
			dynamic_pointer_cast<CMonsterAttackEffect>(spActor)->SetAttackDesc(m_Pattern3_Desc);
		}
	}
}
void CAndreusStateMachine::MakeWindBlade(const wstring& _wstrEffectMeshName)
{
	m_Pattern2_Desc.wstrAttackName = L"Andreus Pattern2" + to_wstring(rand());

	for (_uint i = 0; i < 6; ++i)
	{
		shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->RecycleFromPool(_wstrEffectMeshName);
		shared_ptr<CMonsterProjectileEffect> spMonsterProjectile;

		if(nullptr != spActor)
			spMonsterProjectile = dynamic_pointer_cast<CMonsterProjectileEffect>(spActor);

		if (nullptr != spMonsterProjectile)
		{
			_vector vOwnerPos = GetOwnerActor().lock()->GetActorTranslation();
			_vector vOwnerRotation = GetOwnerActor().lock()->GetRotation();
			_vector vOwnerLook = XMVector3Normalize(GetOwnerActor().lock()->GetLookVector());
			
			_float3 vRotation = spMonsterProjectile->GetEffectMeshDesc().vRotation;

			spMonsterProjectile->SetActorTranslation_NoNavigation(vOwnerPos);
			spMonsterProjectile->SetRotation(vOwnerRotation + XMLoadFloat3(&vRotation) + XMVectorSet(0.f, 60.f * (_float)i, 0.f, 0.f));
			_vector vShootDir = - XMVector3Normalize(spMonsterProjectile->CalculateWorldMatrix_Immediately().r[0]);
			spMonsterProjectile->SetShootDirrection(vShootDir);
			spMonsterProjectile->SetShootSpeed(3000.f);
			spMonsterProjectile->SetAttackDesc(m_Pattern2_Desc);
		}
	}
}

_vector CAndreusStateMachine::MakeRandomDir(_float2 _vWidth, _float2 _vHeight, _float2 _vDepth)
{
	_float3 vRandomDir;

	uniform_real_distribution<float>	WidthRange(_vWidth.x, _vWidth.y);
	uniform_real_distribution<float>	HeightRange(_vHeight.x, _vHeight.y);
	uniform_real_distribution<float>	DepthRange(_vDepth.x, _vDepth.y);

	vRandomDir.x = WidthRange(m_RandomNumber);
	vRandomDir.y = HeightRange(m_RandomNumber);
	vRandomDir.z = DepthRange(m_RandomNumber);

	return XMVector3Normalize(XMLoadFloat3(&vRandomDir));
}
_float CAndreusStateMachine::MakeRandomRadius(_float2 _vRadiusRange)
{
	_float fRandomRadius;

	uniform_real_distribution<float>	RadiusRange(_vRadiusRange.x, _vRadiusRange.y);

	fRandomRadius = RadiusRange(m_RandomNumber);

	return fRandomRadius;
}

HRESULT CAndreusStateMachine::DoPattern_Paize1(_float _fMonsterToPlayerDistance)
{
	// E_FAIL : ��� �����̳� ������ �����Ǿ���.
	// S_OK : �ƹ��� ������ �������� �ʴ� ����

	_vector vRandomDir;

	if (_fMonsterToPlayerDistance < m_tInfo.m_fAttack5Radius && m_tInfo.m_fAttack5CoolTime >= m_tInfo.m_fAttack5MinTime)
	{
		//MakeParticle(L"Sprite_Andrius_Thunder_One", vRandomDir);
		//MakeParticle(L"Sprite_Andrius_Thunder_One", vRandomDir);
		//MakeParticle(L"Sprite_Andrius_Thunder_One", vRandomDir);

		RequestChangeState(MONSTER_STATE::ATTACK5_START);
		m_tInfo.m_fAttack5CoolTime = 0.0f;
		return E_FAIL;
	}
	if (_fMonsterToPlayerDistance < m_tInfo.m_fAttack4Radius && m_tInfo.m_fAttack4CoolTime >= m_tInfo.m_fAttack4MinTime)
	{
		for (_uint i = 0; i < 5; ++i)
		{
			vRandomDir = MakeRandomDir(_float2(-500.f, 500.f), _float2(190.f, 200.f), _float2(-500.f, 500.f));
			MakeParticle(L"Sprite_Andrius_Thunder_One", vRandomDir * 700.f);
		}

		RequestChangeState(MONSTER_STATE::ATTACK4_START);
		m_tInfo.m_fAttack4CoolTime = 0.0f;
		return E_FAIL;
	}
	if (_fMonsterToPlayerDistance < m_tInfo.m_fAttack3Radius && m_tInfo.m_fAttack3CoolTime >= m_tInfo.m_fAttack3MinTime)
	{
		for (_uint i = 0; i < 5; ++i)
		{
			vRandomDir = MakeRandomDir(_float2(-500.f, 500.f), _float2(190.f, 200.f), _float2(-500.f, 500.f));
			MakeParticle(L"Sprite_Andrius_Thunder_One", vRandomDir * 700.f);
		}

		RequestChangeState(MONSTER_STATE::ATTACK3_START);
		m_tInfo.m_fAttack3CoolTime = 0.0f;
		return E_FAIL;
	}
	if (_fMonsterToPlayerDistance < m_tInfo.m_fAttack2Radius && m_tInfo.m_fAttack2CoolTime >= m_tInfo.m_fAttack2MinTime)
	{
		for (_uint i = 0; i < 5; ++i)
		{
			vRandomDir = MakeRandomDir(_float2(-500.f, 500.f), _float2(190.f, 200.f), _float2(-500.f, 500.f));
			MakeParticle(L"Sprite_Andrius_Thunder_One", vRandomDir * 700.f);
		}

		RequestChangeState(MONSTER_STATE::ATTACK2_START);
		m_tInfo.m_fAttack2CoolTime = 0.0f;
		return E_FAIL;
	}
	if (_fMonsterToPlayerDistance < m_tInfo.m_fAttack1Radius && m_tInfo.m_fAttack1CoolTime >= m_tInfo.m_fAttack1MinTime)
	{
		for (_uint i = 0; i < 5; ++i)
		{
			vRandomDir = MakeRandomDir(_float2(-500.f, 500.f), _float2(190.f, 200.f), _float2(-500.f, 500.f));
			MakeParticle(L"Sprite_Andrius_Thunder_One", vRandomDir * 700.f);
		}

		RequestChangeState(MONSTER_STATE::ATTACK1_START);
		m_tInfo.m_fAttack1CoolTime = 0.0f;
		return E_FAIL;
	}

	return S_OK;
}
HRESULT CAndreusStateMachine::DoPattern_Paize2(_float _fMonsterToPlayerDistance)
{
	_vector vRandomDir;

	if (_fMonsterToPlayerDistance < m_tInfo.m_fPattern3Radius && m_tInfo.m_fPattern3CoolTime >= m_tInfo.m_fPattern3MinTime)
	{
		for (_uint i = 0; i < 5; ++i)
		{
			vRandomDir = MakeRandomDir(_float2(-500.f, 500.f), _float2(190.f, 200.f), _float2(-500.f, 500.f));
			MakeParticle(L"Sprite_Andrius_Thunder_One", vRandomDir * 700.f);
		}

		RequestChangeState(MONSTER_STATE::PATTERN3_START);
		m_tInfo.m_fPattern3CoolTime = 0.0f;
		return E_FAIL;
	}
	if (_fMonsterToPlayerDistance < m_tInfo.m_fPattern2Radius && m_tInfo.m_fPattern2CoolTime >= m_tInfo.m_fPattern2MinTime)
	{
		for (_uint i = 0; i < 5; ++i)
		{
			vRandomDir = MakeRandomDir(_float2(-500.f, 500.f), _float2(190.f, 200.f), _float2(-500.f, 500.f));
			MakeParticle(L"Sprite_Andrius_Thunder_One", vRandomDir * 700.f);
		}

		RequestChangeState(MONSTER_STATE::PATTERN2_START);
		m_tInfo.m_fPattern2CoolTime = 0.0f;
		return E_FAIL;
	}
	/*if (fMonsterToPlayerDistance < m_tInfo.m_fPattern1Radius && m_tInfo.m_fPattern1CoolTime >= m_tInfo.m_fPattern1MinTime)
{
	RequestChangeState(MONSTER_STATE::PATTERN1_START);
	m_tInfo.m_fPattern1CoolTime = 0.0f;
	return E_FAIL;
}*/// -> ��帧�� ������ �����Ǿ�� �ϹǷ� ���߿� ó���Ѵ�.

	return S_OK;
}
HRESULT CAndreusStateMachine::DoPattern_Paize3(_float _fMonsterToPlayerDistance)
{
	if (_fMonsterToPlayerDistance < m_tInfo.m_fPattern4Radius && m_tInfo.m_fPattern4CoolTime >= m_tInfo.m_fPattern4MinTime)
	{
		RequestChangeState(MONSTER_STATE::PATTERN4_START);
		m_tInfo.m_fPattern4CoolTime = 0.0f;
		return E_FAIL;
	}

	return S_OK;
}