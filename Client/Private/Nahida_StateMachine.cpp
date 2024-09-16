#include "pch.h"
#include "Nahida_StateMachine.h"
#include "PlayerStateDefine.h"

#include "InputManager.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "NavigationManager.h"
#include "SoundManager.h"

#include "Level.h"
#include "Layer.h"
#include "Actor.h"
#include "UserInstance.h"
#include "Player.h"
#include "EffectMeshActor.h"
#include "SpriteActor.h"
#include "CaptureEffect.h"

#include "AnimModel.h"
#include "Animation.h"
#include "CameraComponent.h"
#include "ProductionCamera.h"
#include "AttackCollider.h"
#include "Navigation.h"
#include "Cell.h"

using namespace PLAYER_STATE;

void CNahida_StateMachine::Initialize()
{
	wstring wstrOwnerName = m_wpOwnerActor.lock()->GetActorName();
	_float fAtk = dynamic_pointer_cast<CCharactor>(m_wpOwnerActor.lock())->GetAtk();

	m_Attack1_Desc = { wstrOwnerName, L"Player_Nahida Attack1", DENDRO, WEAK,		false,	fAtk * 1.0f };
	m_Attack2_Desc = { wstrOwnerName, L"Player_Nahida Attack2", DENDRO, WEAK,		false,	fAtk * 1.2f };
	m_Attack3_Desc = { wstrOwnerName, L"Player_Nahida Attack3", DENDRO, WEAK,		false,	fAtk * 1.4f };
	m_Attack4_Desc = { wstrOwnerName, L"Player_Nahida Attack4", DENDRO, WEAK,		false,	fAtk * 1.8f };
	m_Attack5_Desc = { wstrOwnerName, L"Player_Nahida Attack5", DENDRO, WEAK,		false,	fAtk * 2.0f };

	m_Skill1_Desc = { wstrOwnerName, L"Player_Nahida Skill1", DENDRO, STRONG,		false,	fAtk * 5.0f };
	m_Skill2_Desc = { wstrOwnerName, L"Player_Nahida Skill2", DENDRO, STRONG,		false,	fAtk * 5.0f };

#pragma region 속도, 시간 등의 구체적인 정보
////////////////
/* == 일반 == */
////////////////
	m_fWalk_Speed = 80.0f;
	m_fTotal_Walk_Speed = 80.0f;

	m_fRun_Speed = 300.0f;

	m_fRun_Start_MinTime = 0.5f;
	m_fRun_Start_RunTime = 0.0f;
	m_fRun_Stop_MinTime = 0.5f;
	m_fRun_Stop_RunTime = 0.0f;

	m_fSprint_Speed = 700.0f;

	m_fSprint_Start_MinTime = 0.8f;
	m_fSprint_Start_RunTime = 0.0f;
	m_fSprint_Stop_MinTime = 1.4f;
	m_fSprint_Stop_RunTime = 0.0f;

	m_fJump_Power = 70.0f;
	m_fJump_RunTime = 0.0f;
	m_fJump_MinTime = 0.5f;

	m_fJumpYAcc = 0.0f;

	m_fClimb_Speed = 250.0f;
	m_fClimbDash_Speed = 1000.0f;

	m_fFly_Speed = 750.0f;

	m_fFlyStart_MinTime = 1.0f;
	m_fFlyStart_RunTime = 0.0f;

////////////////
/* == 전투 == */
////////////////

	m_fAttack_ExitTime = 0.4f;
	m_fAttack_MinTime = 1.2f;
	m_fAttack_RunTime = 0.0f;
	m_fAttackMove_Speed = 5.0f;

	/* ELEMENTAL_SKILL */
	m_iElemental_Skill_ParticleCount = 0;
	m_fElemental_Skill_MinTime = 0.4f;
	m_fElemental_Skill_CoolTime = 6.0f;
	m_fElemental_Skill_RunTime = 0.0f;

	m_fElemental_Charge_MinTime = 5.0f;
	m_fElemental_Charge_RunTime = 0.0f;

	/* ELEMENTAL_BURST */
	m_fElemental_Burst_MinTime = 3.0f;
	m_fElemental_Burst_CoolTime = 15.0f;
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
#pragma endregion 속도, 시간 등의 구체적인 정보

	CStateMachine::Initialize();
}

_int CNahida_StateMachine::PreUpdate(const _float& _fTimeDelta)
{
	return CStateMachine::PreUpdate(_fTimeDelta);
}

_int CNahida_StateMachine::Update(const _float& _fTimeDelta)
{
	ManagementActorState(_fTimeDelta);

	return CStateMachine::Update(_fTimeDelta);
}

_int CNahida_StateMachine::LateUpdate(const _float& _fTimeDelta)
{
	return CStateMachine::LateUpdate(_fTimeDelta);
}

void CNahida_StateMachine::Render()
{
	CStateMachine::Render();
}

void CNahida_StateMachine::EndPlay()
{
	CStateMachine::EndPlay();
}

void CNahida_StateMachine::Release()
{
	if (!m_wpWeaponCollider.expired())
		m_wpWeaponCollider.reset();

	if (!m_wpSkillCollider.expired())
		m_wpSkillCollider.reset();

	CStateMachine::Release();
}

void CNahida_StateMachine::ManagementActorState(const _float& _fTimeDelta)
{
#pragma region 애니메이션 종료 처리
	_uint iCurAnim = m_wpAnimModel.lock()->GetAnimationIndex();
	if (m_wpAnimModel.lock()->GetAnimVector()[iCurAnim]->GetIsFinished() && m_wpAnimModel.lock()->GetIsLoop(iCurAnim))
	{
		RequestChangeState(PLAYER_STATE::STANDBY);
		m_wpAnimModel.lock()->GetAnimVector()[iCurAnim]->SetIsFinished(false);
	}
#pragma endregion 애니메이션 종료 처리

#pragma region 플레이어 정보
	// 플레이어 위치, 방향벡터
	shared_ptr<CPlayer> spPlayer = dynamic_pointer_cast<CPlayer>(m_wpOwnerActor.lock());
	_vector vActorPos = spPlayer->GetActorTranslation();

	_vector vActorRight = spPlayer->GetRightVector();
	_vector vActorUp = spPlayer->GetUpVector();
	_vector vActorLook = spPlayer->GetLookVector();

	// 플레이어 네비 셀 인덱스
	_int& iCurCellIndex = spPlayer->GetCurNaviCellIndex();

	// 룩 벡터에서 Y성분 제거한 벡터
	_vector vActorLookRemoveY = vActorLook - XMVectorSet(0.0f, XMVectorGetY(vActorLook), 0.0f, 0.0f);
	vActorLookRemoveY = XMVector4Normalize(vActorLookRemoveY);
#pragma endregion 플레이어 정보

#pragma region 회전

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

#pragma endregion 회전

	// 사망
	/*if (m_iCurrState & IS_DIE)
	{

	}*/

	// 피격
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

	// 이동
	else if (m_iCurrState & IS_MOVE)
	{
		// 회전
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
				if (0.345f < m_fRunTimeAcc)
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
				if (0.7f < m_fRunTimeAcc)
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

			// 바닥에 닿았으면 점프를 종료한다.
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

			// 바닥에 닿았으면 활공을 종료한다.
			if (IsLand() && XMVectorGetY(vActorPos) <= IsLand())
				RequestChangeState(PLAYER_STATE::STANDBY);
		}
#pragma endregion [ FLYING ]
	}

	// 공격
	if (m_iCurrState & IS_ATTACK)
	{
#pragma region [ NORMAL ATTACK ]
		if (m_iCurrState & IS_NORMALATTACK)
		{
			if (m_iCurrState & PLAYER_STATE::ATTACK_1)
			{
				if (m_fAttack_RunTime < 0.2f)
					spPlayer->SetActorTranslation(vActorPos - (vActorLookRemoveY * m_fAttackMove_Speed * (m_fAttack_RunTime - _fTimeDelta)), &iCurCellIndex);

				if (0.4f < m_fAttack_RunTime && !m_bAttack1_One_Completed)
				{
					m_bAttack1_One_Completed = true;
					PlayNormalAttackVoice();
					CSoundManager::GetInstance()->PlaySoundEx("Nahida_NormalAttack_1.mp3", EFFECT_6, 0.5f);
				}

				NormalAttackColliderOnOff(0.4f, 0.6f);
			}
			else if (m_iCurrState & PLAYER_STATE::ATTACK_2)
			{
				if (m_fAttack_RunTime < 0.2f)
					spPlayer->SetActorTranslation(vActorPos - (vActorLookRemoveY * m_fAttackMove_Speed * (m_fAttack_RunTime - _fTimeDelta)), &iCurCellIndex);

				if (0.4f < m_fAttack_RunTime && !m_bAttack2_One_Completed)
				{
					m_bAttack2_One_Completed = true;
					PlayNormalAttackVoice();
					CSoundManager::GetInstance()->PlaySoundEx("Nahida_NormalAttack_2.mp3", EFFECT_6, 0.5f);
				}

				NormalAttackColliderOnOff(0.4f, 0.6f);
			}
			else if (m_iCurrState & PLAYER_STATE::ATTACK_3)
			{
				if (m_fAttack_RunTime < 0.3f)
					spPlayer->SetActorTranslation(vActorPos - (vActorLookRemoveY * 2.f * m_fAttackMove_Speed * (m_fAttack_RunTime - _fTimeDelta)), &iCurCellIndex);

				if (0.4f < m_fAttack_RunTime && !m_bAttack3_One_Completed)
				{
					m_bAttack3_One_Completed = true;
					PlayNormalAttackVoice();
					CSoundManager::GetInstance()->PlaySoundEx("Nahida_NormalAttack_3.mp3", EFFECT_6, 0.5f);
				}

				NormalAttackColliderOnOff(0.4f, 0.6f);
			}
			else if (m_iCurrState & PLAYER_STATE::ATTACK_4)
			{
				if (m_fAttack_RunTime < 0.4f)
					spPlayer->SetActorTranslation(vActorPos - (vActorLookRemoveY * m_fAttackMove_Speed * (m_fAttack_RunTime - _fTimeDelta)), &iCurCellIndex);

				if (0.4f < m_fAttack_RunTime && !m_bAttack4_One_Completed)
				{
					m_bAttack4_One_Completed = true;
					PlayHeavyAttackVoice();
					CSoundManager::GetInstance()->PlaySoundEx("Nahida_NormalAttack_4.mp3", EFFECT_6, 0.5f);
				}

				NormalAttackColliderOnOff(0.6f, 0.8f);
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
				m_fElementalHoldTimeAcc += _fTimeDelta;

				m_fElemental_Charge_RunTime += _fTimeDelta;
				if (m_fElemental_Charge_RunTime >= m_fElemental_Charge_MinTime)
					m_fElemental_Charge_RunTime = m_fElemental_Charge_MinTime;

				if (m_fElemental_Charge_MinTime < m_fElemental_Charge_RunTime || 
					CInputManager::GetInstance()->IsKeyUp(DIK_E))
					RequestChangeState(PLAYER_STATE::STANDBY);

				CCameraManager::GetInstance()->SetCurCamera(L"Player_ShoulderView_Camera");

				_int iDeltaY = 0;
				if (iDeltaY = CInputManager::GetInstance()->GetMouseDeltaY())
				{
					_vector vRightVector = m_wpOwnerActor.lock()->GetRightVector();
					_matrix matMouseRotateMatrix = m_wpOwnerActor.lock()->GetMouseRotateMatrix();
					_matrix matRotateMatrix;

					matRotateMatrix = XMMatrixRotationAxis(vRightVector, XMConvertToRadians((_float)iDeltaY / 10.0f));
					matMouseRotateMatrix = XMMatrixMultiply(matMouseRotateMatrix, matRotateMatrix);
					m_wpOwnerActor.lock()->SetMouseRotateMatrix(matMouseRotateMatrix);
				}

				_int iDeltaX = 0;
				if (iDeltaX = CInputManager::GetInstance()->GetMouseDeltaX())
				{
					_vector vUpVector = { 0.0f, 1.0f, 0.0f };
					_matrix matMouseRotateMatrix = m_wpOwnerActor.lock()->GetMouseRotateMatrix();
					_matrix matRotateMatrix;

					matRotateMatrix = XMMatrixRotationAxis(vUpVector, XMConvertToRadians((_float)iDeltaX / 10.0f));
					matMouseRotateMatrix = XMMatrixMultiply(matMouseRotateMatrix, matRotateMatrix);
					m_wpOwnerActor.lock()->SetMouseRotateMatrix(matMouseRotateMatrix);
				}

				if (2.f < m_fElementalHoldTimeAcc)
				{
					CSoundManager::GetInstance()->PlaySoundEx("Nahida_ElementalSkill_Hold.mp3", EFFECT_8, 0.5f);
					m_fElementalHoldTimeAcc = 0.f;
				}

				SkillColliderOnOff(0.4f, 5.f);
			}
			else if (m_iCurrState & PLAYER_STATE::ELEMENTAL_BURST)
			{
				#pragma region 연출
				shared_ptr<CProductionCamera> spProductionCam = dynamic_pointer_cast<CProductionCamera>(CCameraManager::GetInstance()->GetCurCamera());

				if (0.79f < m_fElemental_Burst_RunTime && 0 == m_iProductionCount)
				{
					_vector vStartCamPos = vActorPos + (vActorRight * 35.f) + (vActorUp * 180.f) + (vActorLook * 40.f);
					_vector vEndCamPos = vActorPos + (vActorRight * 100.f) + (vActorUp * 80.f) - (vActorLook * 80.f);

					_vector vStartCamAt = vActorPos - (vActorRight * 100.f) + (vActorUp * 500.f) + (vActorLook * 500.f);
					_vector vEndCamAt = vActorPos - (vActorRight * 100.f) + (vActorUp * 500.f) + (vActorLook * 500.f);

					spProductionCam->BeginProduction(vStartCamPos, vEndCamPos, vStartCamAt, vEndCamAt, 0.4f);
					spProductionCam->SetProductionRatio(1.f, 2.0f, 2.0f);
					++m_iProductionCount;
				}
				else if (0.99f < m_fElemental_Burst_RunTime && 1 == m_iProductionCount)
				{
					_vector vStartCamPos = vActorPos + (vActorRight * 100.f) + (vActorUp * 80.f) - (vActorLook * 80.f);
					_vector vEndCamPos = vActorPos + (vActorRight * 30.f) + (vActorUp * 70.f) - (vActorLook * 80.f);

					_vector vStartCamAt = vActorPos + (vActorRight * 50.f) + (vActorUp * 500.f) + (vActorLook * 500.f);
					_vector vEndCamAt = vActorPos + (vActorRight * 50.f) + (vActorUp * 500.f) + (vActorLook * 600.f);

					spProductionCam->BeginProduction(vStartCamPos, vEndCamPos, vStartCamAt, vEndCamAt, 2.0f);
					spProductionCam->SetProductionRatio(1.f, 0.5f, 0.5f);
					++m_iProductionCount;
				}
#pragma endregion 연출

				if (m_fElemental_Burst_MinTime < m_fElemental_Burst_RunTime)
					RequestChangeState(PLAYER_STATE::STANDBY);
			}
		}
	}
	else if (m_iCurrState & PLAYER_STATE::CLIMB_D)
	{
		m_fElemental_Charge_EndRunTime += _fTimeDelta;
		if (m_fElemental_Charge_EndRunTime >= m_fElemental_Charge_EndMinTime)
			m_fElemental_Charge_EndRunTime = m_fElemental_Charge_EndMinTime;

		if (m_fElemental_Charge_EndMinTime <= m_fElemental_Charge_EndRunTime)
			RequestChangeState(PLAYER_STATE::STANDBY);
	}
#pragma endregion [ SKILL ]
}

void CNahida_StateMachine::ManagementActorCoolTime(const _float& _fTimeDelta)
{
	/* == 평타 == */
	m_fAttack_RunTime += _fTimeDelta;
	if (m_fAttack_RunTime >= m_fAttack_MinTime)
		m_fAttack_RunTime = m_fAttack_MinTime;

	/* == 스킬 == */
	m_fElemental_Skill_RunTime += _fTimeDelta;
	if (m_fElemental_Skill_RunTime >= m_fElemental_Skill_CoolTime)
		m_fElemental_Skill_RunTime = m_fElemental_Skill_CoolTime;

	m_fElemental_Burst_RunTime += _fTimeDelta;
	if (m_fElemental_Burst_RunTime >= m_fElemental_Burst_CoolTime)
		m_fElemental_Burst_RunTime = m_fElemental_Burst_CoolTime;
}

void CNahida_StateMachine::RequestChangeState(_ullong _iPlayerStateFlag, REQUEST_DIR _eDirection, ATTACK_DESC _AttackDesc)
{
	// 요청받은 상태 변화가 방향이 현재 상태와 같다면 return, 하지만 피격판정이라면 통과
	if (_iPlayerStateFlag == m_iCurrState && _eDirection == m_eDirrection 
		&& !(_iPlayerStateFlag & IS_HIT))
		return;

	// 요청받은 상태의 방향성
	if (_eDirection != REQUEST_DIR::DIR_NONE)
		m_eDirrection = _eDirection;

#pragma region 플레이어 정보
	shared_ptr<CPlayer> spPlayer = dynamic_pointer_cast<CPlayer>(m_wpOwnerActor.lock());
	_int& iCurCellIndex = spPlayer->GetCurNaviCellIndex();

	_vector vPlayerPos = spPlayer->GetActorTranslation();
	_vector vPlayerRight = XMVector3Normalize(spPlayer->GetRightVector());
	_vector vPlayerUp = XMVector3Normalize(spPlayer->GetUpVector());
	_vector vPlayerLook = XMVector3Normalize(spPlayer->GetLookVector());
#pragma endregion 플레이어 정보

	///////////////////////////////////////////
	// ※ 요청받은 상태 변화의 조건을 체크 ※//
	///////////////////////////////////////////

	// 1. 요청받은 상태가 DIE라면 -> 최우선적으로 죽음 상태를 처리한다.
	// if ()
	// {
		  /*m_wpAnimModel.lock()->SetAnimationIndex(ANIM::DIE);
		  m_iCurrState = _iPlayerStateFlag;*/
	// }
	// 2. 요청받은 상태가 HIT라면 -> HIT_TYPE을 판단하여 차선적으로 처리한다.
#pragma region [ HIT ]
	if (_iPlayerStateFlag & IS_HIT)
	{
		// 2.2 원소 폭발 사용 중이거나 대쉬 시작 모션이면 무적이다.
		if (m_iCurrState & PLAYER_STATE::ELEMENTAL_BURST || m_iCurrState & PLAYER_STATE::SPRINT_START)
			return;

		// 2.3 이전에 받았던 공격의 공격자, 공격 이름을 비교하여 같으면 return;
		if (m_HittedAttack_Desc.wstrOwnerName == _AttackDesc.wstrOwnerName &&
			m_HittedAttack_Desc.wstrAttackName == _AttackDesc.wstrAttackName &&
			false == _AttackDesc.bMultiHit)
			return;

		// 2.4 공격 모션 시전 중이였고, 강한 공격이 아니라면 피격 모션을 스킵하고 데미지만 들어오게 한다.
		if (m_iCurrState & IS_ATTACK && KNOCKBACK_TYPE::POWERFUL != _AttackDesc.eKnockbackType)
		{
			Hitted(_AttackDesc, _AttackDesc.eDamage);
			MakeHitEffect(XMVectorSet(0.0f, 100.f, 0.0f, 0.0f));

			// 맞은 ATTACK_DESC을 기록한다.
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

		m_wpWeaponCollider.lock()->SetIsCollideOn(false);
		m_wpSkillCollider.lock()->SetIsCollideOn(false);

		RemoveCaptureEffect();
		m_wpOwnerActor.lock()->SetMouseRotateMatrix(XMMatrixIdentity());


		PlayHitVoice(_AttackDesc);

		// 피격 이펙트 생성
		Hitted(_AttackDesc, _AttackDesc.eDamage);
		if(!m_wpOwnerActor.expired())
			MakeHitEffect(XMVectorSet(0.0f, 100.f, 0.0f, 0.0f));

		// 맞은 ATTACK_DESC을 기록한다.
		m_HittedAttack_Desc = _AttackDesc;
	}
#pragma endregion [ HIT ]
	// 3. 요청받은 상태가 움직임동작 중 하나라면
	else if (_iPlayerStateFlag & IS_MOVE)
	{
		// 1. DIE나 HIT 중에는 움직일 수 없다.
		if (/*m_iCurrState & IS_DIE || */m_iCurrState & IS_HIT || m_iCurrState & PLAYER_STATE::ELEMENTAL_BURST)
			return;

		// 2. 행동들의 RunTime이 MinTime을 넘어가야 한다.
		if (m_fAttack_ExitTime > m_fAttack_RunTime ||
			m_fElemental_Skill_MinTime > m_fElemental_Skill_RunTime ||
			m_fElemental_Burst_MinTime > m_fElemental_Burst_RunTime)
			return;

		// 3. 그렇지 않다면 상태 변화를 허용한다.
		m_wpWeaponCollider.lock()->SetIsCollideOn(false);
		m_wpSkillCollider.lock()->SetIsCollideOn(false);

		RemoveCaptureEffect();
		m_wpOwnerActor.lock()->SetMouseRotateMatrix(XMMatrixIdentity());
		CCameraManager::GetInstance()->SetCurCamera(L"Player_Production_Camera");

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
				CSoundManager::GetInstance()->PlaySoundEx("Nahida_FlyStart.ogg", SOUND_TYPE::VOICE, 0.1f);
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
	// 4. 요청받은 상태가 만약 공격동작 중 하나라면
	else if (_iPlayerStateFlag & IS_ATTACK)
	{
		// 1. (임시)점프중에는 공격할 수 없다.
		if (m_iCurrState & PLAYER_STATE::IS_FLYING)
			return;

		// 2. 원소폭팔 시전중에는 공격할 수 없다.
		if (m_iCurrState & PLAYER_STATE::ELEMENTAL_BURST)
			return;

		// 3. 그렇지 않다면 상태 변화를 허용한다.
		// 4. 현재 어떤 상태였느냐에 따라 돌아갈 행동을 결정한다.

#pragma region [ NORMAL ATTACK]
		if (_iPlayerStateFlag & IS_NORMALATTACK && m_fAttack_ExitTime <= m_fAttack_RunTime)
		{
			AutoTargetting();
			RemoveCaptureEffect();
			m_wpOwnerActor.lock()->SetMouseRotateMatrix(XMMatrixIdentity());

			ATTACK_DESC AttackDesc = {};

#pragma region 플레이어 정보
			shared_ptr<CPlayer> spPlayer = dynamic_pointer_cast<CPlayer>(m_wpOwnerActor.lock());
			_int& iCurCellIndex = spPlayer->GetCurNaviCellIndex();

			_vector vPlayerPos = spPlayer->GetActorTranslation();
			_vector vPlayerRight = XMVector3Normalize(spPlayer->GetRightVector());
			_vector vPlayerUp = XMVector3Normalize(spPlayer->GetUpVector());
			_vector vPlayerLook = XMVector3Normalize(spPlayer->GetLookVector());
#pragma endregion 갱신된 플레이어 정보
#pragma region AttackDesc
			if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_1)
			{
				AttackDesc = m_Attack1_Desc;
				AttackDesc.wstrAttackName = L"Player_Nahida Attack1" + to_wstring(rand());
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_2)
			{
				AttackDesc = m_Attack2_Desc;
				AttackDesc.wstrAttackName = L"Player_Nahida Attack2" + to_wstring(rand());
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_3)
			{
				AttackDesc = m_Attack3_Desc;
				AttackDesc.wstrAttackName = L"Player_Nahida Attack3" + to_wstring(rand());
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_4)
			{
				AttackDesc = m_Attack4_Desc;
				AttackDesc.wstrAttackName = L"Player_Nahida Attack4" + to_wstring(rand());
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_5)
			{
				AttackDesc = m_Attack5_Desc;
				AttackDesc.wstrAttackName = L"Player_Nahida Attack5" + to_wstring(rand());
			}
#pragma endregion AttackDesc

			if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_1)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ATTACK_1);
				m_wpWeaponCollider.lock()->SetAttackDesc(m_Attack1_Desc);
				m_iCurrState = PLAYER_STATE::ATTACK_1;

				MakeNormalAttackEffect_1(vPlayerPos, vPlayerRight, vPlayerUp, vPlayerLook);
				MakeParticle_LifeTime(L"Particle_Nahida_NormalAttack_One", XMVectorSet(0.0f, 30.f, 0.0f, 0.0f), _float2(0.4f, 1.f));
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_2)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ATTACK_2);
				m_wpWeaponCollider.lock()->SetAttackDesc(m_Attack2_Desc);
				m_iCurrState = PLAYER_STATE::ATTACK_2;

				MakeNormalAttackEffect_2(vPlayerPos, vPlayerRight, vPlayerUp, vPlayerLook);
				MakeParticle_LifeTime(L"Particle_Nahida_NormalAttack_One", XMVectorSet(0.0f, 30.f, 0.0f, 0.0f), _float2(0.4f, 1.f));
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_3)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ATTACK_3);
				m_wpWeaponCollider.lock()->SetAttackDesc(m_Attack3_Desc);
				m_iCurrState = PLAYER_STATE::ATTACK_3;

				MakeNormalAttackEffect_3(vPlayerPos, vPlayerRight, vPlayerUp, vPlayerLook);
				MakeParticle_LifeTime(L"Particle_Nahida_NormalAttack_One", XMVectorSet(0.0f, 30.f, 0.0f, 0.0f), _float2(0.4f, 1.f));
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ATTACK_4)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ATTACK_4);
				m_wpWeaponCollider.lock()->SetAttackDesc(m_Attack4_Desc);
				m_iCurrState = PLAYER_STATE::ATTACK_4;

				MakeNormalAttackEffect_4(vPlayerPos, vPlayerRight, vPlayerUp, vPlayerLook);
				MakeParticle_LifeTime(L"Particle_Nahida_NormalAttack_One", XMVectorSet(0.0f, 30.f, 0.0f, 0.0f), _float2(0.6f, 1.2f));
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
		else if ((_iPlayerStateFlag & PLAYER_STATE::ELEMENTAL_CLICK || _iPlayerStateFlag & PLAYER_STATE::ELEMENTAL_CHARGE)
				&& m_fElemental_Skill_CoolTime <= m_fElemental_Skill_RunTime)
		{
			if (_iPlayerStateFlag & PLAYER_STATE::ELEMENTAL_CLICK)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ELEMENTAL_CLICK);
				m_iCurrState = PLAYER_STATE::ELEMENTAL_CLICK;
				m_wpSkillCollider.lock()->SetAttackDesc(m_Skill1_Desc);
				PlayElementalSkillVoice();
				CSoundManager::GetInstance()->PlaySoundEx("Nahida_ElementalSkill_Start.mp3", EFFECT_6, 0.5f);

				m_fElemental_Skill_RunTime = 0.0f;
			}
			else if (_iPlayerStateFlag & PLAYER_STATE::ELEMENTAL_CHARGE)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ELEMENTAL_CHARGE);
				m_iCurrState = PLAYER_STATE::ELEMENTAL_CHARGE;
				m_wpSkillCollider.lock()->SetAttackDesc(m_Skill1_Desc);

				m_fElemental_Charge_RunTime = 0.0f;
			}
		}
		else if ((_iPlayerStateFlag & PLAYER_STATE::ELEMENTAL_BURST) && m_fElemental_Burst_CoolTime <= m_fElemental_Burst_RunTime)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ELEMENTAL_BURST);
			m_iCurrState = PLAYER_STATE::ELEMENTAL_BURST;

			MakeEffect(L"Nahida_Skill_Q_Palace", XMVectorSet(0.0f, 100.f, 0.0f, 0.0f));

			// 유저 인스턴스에 파티 힐 요청
			dynamic_pointer_cast<CUserInstance>(m_wpOwnerActor.lock()->GetOwnerActor().lock())->RequestPartyHeal(15.f, 2.5f, 2500.f);
#pragma region 연출
			shared_ptr<CPlayer> spPlayer = dynamic_pointer_cast<CPlayer>(m_wpOwnerActor.lock());
			_int& iCurCellIndex = spPlayer->GetCurNaviCellIndex();

			_vector vPlayerPos = spPlayer->GetActorTranslation();
			_vector vPlayerRight = XMVector3Normalize(spPlayer->GetRightVector());
			_vector vPlayerUp = XMVector3Normalize(spPlayer->GetUpVector());
			_vector vPlayerLook = XMVector3Normalize(spPlayer->GetLookVector());

			shared_ptr<CProductionCamera> spProductionCam = dynamic_pointer_cast<CProductionCamera>(CCameraManager::GetInstance()->GetCurCamera());

			_vector vStartCamPos = vPlayerPos + (vPlayerRight * 5.f) + (vPlayerUp * 180.f) + (vPlayerLook * 40.f);
			_vector vEndCamPos = vPlayerPos + (vPlayerRight * 35.f) + (vPlayerUp * 180.f) + (vPlayerLook * 40.f);

			_vector vStartCamAt = vPlayerPos + (vPlayerUp * 180.f);
			_vector vEndCamAt = vPlayerPos + (vPlayerUp * 180.f) + (vPlayerLook * 10.f);

			spProductionCam->BeginProduction(vStartCamPos, vEndCamPos, vStartCamAt, vEndCamAt, 1.2f);
			spProductionCam->SetProductionRatio(1.f, 1.0f, 1.0f);
#pragma endregion 연출

			CSoundManager::GetInstance()->PlaySoundEx("Nahida_ElementalBurst_Full.mp3", EFFECT_6, 0.5f);
			PlayElementalBurstVoice();

			m_fElemental_Burst_RunTime = 0.0f;
		}
	}
#pragma endregion [SKILL]
#pragma region [ STANDBY ]
	else if (_iPlayerStateFlag == PLAYER_STATE::STANDBY)
	{
		// 5.1 STANDBY로 돌아갈 수 있는 상태인지 체크한다.(IS_ATTACK, IS_JUMP, IS_FLYING)
		// if (m_iCurrState & IS_FLYING)
			// return;

		// 5.2 그렇지 않다면 상태변화를 허용한다.
		// 5.3 현재 어떤 상태였느냐에 따라 돌아갈 행동을 결정한다.
		_uint iCurAnim = m_wpAnimModel.lock()->GetAnimationIndex();

		/* [ HIT ] */
		if (m_iCurrState & IS_HIT)
		{
			if (m_iCurrState & PLAYER_STATE::HIT_H && m_fHitH_RunTime >= m_fHitH_MinTime)
			{
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
			//if (m_iCurrState & PLAYER_STATE::ELEMENTAL_BURST && m_fElemental_Burst_RunTime < m_fElemental_Burst_MinTime)
			//	return;

			// Elemental Click
			if (m_iCurrState & PLAYER_STATE::ELEMENTAL_CLICK)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ELEMENTAL_CHARGE);
				m_iCurrState = PLAYER_STATE::ELEMENTAL_CHARGE;

				m_wpOwnerActor.lock()->SetRotation(XMVectorSet(0.f, CalculateActorRotation(), 0.f, 0.f));

				MakeCaptureEffect(L"Nahida_Skill_E_Frame", vPlayerLook * 50.f + vPlayerUp * 150.f);
				MakeCaptureEffect(L"Nahida_Skill_E_Frame_One", vPlayerLook * 50.f + vPlayerUp * 150.f);
				MakeCaptureEffect(L"Nahida_Skill_E_Frame_Two_FlowerDeco", vPlayerLook * 50.f + vPlayerUp * 150.f);

				CSoundManager::GetInstance()->PlaySoundEx("Nahida_ElementalSkill_Hold.mp3", EFFECT_8, 0.5f);

				return;
			}
			else if (m_iCurrState & PLAYER_STATE::ELEMENTAL_CHARGE)
			{
				if (!CInputManager::GetInstance()->IsKeyReleased(DIK_E))
				{
					m_wpAnimModel.lock()->SetAnimationIndex(ANIM::ELEMENTAL_CHARGE);
					m_iCurrState = PLAYER_STATE::ELEMENTAL_CHARGE;
					return;
				}

				m_wpAnimModel.lock()->SetAnimationIndex(ANIM::CLIMB_D);
				m_iCurrState = PLAYER_STATE::CLIMB_D;
				CCameraManager::GetInstance()->SetCurCamera(L"Player_Production_Camera");
				RemoveCaptureEffect();

				CSoundManager::GetInstance()->StopSound(EFFECT_8);
				CSoundManager::GetInstance()->PlaySoundEx("Nahida_ElementalSkill_End.mp3", EFFECT_6, 0.5f);
				return;
			}
			else if (m_iCurrState & PLAYER_STATE::ELEMENTAL_BURST && m_fElemental_Burst_MinTime <= m_fElemental_Burst_RunTime)
			{
				m_iProductionCount = 0;
				// CSoundManager::GetInstance()->PlaySoundEx("Nahida_ElementalBurst_Loop.mp3", EFFECT_7, 0.5f);
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

			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
			m_iCurrState = PLAYER_STATE::STANDBY;
		}
		else if (m_iCurrState & PLAYER_STATE::CLIMB_D && m_fElemental_Charge_EndMinTime <= m_fElemental_Charge_EndRunTime)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
			m_iCurrState = PLAYER_STATE::STANDBY;

			// CSoundManager::GetInstance()->PlaySoundEx("Nahida_ElementalSkill_End.mp3", EFFECT_6, 0.5f);
			RemoveCaptureEffect();

			m_fElemental_Charge_EndRunTime = 0.f;
			m_wpOwnerActor.lock()->SetMouseRotateMatrix(XMMatrixIdentity());
		}

		m_wpWeaponCollider.lock()->SetIsCollideOn(false);
		m_wpSkillCollider.lock()->SetIsCollideOn(false);
	}
#pragma endregion [ STANDBY ]
}

_float CNahida_StateMachine::CalculateArrowRotation()
{
	// 액터 방향벡터에서 y성분 제거하고 정규화
	_vector vActorLook = m_wpOwnerActor.lock()->GetLookVector();
	vActorLook = XMVector3Normalize(XMVectorSetY(vActorLook, 0.0f));

	// z축 방향벡터에서 y성분 제거하고 정규화
	_vector vZ = XMVector3Normalize(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));

	// 내적하여 라디안 각도 도출
	_float fAngle = acos(XMVector3Dot(vZ, vActorLook).m128_f32[0]) * (180.0f / DirectX::XM_PI);

	// Up 벡터와 vCamLook, vActorLook을 외적한 벡터를 비교, 방향이 같다면 각도는 180도 이하이다.
	if (XMVectorGetY(XMVector3Cross(vActorLook, vZ)) > 0)
	{
		fAngle = 360.0f - fAngle;
	}

	return fAngle;
}

void CNahida_StateMachine::MakeNormalAttackEffect_1(_vector _vPlayerPos, _vector _vRight, _vector _vUp, GXMVECTOR _vLook)
{
	_vector vRightOffset = {};

	// 장판
	for (_uint i = 0; i < 3; ++i)
	{
		MakeEffect_LifeTime(L"Nahida_NormalAttack_Floor", _vLook * (_float(i) * 250.f) + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.2f + 0.1f * _float(i)));
	}
	// 잡초
	for (_uint i = 0; i < 4; ++i)
	{
		if (0 == i % 2)
			vRightOffset = -_vRight;
		else
			vRightOffset = _vRight;

		switch (rand() % 3)
		{
		case 0:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_One", _vLook * (_float(i + 1) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.0f + 0.1f * _float(i)));
			break;
		}
		case 1:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Five", _vLook * (_float(i + 1) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.0f + 0.1f * _float(i)));
			break;
		}
		case 2:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Six", _vLook * (_float(i + 1) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.0f + 0.1f * _float(i)));
			break;
		}
		}
	}
	// 풀
	for (_uint i = 0; i < 3; ++i)
	{
		if (0 == i % 2)
			vRightOffset = _vRight;
		else
			vRightOffset = -_vRight;

		MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Two", _vLook * (_float(i + 1) * 150.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 0.8f + 0.1f * _float(i)));
	}
}
void CNahida_StateMachine::MakeNormalAttackEffect_2(_vector _vPlayerPos, _vector _vRight, _vector _vUp, GXMVECTOR _vLook)
{
	_vector vRightOffset = {};

	// 장판
	for (_uint i = 0; i < 3; ++i)
	{
		MakeEffect_LifeTime(L"Nahida_NormalAttack_Floor", _vLook * (_float(i) * 250.f) + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.2f + 0.1f * _float(i)));
	}
	// 잡초
	for (_uint i = 0; i < 4; ++i)
	{
		if (0 == i % 2)
			vRightOffset = _vRight;
		else
			vRightOffset = +_vRight;

		switch (rand() % 3)
		{
		case 0:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_One", _vLook * (_float(i + 1) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.0f + 0.1f * _float(i)));
			break;
		}
		case 1:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Five", _vLook * (_float(i + 1) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.0f + 0.1f * _float(i)));
			break;
		}
		case 2:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Six", _vLook * (_float(i + 1) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.0f + 0.1f * _float(i)));
			break;
		}
		}
	}
	// 풀
	for (_uint i = 0; i < 3; ++i)
	{
		if (0 == i % 2)
			vRightOffset = -_vRight;
		else
			vRightOffset = _vRight;

		MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Three", _vLook * (_float(i + 1) * 150.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.0f + 0.1f * _float(i)));
	}
}
void CNahida_StateMachine::MakeNormalAttackEffect_3(_vector _vPlayerPos, _vector _vRight, _vector _vUp, GXMVECTOR _vLook)
{
	_vector vRightOffset = {};

	// 장판
	for (_uint i = 0; i < 3; ++i)
	{
		MakeEffect_LifeTime(L"Nahida_NormalAttack_Floor", _vLook * (_float(i) * 250.f) + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.2f + 0.1f * _float(i)));
	}
	// 잡초
	for (_uint i = 0; i < 3; ++i)
	{
		if (0 == i % 2)
			vRightOffset = _vRight;
		else
			vRightOffset = +_vRight;

		switch (rand() % 3)
		{
		case 0:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_One", _vLook * (_float(i + 2) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.0f + 0.1f * _float(i)));
			break;
		}
		case 1:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Five", _vLook * (_float(i + 2) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.0f + 0.1f * _float(i)));
			break;
		}
		case 2:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Six", _vLook * (_float(i + 2) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.0f + 0.1f * _float(i)));
			break;
		}
		}
	}
	// 풀
	for (_uint i = 0; i < 5; ++i)
	{
		if (0 == i % 2)
			vRightOffset = -_vRight;
		else
			vRightOffset = _vRight;

		MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Four", _vLook * (_float(i + 1) * 150.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.4f + 0.1f * _float(i), 1.0f + 0.1f * _float(i)));
	}
}
void CNahida_StateMachine::MakeNormalAttackEffect_4(_vector _vPlayerPos, _vector _vRight, _vector _vUp, GXMVECTOR _vLook)
{
	_vector vRightOffset = {};

	// 장판
	for (_uint i = 0; i < 3; ++i)
	{
		MakeEffect_LifeTime(L"Nahida_NormalAttack_Floor", _vLook * (_float(i) * 250.f) + _vUp * 20.f, _float2(0.6f + 0.1f * _float(i), 1.4f + 0.1f * _float(i)));
	}
	// 잡초
	for (_uint i = 0; i < 3; ++i)
	{
		if (0 == i % 2)
			vRightOffset = _vRight;
		else
			vRightOffset = +_vRight;

		switch (rand() % 3)
		{
		case 0:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_One", _vLook * (_float(i + 2) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.6f + 0.1f * _float(i), 1.2f + 0.1f * _float(i)));
			break;
		}
		case 1:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Five", _vLook * (_float(i + 2) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.6f + 0.1f * _float(i), 1.2f + 0.1f * _float(i)));
			break;
		}
		case 2:
		{
			MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Six", _vLook * (_float(i + 2) * 100.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.6f + 0.1f * _float(i), 1.2f + 0.1f * _float(i)));
			break;
		}
		}
	}
	// 풀
	for (_uint i = 0; i < 3; ++i)
	{
		if (0 == i % 2)
			vRightOffset = -_vRight;
		else
			vRightOffset = _vRight;

		MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Four", _vLook * (_float(i + 1) * 150.f) + vRightOffset * 50.f + _vUp * 20.f, _float2(0.6f + 0.1f * _float(i), 1.2f + 0.1f * _float(i)));
	}
	MakeEffect_LifeTime(L"Nahida_NormalAttack_Flower_Seven", _vLook * 600.f, _float2(1.0f, 1.6f));
}

void CNahida_StateMachine::MakeCaptureEffect(const wstring& _wstrEffectMeshName, _vector _vOffset)
{
	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->RecycleFromPool(_wstrEffectMeshName);

	if (nullptr == spActor)
		return;

	shared_ptr<CCaptureEffect> spCaptureEffect = dynamic_pointer_cast<CCaptureEffect>(spActor);

	_float3 vOwnerRotation;
	XMStoreFloat3(&vOwnerRotation, XMVectorSetZ(XMVectorSetX(m_wpOwnerActor.lock()->GetRotation(), 0.f), 0.f));

	_float3 vRotation = dynamic_pointer_cast<CCaptureEffect>(spActor)->GetEffectMeshDesc().vRotation;

	spCaptureEffect->SetRotation(XMLoadFloat3(&vOwnerRotation) + XMLoadFloat3(&vRotation));
	spCaptureEffect->SetActorTranslation_NoNavigation(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
}
void CNahida_StateMachine::MakeEffect_LifeTime(const wstring& _wstrEffectMeshName, _vector _vOffset, _float2 _vLifeTime)
{
	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->RecycleFromPool(_wstrEffectMeshName);
	if (nullptr == spActor)
		return;

	_float3 vOwnerRotation;
	XMStoreFloat3(&vOwnerRotation, XMVectorSetZ(XMVectorSetX(m_wpOwnerActor.lock()->GetRotation(), 0.f), 0.f));

	_float3 vRotation = dynamic_pointer_cast<CEffectMeshActor>(spActor)->GetEffectMeshDesc().vRotation;

	spActor->SetRotation(XMLoadFloat3(&vOwnerRotation) + XMLoadFloat3(&vRotation));
	spActor->SetActorTranslation_NoNavigation(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
	dynamic_pointer_cast<CEffectMeshActor>(spActor)->SetLifeTime(_vLifeTime);
}
void CNahida_StateMachine::MakeParticle_LifeTime(const wstring& _wstrParticleName, _vector _vOffset, _float2 _vLifeTime)
{
	shared_ptr<CActor> spActor = GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RecycleFromPool(_wstrParticleName);
	if (nullptr != spActor)
	{
		spActor->SetActorTranslation_NoNavigation(GetOwnerActor().lock()->GetActorTranslation() + _vOffset);
		dynamic_pointer_cast<CParticleActor>(spActor)->SetLifeTime(_vLifeTime);
	}
}
void CNahida_StateMachine::MakeParticle_E(_vector _vOffset)
{

}
void CNahida_StateMachine::MakeParticle_Q(_vector _vOffset)
{
	
}

void CNahida_StateMachine::PlayNormalAttackVoice()
{
	++m_iSoundCount_LightAttack;

	if (0 == m_iSoundCount_LightAttack % 2)
	{
		switch (m_iSoundCount_LightAttack % 7)
		{
		case 0:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_NormalAttack1.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 1:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_NormalAttack2.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 2:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_NormalAttack3.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 3:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_NormalAttack4.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 4:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_NormalAttack5.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 5:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_NormalAttack6.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 6:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_NormalAttack7.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		}
	}
}
void CNahida_StateMachine::PlayHeavyAttackVoice()
{
	++m_iSoundCount_HeavyAttack;

	// 사운드 재생
	switch (m_iSoundCount_HeavyAttack % 3)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_HeavyAttack1.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_HeavyAttack2.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_HeavyAttack3.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	}
}
void CNahida_StateMachine::PlayHitVoice(ATTACK_DESC _AttackDesc)
{
	// 4. 피격 사운드 재생
	switch (_AttackDesc.eKnockbackType)
	{
	case KNOCKBACK_TYPE::WEAK:
	case KNOCKBACK_TYPE::STRONG:
	{
		switch (rand() % 6)
		{
		case 0:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_Hit_L1.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 1:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_Hit_L2.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 2:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_Hit_L3.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 3:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_Hit_L4.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 4:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_Hit_L5.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 5:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_Hit_L6.ogg", SOUND_TYPE::VOICE, 0.5f);
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
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_HeavyHit1.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 1:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_HeavyHit2.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 2:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_HeavyHit3.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 3:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_HeavyHit4.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 4:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_HeavyHit5.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		case 5:
		{
			CSoundManager::GetInstance()->PlaySoundEx("Nahida_HeavyHit6.ogg", SOUND_TYPE::VOICE, 0.3f);
			break;
		}
		}
		break;
	}
	}
}
void CNahida_StateMachine::PlaySprintVoice()
{
	++m_iSoundCount_SprintStart;

	// 대시 시작 사운드
	switch (m_iSoundCount_SprintStart % 3)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_SprintStart1.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_SprintStart2.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_SprintStart3.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	}
}
void CNahida_StateMachine::PlayJumpVoice()
{
	++m_iSoundCount_Jump;

	// 점프 사운드
	switch (m_iSoundCount_Jump % 5)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Jump1.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Jump2.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Jump3.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 3:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Jump4.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	case 4:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Jump5.ogg", SOUND_TYPE::VOICE, 0.5f);
		break;
	}
	}
}

void CNahida_StateMachine::PlayElementalSkillVoice()
{
	++m_iSoundCount_ElementalSkill;

	// 원소 전투 스킬 사운드 재생
	switch (m_iSoundCount_ElementalSkill % 6)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Elemental_Skill1.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Elemental_Skill2.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Elemental_Skill3.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	case 3:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Elemental_Skill4.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	case 4:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Elemental_Skill5.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	case 5:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Elemental_Skill6.ogg", SOUND_TYPE::VOICE, 0.3f);
		break;
	}
	}
}
void CNahida_StateMachine::PlayElementalChargeVoice()
{
}
void CNahida_StateMachine::PlayElementalBurstVoice()
{
	++m_iSoundCount_ElementalBurst;

	// 원소 폭발 사운드 재생
	switch (m_iSoundCount_ElementalBurst % 3)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Elemental_Burst1.ogg", SOUND_TYPE::VOICE, 0.6f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Elemental_Burst2.ogg", SOUND_TYPE::VOICE, 0.6f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Nahida_Elemental_Burst3.ogg", SOUND_TYPE::VOICE, 0.6f);
		break;
	}
	}
}

void CNahida_StateMachine::PlayRunSound_FootSteps()
{
	switch (m_iRunSoundCount % 4)
	{
	case 0:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_633.ogg", SOUND_TYPE::EFFECT_2, 0.2f);
		break;
	}
	case 1:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_634.ogg", SOUND_TYPE::EFFECT_2, 0.2f);
		break;
	}
	case 2:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_635.ogg", SOUND_TYPE::EFFECT_2, 0.2f);
		break;
	}
	case 3:
	{
		CSoundManager::GetInstance()->PlaySoundEx("Banks1_636.ogg", SOUND_TYPE::EFFECT_2, 0.2f);
		break;
	}
	}

	++m_iRunSoundCount;

	m_fRunTimeAcc = 0.f;
}
void CNahida_StateMachine::PlaySprintSound_FootSteps()
{
}
void CNahida_StateMachine::RemoveCaptureEffect()
{
	// 생성된 캡처 이펙트를 모두 꺼준다.
	const unordered_map<wstring, vector<shared_ptr<CActor>>>& umap_Pool = m_wpLevel.lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->GetActors_Pool();

	auto iter = umap_Pool.find(L"Nahida_Skill_E_Frame");
	if (iter != umap_Pool.end())
	{
		for (auto& Effect : iter->second)
			Effect->SetActorUse(false);
	}

	iter = umap_Pool.find(L"Nahida_Skill_E_Frame_One");
	if (iter != umap_Pool.end())
	{
		for (auto& Effect : iter->second)
			Effect->SetActorUse(false);
	}

	iter = umap_Pool.find(L"Nahida_Skill_E_Frame_Two_FlowerDeco");
	if (iter != umap_Pool.end())
	{
		for (auto& Effect : iter->second)
			Effect->SetActorUse(false);
	}

	iter = umap_Pool.find(L"Nahida_Skill_E_RotateCard");
	if (iter != umap_Pool.end())
	{
		for (auto& Effect : iter->second)
			Effect->SetActorUse(false);
	}
}
