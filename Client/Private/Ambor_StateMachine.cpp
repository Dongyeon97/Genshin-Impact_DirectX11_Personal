#include "pch.h"
#include "Ambor_StateMachine.h"
#include "PlayerStateDefine.h"

#include "ObjectManager.h"
#include "CameraManager.h"
#include "NavigationManager.h"
#include "SoundManager.h"

#include "Level.h"
#include "Layer.h"
#include "Actor.h"
#include "Player.h"
#include "EffectMeshActor.h"
#include "SpriteActor.h"
#include "PlayerProjectileEffect.h"

#include "AnimModel.h"
#include "Animation.h"
#include "CameraComponent.h"
#include "ProductionCamera.h"
#include "Navigation.h"
#include "Cell.h"

using namespace PLAYER_STATE;

void CAmbor_StateMachine::Initialize()
{
	wstring wstrOwnerName = m_wpOwnerActor.lock()->GetActorName();
	_float fAtk = dynamic_pointer_cast<CCharactor>(m_wpOwnerActor.lock())->GetAtk();

	m_Attack1_Desc = { wstrOwnerName, L"Player_Ambor Attack1", NONE_ELEMENTAL, WEAK,		false,	fAtk * 1.0f };
	m_Attack2_Desc = { wstrOwnerName, L"Player_Ambor Attack2", NONE_ELEMENTAL, WEAK,		false,	fAtk * 1.2f };
	m_Attack3_Desc = { wstrOwnerName, L"Player_Ambor Attack3", NONE_ELEMENTAL, WEAK,		false,	fAtk * 1.4f };
	m_Attack4_Desc = { wstrOwnerName, L"Player_Ambor Attack4", NONE_ELEMENTAL, WEAK,		false,	fAtk * 1.8f };
	m_Attack5_Desc = { wstrOwnerName, L"Player_Ambor Attack5", NONE_ELEMENTAL, WEAK,		false,	fAtk * 2.0f };

	m_Skill1_Desc = { wstrOwnerName, L"Player_Ambor Skill1", PYRO, WEAK,		false,	fAtk * 5.0f };
	m_Skill2_Desc = { wstrOwnerName, L"Player_Ambor Skill2", PYRO, STRONG,		false,	fAtk * 5.4f };

#pragma region �ӵ�, �ð� ���� ��ü���� ����
////////////////
/* == �Ϲ� == */
////////////////
	m_fWalk_Speed = 100.0f;
	m_fTotal_Walk_Speed = 100.0f;

	m_fRun_Speed = 400.0f;

	m_fRun_Start_MinTime = 0.5f;
	m_fRun_Start_RunTime = 0.0f;
	m_fRun_Stop_MinTime = 0.5f;
	m_fRun_Stop_RunTime = 0.0f;

	m_fSprint_Speed = 800.0f;

	m_fSprint_Start_MinTime = 0.8f;
	m_fSprint_Start_RunTime = 0.0f;
	m_fSprint_Stop_MinTime = 0.6f;
	m_fSprint_Stop_RunTime = 0.0f;

	m_fJump_Power = 80.0f;
	m_fJump_RunTime = 0.0f;
	m_fJump_MinTime = 0.5f;

	m_fJumpYAcc = 0.0f;

	m_fClimb_Speed = 250.0f;
	m_fClimbDash_Speed = 1000.0f;

	m_fFly_Speed = 750.0f;

	m_fFlyStart_MinTime = 1.0f;
	m_fFlyStart_RunTime = 0.0f;

////////////////
/* == ���� == */
////////////////

	m_fAttack_ExitTime = 0.4f;
	m_fAttack_MinTime = 1.2f;
	m_fAttack_RunTime = 0.0f;
	m_fAttackMove_Speed = 5.0f;

	/* ELEMENTAL_SKILL */
	m_iElemental_Skill_ParticleCount = 0;
	m_fElemental_Skill_MinTime = 0.8f;
	m_fElemental_Skill_CoolTime = 4.0f;
	m_fElemental_Skill_RunTime = 0.0f;

	/* ELEMENTAL_BURST */
	m_fElemental_Burst_MinTime = 1.2f;
	m_fElemental_Burst_CoolTime = 3.0f;
	m_fElemental_Burst_RunTime = 0.0f;
	m_iProductionCount = 0;

	m_fHitH_MinTime = 0.5f;
	m_fHitH_RunTime = 0.0f;

	m_fHitL_MinTime = 0.5f;
	m_fHitL_RunTime = 0.0f;

	m_fHitThrow_MinTime = 1.0f;
	m_fHitThrow_RunTime = 0.0f;

	m_fHitFall_MinTime = 1.2f;
	m_fHitFall_RunTime = 0.0f;
#pragma endregion �ӵ�, �ð� ���� ��ü���� ����

	CStateMachine::Initialize();
}

_int CAmbor_StateMachine::PreUpdate(const _float& _fTimeDelta)
{
	return CStateMachine::PreUpdate(_fTimeDelta);
}

_int CAmbor_StateMachine::Update(const _float& _fTimeDelta)
{
	// ManagementActorCoolTime(_fTimeDelta); -> UserInstance�� ����
	ManagementActorState(_fTimeDelta);

	return CStateMachine::Update(_fTimeDelta);
}

_int CAmbor_StateMachine::LateUpdate(const _float& _fTimeDelta)
{
	return CStateMachine::LateUpdate(_fTimeDelta);
}

void CAmbor_StateMachine::Render()
{
	CStateMachine::Render();
}

void CAmbor_StateMachine::EndPlay()
{
	CStateMachine::EndPlay();
}

void CAmbor_StateMachine::Release()
{
	if (!m_wpWeaponCollider.expired())
		m_wpWeaponCollider.reset();

	if (!m_wpSkillCollider.expired())
		m_wpSkillCollider.reset();

	CStateMachine::Release();
}

void CAmbor_StateMachine::ManagementActorState(const _float& _fTimeDelta)
{
#pragma region �ִϸ��̼� ���� ó��
	_uint iCurAnim = m_wpAnimModel.lock()->GetAnimationIndex();
	if (m_wpAnimModel.lock()->GetAnimVector()[iCurAnim]->GetIsFinished() && m_wpAnimModel.lock()->GetIsLoop(iCurAnim))
	{
		RequestChangeState(PLAYER_STATE::STANDBY);
		m_wpAnimModel.lock()->GetAnimVector()[iCurAnim]->SetIsFinished(false);
	}
#pragma endregion �ִϸ��̼� ���� ó��

#pragma region �÷��̾� ����
	// �÷��̾� ��ġ, ���⺤��
	shared_ptr<CPlayer> spPlayer = dynamic_pointer_cast<CPlayer>(m_wpOwnerActor.lock());
	_vector vActorPos = spPlayer->GetActorTranslation();

	_vector vActorRight = spPlayer->GetRightVector();
	_vector vActorUp = spPlayer->GetUpVector();
	_vector vActorLook = spPlayer->GetLookVector();

	// �÷��̾� �׺� �� �ε���
	_int& iCurCellIndex = spPlayer->GetCurNaviCellIndex();

	// �� ���Ϳ��� Y���� ������ ����
	_vector vActorLookRemoveY = vActorLook - XMVectorSet(0.0f, XMVectorGetY(vActorLook), 0.0f, 0.0f);
	vActorLookRemoveY = XMVector4Normalize(vActorLookRemoveY);
#pragma endregion �÷��̾� ����

#pragma region ȸ��

	_float fAngle = CalculateActorRotation();

	if (!(m_iCurrState & PLAYER_STATE::IS_ATTACK) || (m_iCurrState & PLAYER_STATE::FLY_START))
	{
		switch (m_eDirrection)
		{
		case REQUEST_DIR::FOWARD: break;
		case REQUEST_DIR::FOWARD_LEFT:	fAngle += 315.0f; break;
		case REQUEST_DIR::LEFT:			fAngle += 270.0f; break;
		case REQUEST_DIR::BACK_LEFT:	fAngle += 225.0f; break;
		case REQUEST_DIR::BACK:			fAngle += 180.0f; break;
		case REQUEST_DIR::BACK_RIGHT:	fAngle += 135.0f; break;
		case REQUEST_DIR::RIGHT:		fAngle += 90.0f; break;
		case REQUEST_DIR::FOWARD_RIGHT:	fAngle += 45.0f; break;
		default: break; /* DIR_NONE */
		}

		if (abs(fAngle - m_fPrevAngle) > 180.0f)
			fAngle = fAngle - 360.0f;

		fAngle = lerp(m_fPrevAngle, fAngle, 0.5f);
	}
	else if (m_iCurrState & PLAYER_STATE::IS_FLYING)
	{
		fAngle = lerp(m_fPrevAngle, fAngle, 0.5f);
	}

#pragma endregion ȸ��

	// ���
	/*if (m_iCurrState & IS_DIE)
	{

	}*/

	// �ǰ�
	if (m_iCurrState & IS_HIT)
	{
#pragma region [ HIT ]
		if (m_iCurrState & PLAYER_STATE::HIT_H)
		{
			m_fHitH_RunTime += _fTimeDelta;
			if (m_fHitH_RunTime >= m_fHitH_MinTime)
				RequestChangeState(PLAYER_STATE::STANDBY);
		}
		else if (m_iCurrState & PLAYER_STATE::HIT_L)
		{
			m_fHitL_RunTime += _fTimeDelta;
			if (m_fHitL_RunTime >= m_fHitL_MinTime)
				RequestChangeState(PLAYER_STATE::STANDBY);
		}
		else if (m_iCurrState & PLAYER_STATE::HIT_THROW)
		{
			m_fHitThrow_RunTime += _fTimeDelta;
			if (m_fHitThrow_RunTime >= m_fHitThrow_MinTime)
				RequestChangeState(PLAYER_STATE::STANDBY);
		}
		else if (m_iCurrState & PLAYER_STATE::HIT_FALLTOGROUND)
		{
			spPlayer->SetActorTranslation(vActorPos - (vActorLookRemoveY * m_fRun_Speed *
				((m_fHitFall_MinTime - m_fHitFall_RunTime) / m_fHitFall_MinTime) * _fTimeDelta), &iCurCellIndex);

			m_fHitFall_RunTime += _fTimeDelta;
			if (m_fHitFall_RunTime >= m_fHitFall_MinTime)
				RequestChangeState(PLAYER_STATE::STANDBY);
		}
#pragma endregion [ HIT ]
	}

	// �̵�
	else if (m_iCurrState & IS_MOVE)
	{
		// ȸ��
		if (!(m_iCurrState & PLAYER_STATE::WALK_STOP || m_iCurrState & PLAYER_STATE::RUN_STOP || m_iCurrState & PLAYER_STATE::SPRINT_STOP))
		{
			spPlayer->SetRotation(XMVectorSet(0.0f, fAngle, 0.0f, 0.0f));

			m_fPrevAngle = fAngle;
		}
		else if (m_iCurrState & PLAYER_STATE::FLY_TURN_L || m_iCurrState & PLAYER_STATE::FLY_TURN_R)
		{
			spPlayer->SetRotation(XMVectorSet(0.0f, fAngle / 50.0f, 0.0f, 0.0f));

			m_fPrevAngle = fAngle;
		}

#pragma region [ WALK ]
		if (m_iCurrState & IS_WALK)
		{
			if (m_iCurrState == PLAYER_STATE::WALK)
				spPlayer->SetActorTranslation(vActorPos + (vActorLookRemoveY * m_fWalk_Speed * _fTimeDelta), &iCurCellIndex);
		}
#pragma endregion [ WALK ]
#pragma region [ RUN ]
		if (m_iCurrState & IS_RUN)
		{
			if (m_iCurrState & PLAYER_STATE::RUN_START)
			{
				m_fRun_Start_RunTime += _fTimeDelta;
				if (m_fRun_Start_RunTime >= m_fRun_Start_MinTime)
					RequestChangeState(PLAYER_STATE::RUN_CYCLE);
			}
			else if (m_iCurrState & PLAYER_STATE::RUN_CYCLE)
			{
				m_fRunTimeAcc += _fTimeDelta;
				if (0.335f < m_fRunTimeAcc)
					PlayRunSound_FootSteps();

				spPlayer->SetActorTranslation(vActorPos + (vActorLookRemoveY * m_fRun_Speed * _fTimeDelta), &iCurCellIndex);
			}
			else if (m_iCurrState & PLAYER_STATE::RUN_STOP)
			{
				m_fRunTimeAcc += _fTimeDelta;
				if (0.2f < m_fRunTimeAcc)
					PlayRunSound_FootSteps();

				m_fRun_Stop_RunTime += _fTimeDelta;
				if (m_fRun_Stop_RunTime >= m_fRun_Stop_MinTime)
					RequestChangeState(PLAYER_STATE::STANDBY);
			}
		}
#pragma endregion [ RUN ]
#pragma region [ SPRINT ]
		if (m_iCurrState & IS_SPRINT)
		{
			if (m_iCurrState & PLAYER_STATE::SPRINT_START)
			{
				m_fSprint_Start_RunTime += _fTimeDelta;

				if (0.5f > m_fSprint_Start_RunTime)
				{
					spPlayer->SetActorTranslation(vActorPos + (vActorLookRemoveY * 1.5f * m_fSprint_Speed *
						((m_fSprint_Start_MinTime - m_fSprint_Start_RunTime) / m_fSprint_Start_MinTime) * _fTimeDelta), &iCurCellIndex);
				}
				else
				{
					spPlayer->SetActorTranslation(vActorPos + (vActorLookRemoveY * m_fSprint_Speed * _fTimeDelta), &iCurCellIndex);
				}

				if (m_fSprint_Start_RunTime >= m_fSprint_Start_MinTime)
					RequestChangeState(PLAYER_STATE::SPRINT_CYCLE);
			}
			else if (m_iCurrState & PLAYER_STATE::SPRINT_CYCLE)
			{
				m_fRunTimeAcc += _fTimeDelta;
				if (0.22f < m_fRunTimeAcc)
					PlayRunSound_FootSteps();

				spPlayer->SetActorTranslation(vActorPos + (vActorLookRemoveY * m_fSprint_Speed * _fTimeDelta), &iCurCellIndex);
			}
			else if (m_iCurrState & PLAYER_STATE::SPRINT_STOP)
			{
				m_fRunTimeAcc += _fTimeDelta;
				if (0.3f < m_fRunTimeAcc)
					PlayRunSound_FootSteps();

				m_fSprint_Stop_RunTime += _fTimeDelta;
				spPlayer->SetActorTranslation(vActorPos + (vActorLookRemoveY * m_fSprint_Speed *
					((m_fSprint_Stop_MinTime - m_fSprint_Stop_RunTime) / m_fSprint_Stop_MinTime) * _fTimeDelta), &iCurCellIndex);

				if (m_fSprint_Stop_RunTime >= m_fSprint_Stop_MinTime)
					RequestChangeState(PLAYER_STATE::STANDBY);
			}
		}
#pragma endregion [ SPRINT ]
#pragma region [ JUMP ]
		if (m_iCurrState & IS_JUMP)
		{
			m_fJump_RunTime += _fTimeDelta;

			if (m_fJump_RunTime >= m_fJump_MinTime)
				m_fJump_RunTime = m_fJump_MinTime;

			_float fTotal = 100.f * powf(m_fJump_RunTime / 2.f, 2);
			if (8.f < fTotal)
				fTotal = 8.f;

			m_fJumpYAcc -= fTotal;

			if (m_iCurrState & PLAYER_STATE::JUMP)
			{
				spPlayer->SetActorTranslation(XMVectorSetY(vActorPos, XMVectorGetY(vActorPos) + m_fJump_Power * m_fJump_RunTime + m_fJumpYAcc), &iCurCellIndex);
			}
			else if (m_iCurrState & PLAYER_STATE::JUMP_FOR_WALK)
			{
				spPlayer->SetActorTranslation(XMVectorSetY(vActorPos, XMVectorGetY(vActorPos) + m_fJump_Power * m_fJump_RunTime + m_fJumpYAcc) +
					(vActorLookRemoveY * m_fWalk_Speed * _fTimeDelta), &iCurCellIndex);
			}
			else if (m_iCurrState & PLAYER_STATE::JUMP_FOR_RUN)
			{
				spPlayer->SetActorTranslation(XMVectorSetY(vActorPos, XMVectorGetY(vActorPos) + m_fJump_Power * m_fJump_RunTime + m_fJumpYAcc) +
					(vActorLookRemoveY * m_fRun_Speed * _fTimeDelta), &iCurCellIndex);
			}
			else if (m_iCurrState & PLAYER_STATE::JUMP_FOR_SPRINT)
			{
				m_fSprint_Start_RunTime += _fTimeDelta;

				spPlayer->SetActorTranslation(XMVectorSetY(vActorPos, XMVectorGetY(vActorPos) + m_fJump_Power * m_fJump_RunTime + m_fJumpYAcc) +
					(vActorLookRemoveY * m_fSprint_Speed * _fTimeDelta), &iCurCellIndex);
			}

			// �ٴڿ� ������� ������ �����Ѵ�.
			if (IsLand() && XMVectorGetY(vActorPos) <= IsLand())
				RequestChangeState(PLAYER_STATE::STANDBY);
		}
#pragma endregion [ JUMP ]
#pragma region [ FLYING ]
		if (m_iCurrState & IS_FLYING)
		{
			_float fFlyingY = XMVectorGetY(vActorPos) - 100.0f * _fTimeDelta;

			if (m_iCurrState & PLAYER_STATE::FLY_START)
			{
				m_fFlyStart_RunTime += _fTimeDelta;
				if (m_fFlyStart_RunTime >= m_fFlyStart_MinTime)
					RequestChangeState(PLAYER_STATE::FLY_NORMAL);

				spPlayer->SetActorTranslation(XMVectorSetY(vActorPos + (vActorLookRemoveY * 0.75f * m_fRun_Speed * _fTimeDelta), fFlyingY), &iCurCellIndex);
			}
			else if (m_iCurrState & PLAYER_STATE::FLY_NORMAL)
			{
				spPlayer->SetActorTranslation(XMVectorSetY(vActorPos + (vActorLookRemoveY * m_fRun_Speed * _fTimeDelta), fFlyingY), &iCurCellIndex);
			}
			else if (m_iCurrState & PLAYER_STATE::FLY_TURN_L)
			{
				spPlayer->SetActorTranslation(XMVectorSetY(vActorPos + (vActorLookRemoveY * m_fRun_Speed * _fTimeDelta), fFlyingY), &iCurCellIndex);
			}
			else if (m_iCurrState & PLAYER_STATE::FLY_TURN_R)
			{
				spPlayer->SetActorTranslation(XMVectorSetY(vActorPos + (vActorLookRemoveY * m_fRun_Speed * _fTimeDelta), fFlyingY), &iCurCellIndex);
			}

			// �ٴڿ� ������� Ȱ���� �����Ѵ�.
			if (IsLand() && XMVectorGetY(vActorPos) <= IsLand())
				RequestChangeState(PLAYER_STATE::STANDBY);
		}
#pragma endregion [ FLYING ]
	}

	// ����
	if (m_iCurrState & IS_ATTACK)
	{
#pragma region [ NORMAL ATTACK ]
		if (m_iCurrState & IS_NORMALATTACK)
		{
			if (m_iCurrState & PLAYER_STATE::ATTACK_1)
			{
				if (m_fAttack_RunTime < 0.2f)
					spPlayer->SetActorTranslation(vActorPos - (vActorLookRemoveY * m_fAttackMove_Speed * (m_fAttack_RunTime - _fTimeDelta)), &iCurCellIndex);

				if (0.2f < m_fAttack_RunTime && !m_bAttack1_One_Completed)
				{
					m_bAttack1_One_Completed = true;
					PlayNormalAttackVoice();
					
				}
			}
			else if (m_iCurrState & PLAYER_STATE::ATTACK_2)
			{
				if (m_fAttack_RunTime < 0.2f)
					spPlayer->SetActorTranslation(vActorPos - (vActorLookRemoveY * m_fAttackMove_Speed * (m_fAttack_RunTime - _fTimeDelta)), &iCurCellIndex);

				if (0.2f < m_fAttack_RunTime && !m_bAttack2_One_Completed)
				{
					m_bAttack2_One_Completed = true;
					PlayNormalAttackVoice();
					
				}
			}
			else if (m_iCurrState & PLAYER_STATE::ATTACK_3)
			{
				if (m_fAttack_RunTime < 0.3f)
					spPlayer->SetActorTranslation(vActorPos - (vActorLookRemoveY * 2.f * m_fAttackMove_Speed * (m_fAttack_RunTime - _fTimeDelta)), &iCurCellIndex);

				if (0.2f < m_fAttack_RunTime && !m_bAttack3_One_Completed)
				{
					m_bAttack3_One_Completed = true;
					PlayNormalAttackVoice();
					
				}
			}
			else if (m_iCurrState & PLAYER_STATE::ATTACK_4)
			{
				if (m_fAttack_RunTime < 0.4f)
					spPlayer->SetActorTranslation(vActorPos - (vActorLookRemoveY * m_fAttackMove_Speed * (m_fAttack_RunTime - _fTimeDelta)), &iCurCellIndex);

				if (0.2f < m_fAttack_RunTime && !m_bAttack4_One_Completed)
				{
					m_bAttack4_One_Completed = true;
					PlayNormalAttackVoice();
					
				}
			}
			else if (m_iCurrState & PLAYER_STATE::ATTACK_5)
			{
				if (m_fAttack_RunTime < 0.2f)
					spPlayer->SetActorTranslation(vActorPos - (vActorLookRemoveY * 2.f * m_fAttackMove_Speed * (m_fAttack_RunTime - _fTimeDelta)), &iCurCellIndex);

				if (0.4f < m_fAttack_RunTime && !m_bAttack5_One_Completed)
				{
					m_bAttack5_One_Completed = true;
					PlayHeavyAttackVoice();
					
				}
			}
		}
#pragma endregion [ NORMAL ATTACK ]
#pragma region [ SKILL ]
		else if (m_iCurrState & IS_SKILL)
		{
			if (m_iCurrState & PLAYER_STATE::ELEMENTAL_CLICK)
			{
				if (m_fElemental_Skill_MinTime < m_fElemental_Skill_RunTime)
					RequestChangeState(PLAYER_STATE::STANDBY);
			}
			else if (m_iCurrState & PLAYER_STATE::ELEMENTAL_CHARGE)
			{
				if (m_fElemental_Skill_MinTime < m_fElemental_Skill_RunTime)
					RequestChangeState(PLAYER_STATE::STANDBY);
			}
			else if (m_iCurrState & PLAYER_STATE::ELEMENTAL_BURST)
			{
				if (m_fElemental_Burst_MinTime < m_fElemental_Burst_RunTime)
					RequestChangeState(PLAYER_STATE::STANDBY);
			}
		}
	}
#pragma endregion [ SKILL ]
}

void CAmbor_StateMachine::ManagementActorCoolTime(const _float& _fTimeDelta)
{
	/* == ��Ÿ == */
	m_fAttack_RunTime += _fTimeDelta;
	if (m_fAttack_RunTime >= m_fAttack_MinTime)
		m_fAttack_RunTime = m_fAttack_MinTime;

	/* == ��ų == */
	m_fElemental_Skill_RunTime += _fTimeDelta;
	if (m_fElemental_Skill_RunTime >= m_fElemental_Skill_CoolTime)
		m_fElemental_Skill_RunTime = m_fElemental_Skill_CoolTime;

	m_fElemental_Burst_RunTime += _fTimeDelta;
	if (m_fElemental_Burst_RunTime >= m_fElemental_Burst_CoolTime)
		m_fElemental_Burst_RunTime = m_fElemental_Burst_CoolTime;

	/* == ��ų == */
	m_fBuff_E_TimeAcc += _fTimeDelta;
	if (m_fBuff_E_TimeAcc > m_fBuff_E_Duration)
	{
		m_fBuff_E_TimeAcc = 0.f;
		m_bBuffed_E = false;
	}
}

void CAmbor_StateMachine::RequestChangeState(_ullong _iPlayerStateFlag, REQUEST_DIR _eDirection, ATTACK_DESC _AttackDesc)
{
	// ��û���� ���� ��ȭ�� ������ ���� ���¿� ���ٸ� return, ������ �ǰ������̶�� ���
	if (_iPlayerStateFlag == m_iCurrState && _eDirection == m_eDirrection 
		&& !(_iPlayerStateFlag & IS_HIT))
		return;

	// ��û���� ������ ���⼺
	if (_eDirection != REQUEST_DIR::DIR_NONE)
		m_eDirrection = _eDirection;

#pragma region �÷��̾� ����
	shared_ptr<CPlayer> spPlayer = dynamic_pointer_cast<CPlayer>(m_wpOwnerActor.lock());
	_int& iCurCellIndex = spPlayer->GetCurNaviCellIndex();

	_vector vPlayerPos = spPlayer->GetActorTranslation();
	_vector vPlayerRight = XMVector3Normalize(spPlayer->GetRightVector());
	_vector vPlayerUp = XMVector3Normalize(spPlayer->GetUpVector());
	_vector vPlayerLook = XMVector3Normalize(spPlayer->GetLookVector());
#pragma endregion �÷��̾� ����

	///////////////////////////////////////////
	// �� ��û���� ���� ��ȭ�� ������ üũ ��//
	///////////////////////////////////////////

	// 1. ��û���� ���°� DIE��� -> �ֿ켱������ ���� ���¸� ó���Ѵ�.
	// if ()
	// {
		  /*m_wpAnimModel.lock()->SetAnimationIndex(ANIM::DIE);
		  m_iCurrState = _iPlayerStateFlag;*/
	// }
	// 2. ��û���� ���°� HIT��� -> HIT_TYPE�� �Ǵ��Ͽ� ���������� ó���Ѵ�.
#pragma region [ HIT ]
	if (_iPlayerStateFlag & IS_HIT)
	{
		// 2.2 ���� ���� ��� ���̰ų� �뽬 ���� ����̸� �����̴�.
		if (m_iCurrState & PLAYER_STATE::ELEMENTAL_BURST || m_iCurrState & PLAYER_STATE::SPRINT_START)
			return;

		// 2.3 ������ �޾Ҵ� ������ ������, ���� �̸��� ���Ͽ� ������ return;
		if (m_HittedAttack_Desc.wstrOwnerName == _AttackDesc.wstrOwnerName &&
			m_HittedAttack_Desc.wstrAttackName == _AttackDesc.wstrAttackName &&
			false == _AttackDesc.bMultiHit)
			return;

		// 2.4 ���� ��� ���� ���̿���, ���� ������ �ƴ϶�� �ǰ� ����� ��ŵ�ϰ� �������� ������ �Ѵ�.
		if (m_iCurrState & IS_ATTACK && KNOCKBACK_TYPE::POWERFUL != _AttackDesc.eKnockbackType)
		{
			Hitted(_AttackDesc, _AttackDesc.eDamage);
			MakeHitEffect(XMVectorSet(0.0f, 100.f, 0.0f, 0.0f));

			// ���� ATTACK_DESC�� ����Ѵ�.
			m_HittedAttack_Desc = _AttackDesc;

			return;
		}

		if (_iPlayerStateFlag & PLAYER_STATE::HIT_H)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::HIT_H);
			m_iCurrState = PLAYER_STATE::HIT_H;
			m_fHitH_RunTime = 0.0f;
		}
		else if (_iPlayerStateFlag & PLAYER_STATE::HIT_L)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::HIT_L);
			m_iCurrState = PLAYER_STATE::HIT_L;
			m_fHitL_RunTime = 0.0f;
		}
		else if (_iPlayerStateFlag & PLAYER_STATE::HIT_THROW)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::HIT_THROW);
			m_iCurrState = PLAYER_STATE::HIT_THROW;
			m_fHitThrow_RunTime = 0.0f;
		}
		else if (_iPlayerStateFlag & PLAYER_STATE::HIT_FALLTOGROUND)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::HIT_FALLTOGROUND);
			m_iCurrState = PLAYER_STATE::HIT_FALLTOGROUND;
			m_fHitFall_RunTime = 0.0f;
		}

		PlayHitVoice(_AttackDesc);

		// �ǰ� ����Ʈ ����
		Hitted(_AttackDesc, _AttackDesc.eDamage);
		if(!m_wpOwnerActor.expired())
			MakeHitEffect(XMVectorSet(0.0f, 100.f, 0.0f, 0.0f));

		// ���� ATTACK_DESC�� ����Ѵ�.
		m_HittedAttack_Desc = _AttackDesc;
	}
#pragma endregion [ HIT ]
	// 3. ��û���� ���°� �����ӵ��� �� �ϳ����
	else if (_iPlayerStateFlag & IS_MOVE)
	{
		// 1. DIE�� HIT �߿��� ������ �� ����.
		if (/*m_iCurrState & IS_DIE || */m_iCurrState & IS_HIT || m_iCurrState & PLAYER_STATE::ELEMENTAL_BURST)
			return;

		// 2. �ൿ���� RunTime�� MinTime�� �Ѿ�� �Ѵ�.
		if (m_fAttack_ExitTime > m_fAttack_RunTime ||
			m_fElemental_Skill_MinTime > m_fElemental_Skill_RunTime ||
			m_fElemental_Burst_MinTime > m_fElemental_Burst_RunTime)
			return;

		// 3. �׷��� �ʴٸ� ���� ��ȭ�� ����Ѵ�.

#pragma region [ WALK ]
		if (_iPlayerStateFlag & PLAYER_STATE::WALK)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::WALK);
			m_iCurrState = PLAYER_STATE::WALK;
		}
#pragma endregion [ WALK ]	
#pragma region [ RUN ]
		if (_iPlayerStateFlag & IS_RUN)
		{
			if (_iPlayerStateFlag & PLAYER_STATE::RUN_CYCLE)
			{
				if (m_iCurrState == PLAYER_STATE::STANDBY || m_iCurrState & PLAYER_STATE::WALK || m_iCurrState & IS_ATTACK)
				{
					m_wpAnimModel.lock()->SetAnimationIndex(ANIM::RUN_START);
					m_iCurrState = PLAYER_STATE::RUN_START;
				}
				else if (m_iCurrState & PLAYER_STATE::RUN_START)
				{
					m_wpAnimModel.lock()->SetAnimationIndex(ANIM::RUN_CYCLE);
					m_iCurrState = PLAYER_STATE::RUN_CYCLE;
					m_fRun_Start_RunTime = 0.0f;
				}
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::RUN_DOWNHILL)
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::RUN_DOWNHILL);
			else if (_iPlayerStateFlag & PLAYER_STATE::RUN_TO_WALK)
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::RUN_TO_WALK);
		}
#pragma endregion [ RUN ]
#pragma region [ SPRINT ]
		if (_iPlayerStateFlag & IS_SPRINT)
		{
			if (_iPlayerStateFlag & PLAYER_STATE::SPRINT_CYCLE)
			{
				if (m_iCurrState & IS_WALK || m_iCurrState & IS_RUN || m_iCurrState & PLAYER_STATE::SPRINT_CYCLE)
				{
					m_wpAnimModel.lock()->SetAnimationIndex(ANIM::SPRINT_START);
					m_iCurrState = PLAYER_STATE::SPRINT_START;

					PlaySprintVoice();
				}
				else if (m_iCurrState & PLAYER_STATE::SPRINT_START/* && m_fSprint_Start_RunTime >= m_fSprint_Start_MinTime*/)
				{
					m_wpAnimModel.lock()->SetAnimationIndex(ANIM::SPRINT_CYCLE);
					m_iCurrState = PLAYER_STATE::SPRINT_CYCLE;
					m_fSprint_Start_RunTime = 0.0f;
				}
			}
			if (_iPlayerStateFlag & PLAYER_STATE::SPRINT_STOP)
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::SPRINT_STOP);
			if (_iPlayerStateFlag & PLAYER_STATE::SPRINT_TO_RUN)
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::SPRINT_TO_RUN);
		}
#pragma endregion [ SPRINT ]
#pragma region [ JUMP ]
		if (_iPlayerStateFlag & IS_JUMP)
		{
			if (_iPlayerStateFlag & PLAYER_STATE::JUMP)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::JUMP);
				m_iCurrState = PLAYER_STATE::JUMP;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::JUMP_FOR_WALK)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::JUMP_FOR_WALK);
				m_iCurrState = PLAYER_STATE::JUMP_FOR_WALK;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::JUMP_FOR_RUN)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::JUMP_FOR_RUN);
				m_iCurrState = PLAYER_STATE::JUMP_FOR_RUN;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::JUMP_FOR_SPRINT)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::JUMP_FOR_SPRINT);
				m_iCurrState = PLAYER_STATE::JUMP_FOR_SPRINT;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::JUMP_UPSTAIRS_H)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::JUMP_UPSTAIRS_H);
				m_iCurrState = PLAYER_STATE::JUMP_UPSTAIRS_H;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::JUMP_UPSTAIRS_L)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::JUMP_UPSTAIRS_L);
				m_iCurrState = PLAYER_STATE::JUMP_UPSTAIRS_L;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::JUMP_OFF_WALL)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::JUMP_OFF_WALL);
				m_iCurrState = PLAYER_STATE::JUMP_OFF_WALL;
			}

			PlayJumpVoice();

			m_fJump_RunTime = 0.0f;
			m_fJumpYAcc = 0.0f;
		}
#pragma endregion [ JUMP ]
#pragma region [ FALL_TO_GROUND ]
		else if (_iPlayerStateFlag & PLAYER_STATE::FALL_TO_GROUND_L)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::FALL_TO_GROUND_L);
		else if (_iPlayerStateFlag & PLAYER_STATE::FALL_TO_GROUND_H)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::FALL_TO_GROUND_H);
		else if (_iPlayerStateFlag & PLAYER_STATE::FALL_TO_GROUND_BSH)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::FALL_TO_GROUND_BSH);
#pragma endregion [ FALL_TO_GROUND ]
#pragma region [ CLIMB ]
		else if (_iPlayerStateFlag & PLAYER_STATE::CLIMB_IDLE_D)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::CLIMB_IDLE_D);
		else if (_iPlayerStateFlag & PLAYER_STATE::CLIMB_L)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::CLIMB_L);
		else if (_iPlayerStateFlag & PLAYER_STATE::CLIMB_U)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::CLIMB_U);
		else if (_iPlayerStateFlag & PLAYER_STATE::CLIMB_R)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::CLIMB_R);
		else if (_iPlayerStateFlag & PLAYER_STATE::CLIMB_D)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::CLIMB_D);

		else if (_iPlayerStateFlag & PLAYER_STATE::CLIMB_DASH_L)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::CLIMB_DASH_L);
		else if (_iPlayerStateFlag & PLAYER_STATE::CLIMB_DASH_LU)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::CLIMB_DASH_LU);
		else if (_iPlayerStateFlag & PLAYER_STATE::CLIMB_DASH_U)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::CLIMB_DASH_U);
		else if (_iPlayerStateFlag & PLAYER_STATE::CLIMB_DASH_RU)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::CLIMB_DASH_RU);
		else if (_iPlayerStateFlag & PLAYER_STATE::CLIMB_DASH_R)
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::CLIMB_DASH_R);
#pragma endregion [ CLIMB ]
#pragma region [ FLYING ]
		if (_iPlayerStateFlag & IS_FLYING)
		{
			if (_iPlayerStateFlag & PLAYER_STATE::FLY_START)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::FLY_START);
				m_iCurrState = PLAYER_STATE::FLY_START;

				PlayFlyingStartSound();
				CSoundManager::GetInstance()->PlaySoundEx("Ambor_FlyStart.ogg", SOUND_TYPE::VOICE, 0.1f);
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::FLY_NORMAL)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::FLY_NORMAL);
				m_iCurrState = PLAYER_STATE::FLY_NORMAL;
				m_fFlyStart_RunTime = 0.0f;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::FLY_TURN_L)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::FLY_TURN_L);
				m_iCurrState = PLAYER_STATE::FLY_TURN_L;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::FLY_TURN_R)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::FLY_TURN_R);
				m_iCurrState = PLAYER_STATE::FLY_TURN_R;
			}
		}
#pragma endregion [ FLYING ]
	}
	// 4. ��û���� ���°� ���� ���ݵ��� �� �ϳ����
	else if (_iPlayerStateFlag & IS_ATTACK)
	{
		// 1. (�ӽ�)�����߿��� ������ �� ����.
		if (m_iCurrState & PLAYER_STATE::IS_FLYING)
			return;

		// 2. �������� �����߿��� ������ �� ����.
		if (m_iCurrState & PLAYER_STATE::ELEMENTAL_BURST)
			return;

		// 3. �׷��� �ʴٸ� ���� ��ȭ�� ����Ѵ�.
		// 4. ���� � ���¿����Ŀ� ���� ���ư� �ൿ�� �����Ѵ�.

#pragma region [ NORMAL ATTACK]
		if (_iPlayerStateFlag & IS_NORMALATTACK && m_fAttack_ExitTime <= m_fAttack_RunTime)
		{
			AutoTargetting();

			ATTACK_DESC AttackDesc = {};

			if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_1)
			{
				AttackDesc = m_Attack1_Desc;
				AttackDesc.wstrAttackName = L"Player_Ambor Attack1" + to_wstring(rand());

				CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack_1.mp3", EFFECT_9, 0.5f);
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_2)
			{
				AttackDesc = m_Attack2_Desc;
				AttackDesc.wstrAttackName = L"Player_Ambor Attack2" + to_wstring(rand());

				CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack_2.mp3", EFFECT_9, 0.5f);
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_3)
			{
				AttackDesc = m_Attack3_Desc;
				AttackDesc.wstrAttackName = L"Player_Ambor Attack3" + to_wstring(rand());

				CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack_3.mp3", EFFECT_9, 0.5f);
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_4)
			{
				AttackDesc = m_Attack4_Desc;
				AttackDesc.wstrAttackName = L"Player_Ambor Attack4" + to_wstring(rand());

				CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack_4.mp3", EFFECT_9, 0.5f);
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_5)
			{
				AttackDesc = m_Attack5_Desc;
				AttackDesc.wstrAttackName = L"Player_Ambor Attack5" + to_wstring(rand());

				CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack_5.mp3", EFFECT_9, 0.5f);
			}

			if (m_bBuffed_E)
			{
				AttackDesc.eElementalType = PYRO;
				AttackDesc.eDamage = AttackDesc.eDamage * 2.f;

				MakeArrow(AttackDesc, L"Ambor_NormalAtk_Fire");
				MakeParticle(L"Particle_Ambor_NormalAttack_Two", vPlayerLook * 50.f + XMVectorSet(0.0f, 125.f, 0.0f, 0.0f));
				MakeParticle(L"Sprite_Ambor_NormalAttack_Fire", vPlayerLook * 50.f + XMVectorSet(0.0f, 125.f, 0.0f, 0.0f));
			}
			else
			{
				AttackDesc.eElementalType = NONE_ELEMENTAL;

				MakeArrow(AttackDesc, L"Ambor_NormalAtk_One");
				MakeParticle(L"Particle_Ambor_NormalAttack_One", vPlayerLook * 50.f + XMVectorSet(0.0f, 125.f, 0.0f, 0.0f));
				MakeParticle(L"Sprite_Ambor_AmosAttack", vPlayerLook * 50.f + XMVectorSet(0.0f, 125.f, 0.0f, 0.0f));
			}

			if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_1)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ATTACK_1);
				m_iCurrState = PLAYER_STATE::ATTACK_1;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_2)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ATTACK_2);
				m_iCurrState = PLAYER_STATE::ATTACK_2;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_3)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ATTACK_3);
				m_iCurrState = PLAYER_STATE::ATTACK_3;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_4)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ATTACK_4);
				m_iCurrState = PLAYER_STATE::ATTACK_4;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_5)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ATTACK_5);
				m_iCurrState = PLAYER_STATE::ATTACK_5;
			}

			m_bAttack1_One_Completed = false;
			m_bAttack1_Two_Completed = false;
			m_bAttack2_One_Completed = false;
			m_bAttack2_Two_Completed = false;
			m_bAttack3_One_Completed = false;
			m_bAttack3_Two_Completed = false;
			m_bAttack4_One_Completed = false;
			m_bAttack4_Two_Completed = false;
			m_bAttack5_One_Completed = false;
			m_bAttack5_Two_Completed = false;

			m_fAttack_RunTime = 0.0f;
		}
#pragma endregion [ NORMAL ATTACK]
#pragma region [ SKILL ]
		else if ((_iPlayerStateFlag & PLAYER_STATE::ELEMENTAL_CLICK || _iPlayerStateFlag & PLAYER_STATE::ELEMENTAL_CHARGE) && m_fElemental_Skill_CoolTime <= m_fElemental_Skill_RunTime)
		{
			if (_iPlayerStateFlag & PLAYER_STATE::ELEMENTAL_CLICK)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ELEMENTAL_CLICK);
				m_iCurrState = PLAYER_STATE::ELEMENTAL_CLICK;

				MakeEffect(L"Ambor_Skill_E_Aura_One", XMVectorSet(0.f, 0.f, 0.f, 0.f));
				MakeEffect(L"Ambor_Skill_E_Aura_Two", XMVectorSet(0.f, 0.f, 0.f, 0.f));

				PlayElementalSkillVoice();
				CSoundManager::GetInstance()->PlaySoundEx("Ambor_ElementalSkill.mp3", EFFECT_9, 0.5f);

				m_fBuff_E_TimeAcc = 0.f;
				m_bBuffed_E = true;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ELEMENTAL_CHARGE)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ELEMENTAL_CHARGE);
				m_iCurrState = PLAYER_STATE::ELEMENTAL_CHARGE;
			}

			m_fElemental_Skill_RunTime = 0.0f;
		}
		else if ((_iPlayerStateFlag & PLAYER_STATE::ELEMENTAL_BURST) && m_fElemental_Burst_CoolTime <= m_fElemental_Burst_RunTime)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ELEMENTAL_BURST);
			m_iCurrState = PLAYER_STATE::ELEMENTAL_BURST;

			MakeEffect(L"Ambor_Skill_Q_Floor_Three", XMVectorSetY(-vPlayerPos + AutoTargetting(), 10.f));
			MakeParticle_Q(- vPlayerPos + AutoTargetting());

			shared_ptr<CPlayer> spPlayer = dynamic_pointer_cast<CPlayer>(m_wpOwnerActor.lock());
			_int& iCurCellIndex = spPlayer->GetCurNaviCellIndex();

			_vector vPlayerPos = spPlayer->GetActorTranslation();
			_vector vPlayerRight = XMVector3Normalize(spPlayer->GetRightVector());
			_vector vPlayerUp = XMVector3Normalize(spPlayer->GetUpVector());
			_vector vPlayerLook = XMVector3Normalize(spPlayer->GetLookVector());

			shared_ptr<CProductionCamera> spProductionCam = dynamic_pointer_cast<CProductionCamera>(CCameraManager::GetInstance()->GetCurCamera());

			_vector vStartCamPos = vPlayerPos + (vPlayerRight * 200.f) + (vPlayerUp * 200.f) + (vPlayerLook * 200.f);
			_vector vEndCamPos = vPlayerPos + (vPlayerRight * 75.f) + (vPlayerUp * 60.f) + (vPlayerLook * 75.f);

			_vector vStartCamAt = vPlayerPos;
			_vector vEndCamAt = vPlayerPos + (vPlayerUp * 50.f) + (vPlayerLook * 30.f);

			spProductionCam->BeginProduction(vStartCamPos, vEndCamPos, vStartCamAt, vEndCamAt, 0.8f);
			spProductionCam->SetProductionRatio(1.f, 3.5f, 3.5f);

			PlayElementalBurstVoice();
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_ElementalBurst.mp3", EFFECT_9, 0.5f);

			m_fElemental_Burst_RunTime = 0.0f;
		}
	}
#pragma endregion [SKILL]
#pragma region [ STANDBY ]
	else if (_iPlayerStateFlag == PLAYER_STATE::STANDBY)
	{
		// 5.1 STANDBY�� ���ư� �� �ִ� �������� üũ�Ѵ�.(IS_ATTACK, IS_JUMP, IS_FLYING)
		// if (m_iCurrState & IS_FLYING)
			// return;

		// 5.2 �׷��� �ʴٸ� ���º�ȭ�� ����Ѵ�.
		// 5.3 ���� � ���¿����Ŀ� ���� ���ư� �ൿ�� �����Ѵ�.
		_uint iCurAnim = m_wpAnimModel.lock()->GetAnimationIndex();

		/* [ HIT ] */
		if (m_iCurrState & IS_HIT)
		{
			if (m_iCurrState & PLAYER_STATE::HIT_H && m_fHitH_RunTime >= m_fHitH_MinTime)
			{
				// �ءء� ���߿� �̰� ����STANDBY�� �ٲ�� ��. �ءء�
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
				m_iCurrState = PLAYER_STATE::STANDBY;
				m_fHitH_RunTime = 0.0f;
			}
			else if (m_iCurrState & PLAYER_STATE::HIT_L && m_fHitL_RunTime >= m_fHitL_MinTime)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
				m_iCurrState = PLAYER_STATE::STANDBY;
				m_fHitL_RunTime = 0.0f;
			}
			else if (m_iCurrState & PLAYER_STATE::HIT_THROW && m_fHitThrow_RunTime >= m_fHitThrow_MinTime)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
				m_iCurrState = PLAYER_STATE::STANDBY;
				m_fHitThrow_RunTime = 0.0f;
			}
			else if (m_iCurrState & PLAYER_STATE::HIT_FALLTOGROUND && m_fHitFall_RunTime >= m_fHitFall_MinTime)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
				m_iCurrState = PLAYER_STATE::STANDBY;
				m_fHitFall_RunTime = 0.0f;
			}
		}
		/* [ WALK ] */
		else if (m_iCurrState & IS_WALK)
		{
			if (m_iCurrState & PLAYER_STATE::WALK)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::WALK_STOP);
				m_iCurrState = PLAYER_STATE::WALK_STOP;
				return;
			}
			if (m_iCurrState & PLAYER_STATE::WALK_STOP)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
				m_iCurrState = PLAYER_STATE::STANDBY;
				return;
			}
		}
		/* [ RUN ] */
		else if (m_iCurrState & IS_RUN)
		{
			if (m_iCurrState != PLAYER_STATE::RUN_STOP)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::RUN_STOP);
				m_iCurrState = PLAYER_STATE::RUN_STOP;
				return;
			}
			else if (m_iCurrState & PLAYER_STATE::RUN_STOP && m_fRun_Stop_RunTime >= m_fRun_Stop_MinTime)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
				m_iCurrState = PLAYER_STATE::STANDBY;
				m_fRun_Stop_RunTime = 0.0f;
				return;
			}
		}
		/* [ SPRINT ] */
		else if (m_iCurrState & IS_SPRINT)
		{
			if (m_iCurrState != PLAYER_STATE::SPRINT_STOP)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::SPRINT_STOP);
				m_iCurrState = PLAYER_STATE::SPRINT_STOP;
				return;
			}
			else if (m_iCurrState & PLAYER_STATE::SPRINT_STOP && m_fSprint_Stop_RunTime >= m_fSprint_Stop_MinTime)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
				m_iCurrState = PLAYER_STATE::STANDBY;
				m_fSprint_Stop_RunTime = 0.0f;
				return;
			}
		}
		/* [ JUMP ] */
		else if (m_iCurrState & IS_JUMP)
		{
			shared_ptr<CPlayer> spPlayer = dynamic_pointer_cast<CPlayer>(m_wpOwnerActor.lock());
			_vector vActorPos = spPlayer->GetActorTranslation();

			if (IsLand() && 0.2f <= m_fJump_RunTime)
			{
				PlayRunSound_FootSteps();

				if (m_iCurrState & PLAYER_STATE::JUMP_FOR_WALK)
				{
				}
				else if (m_iCurrState & PLAYER_STATE::JUMP_FOR_RUN)
					m_fRun_Start_RunTime = 0.0f;
				else if (m_iCurrState & PLAYER_STATE::JUMP_FOR_SPRINT)
					m_fSprint_Start_RunTime = 0.0f;

				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
				m_iCurrState = PLAYER_STATE::STANDBY;
				m_fJump_RunTime = 0.0f;
				spPlayer->SetActorTranslation(XMVectorSetY(vActorPos, IsLand()), &iCurCellIndex);
				return;
			}
			else if (!IsLand())
			{
				if (m_iCurrState & PLAYER_STATE::JUMP_FOR_WALK)
				{
					m_iCurrState = PLAYER_STATE::JUMP_FOR_WALK;
				}
				else if (m_iCurrState & PLAYER_STATE::JUMP_FOR_RUN)
				{
					m_iCurrState = PLAYER_STATE::JUMP_FOR_RUN;
				}
				else if (m_iCurrState & PLAYER_STATE::JUMP_FOR_SPRINT)
				{
					m_iCurrState = PLAYER_STATE::JUMP_FOR_SPRINT;
				}
				return;
			}
		}
		/* [ FLYING ] */
		else if (m_iCurrState & IS_FLYING)
		{
			shared_ptr<CPlayer> spPlayer = dynamic_pointer_cast<CPlayer>(m_wpOwnerActor.lock());
			_vector vActorPos = spPlayer->GetActorTranslation();

			if (IsLand())
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
				m_iCurrState = PLAYER_STATE::STANDBY;
				m_fFlyStart_RunTime = 0.0f;
				spPlayer->SetActorTranslation(XMVectorSetY(vActorPos, IsLand()), &iCurCellIndex);
				return;
			}
		}
		/* [ ATTACK ] */
		else if (m_iCurrState & IS_ATTACK && m_wpAnimModel.lock()->GetAnimVector()[iCurAnim]->GetIsFinished())
		{
			if (m_iCurrState & PLAYER_STATE::ELEMENTAL_BURST && m_fElemental_Burst_RunTime < m_fElemental_Burst_MinTime)
				return;

			if (m_iCurrState & PLAYER_STATE::ELEMENTAL_CLICK && m_fElemental_Skill_RunTime < m_fElemental_Skill_MinTime)
				return;

			if (m_iCurrState & PLAYER_STATE::ELEMENTAL_CHARGE && m_fElemental_Skill_RunTime < m_fElemental_Skill_MinTime)
				return;

			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
			m_iCurrState = PLAYER_STATE::STANDBY;

			m_bAttack1_One_Completed = false;
			m_bAttack1_Two_Completed = false;
			m_bAttack2_One_Completed = false;
			m_bAttack2_Two_Completed = false;
			m_bAttack3_One_Completed = false;
			m_bAttack3_Two_Completed = false;
			m_bAttack4_One_Completed = false;
			m_bAttack4_Two_Completed = false;
			m_bAttack5_One_Completed = false;
			m_bAttack5_Two_Completed = false;

			m_iProductionCount = 0;
		}
		//// �ǰ� �ݶ��̴��� OnCollisionEnter�� �ٽ� �Ҹ� �� �ְԲ�, �浹 ����Ʈ�� �ʱ�ȭ
		//dynamic_pointer_cast<CCharactor>(m_wpOwnerActor.lock())->GetCollider()->GetColliderComponents().clear();
	}
#pragma endregion [ STANDBY ]
}

_float CAmbor_StateMachine::CalculateArrowRotation()
{
	// ���� ���⺤�Ϳ��� y���� �����ϰ� ����ȭ
	_vector vActorLook = m_wpOwnerActor.lock()->GetLookVector();
	vActorLook = XMVector3Normalize(XMVectorSetY(vActorLook, 0.0f));

	// z�� ���⺤�Ϳ��� y���� �����ϰ� ����ȭ
	_vector vZ = XMVector3Normalize(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));

	// �����Ͽ� ���� ���� ����
	_float fAngle = acos(XMVector3Dot(vZ, vActorLook).m128_f32[0]) * (180.0f / DirectX::XM_PI);

	// Up ���Ϳ� vCamLook, vActorLook�� ������ ���͸� ��, ������ ���ٸ� ������ 180�� �����̴�.
	if (XMVectorGetY(XMVector3Cross(vActorLook, vZ)) > 0)
	{
		fAngle = 360.0f - fAngle;
	}

	return fAngle;
}
void CAmbor_StateMachine::MakeArrow(ATTACK_DESC& _AttackDesc, const wstring& _wstrEffectName)
{
	// _AttackDesc.wstrAttackName += to_wstring(rand() % 2);

	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->RecycleFromPool(_wstrEffectName);
	shared_ptr<CPlayerProjectileEffect> spPlayerProjectile;

	if (nullptr != spActor)
		spPlayerProjectile = dynamic_pointer_cast<CPlayerProjectileEffect>(spActor);

	if (nullptr != spPlayerProjectile)
	{
		_vector vOwnerPos = GetOwnerActor().lock()->GetActorTranslation();
		_vector vOwnerLook = XMVector3Normalize(XMVectorSetY(GetOwnerActor().lock()->GetLookVector(), 0.f));

		_float fAngle = CalculateArrowRotation();

		spPlayerProjectile->SetRotation(XMVectorSet(0.f, fAngle, 0.f, 0.f));
		spPlayerProjectile->SetActorTranslation_NoNavigation(vOwnerPos + XMVectorSet(0.f, 75.f, 0.f, 0.f));
		spPlayerProjectile->SetShootDirrection(vOwnerLook);
		spPlayerProjectile->SetShootSpeed(4000.f);
		spPlayerProjectile->SetAttackDesc(_AttackDesc);
	}
}

void CAmbor_StateMachine::MakeParticle_E(_vector _vOffset)
{

}
void CAmbor_StateMachine::MakeParticle_Q(_vector _vOffset)
{
	MakeParticle(L"Particle_Ambor_Skill_Q", _vOffset);
}

void CAmbor_StateMachine::PlayNormalAttackVoice()
{
	++m_iSoundCount_LightAttack;

	if (0 == m_iSoundCount_LightAttack % 2)
	{
		switch (m_iSoundCount_LightAttack % 7)
		{
		case 0:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack1.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 1:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack2.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 2:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack3.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 3:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack4.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 4:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack5.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 5:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack6.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 6:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_NormalAttack7.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		}
	}
}
void CAmbor_StateMachine::PlayHeavyAttackVoice()
{
	++m_iSoundCount_HeavyAttack;

	// ���� ���
	switch (m_iSoundCount_HeavyAttack % 3)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_HeavyAttack1.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_HeavyAttack2.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_HeavyAttack3.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	}
}
void CAmbor_StateMachine::PlayHitVoice(ATTACK_DESC _AttackDesc)
{
	// 4. �ǰ� ���� ���
	switch (_AttackDesc.eKnockbackType)
	{
	case KNOCKBACK_TYPE::WEAK:
	case KNOCKBACK_TYPE::STRONG:
	{
		switch (rand() % 6)
		{
		case 0:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_Hit_L1.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 1:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_Hit_L2.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 2:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_Hit_L3.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 3:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_Hit_L4.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		}
		break;
	}
	case KNOCKBACK_TYPE::POWERFUL:
	{
		switch (rand() % 6)
		{
		case 0:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_HeavyHit1.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 1:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_HeavyHit2.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 2:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_HeavyHit3.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 3:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_HeavyHit4.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 4:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_HeavyHit5.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 5:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Ambor_HeavyHit6.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		}
		break;
	}
	}
}
void CAmbor_StateMachine::PlaySprintVoice()
{
	++m_iSoundCount_SprintStart;

	// ��� ���� ����
	switch (m_iSoundCount_SprintStart % 5)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_SprintStart1.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_SprintStart2.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_SprintStart3.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 3:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_SprintStart4.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	case 4:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_SprintStart5.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	}
	}
}
void CAmbor_StateMachine::PlayJumpVoice()
{
	++m_iSoundCount_Jump;

	// ���� ����
	switch (m_iSoundCount_Jump % 5)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_Jump1.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_Jump2.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_Jump3.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 3:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_Jump4.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 4:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_Jump5.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	}
}

void CAmbor_StateMachine::PlayElementalSkillVoice()
{
	++m_iSoundCount_ElementalSkill;

	// ���� ���� ��ų ���� ���
	switch (m_iSoundCount_ElementalSkill % 3)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_Elemental_Click1.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_Elemental_Click2.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	}
}
void CAmbor_StateMachine::PlayElementalChargeVoice()
{

}
void CAmbor_StateMachine::PlayElementalBurstVoice()
{
	++m_iSoundCount_ElementalBurst;

	// ���� ���� ���� ���
	switch (m_iSoundCount_ElementalBurst % 3)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_Elemental_Burst1.ogg", SOUND_TYPE::VOICE, 0.6f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_Elemental_Burst2.ogg", SOUND_TYPE::VOICE, 0.6f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Ambor_Elemental_Burst3.ogg", SOUND_TYPE::VOICE, 0.6f);
		break;
	}
	}
}

void CAmbor_StateMachine::PlayRunSound_FootSteps()
{
	switch (m_iRunSoundCount % 8)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_455.ogg", SOUND_TYPE::EFFECT_2, 0.15f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_456.ogg", SOUND_TYPE::EFFECT_2, 0.15f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_457.ogg", SOUND_TYPE::EFFECT_2, 0.15f);
		break;
	}
	case 3:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_458.ogg", SOUND_TYPE::EFFECT_2, 0.15f);
		break;
	}
	case 4:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_459.ogg", SOUND_TYPE::EFFECT_2, 0.15f);
		break;
	}
	case 5:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_460.ogg", SOUND_TYPE::EFFECT_2, 0.15f);
		break;
	}
	case 6:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_461.ogg", SOUND_TYPE::EFFECT_2, 0.15f);
		break;
	}
	case 7:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_462.ogg", SOUND_TYPE::EFFECT_2, 0.15f);
		break;
	}
	}

	++m_iRunSoundCount;

	m_fRunTimeAcc = 0.f;
}
void CAmbor_StateMachine::PlaySprintSound_FootSteps()
{
}
