#include "pch.h"
#include "Hili_Fire_StateMachine.h"

#include "ObjectManager.h"

#include "Level.h"

#include "NormalMonster.h"

#include "AnimModel.h"
#include "Animation.h"

using namespace MONSTER_STATE;

void CHili_Fire_StateMachine::Initialize()
{
	wstring wstrOwnerName = m_wpOwnerActor.lock()->GetActorName();
	_float fAtk = dynamic_pointer_cast<CCharactor>(m_wpOwnerActor.lock())->GetAtk();

	m_Attack1_Desc = { wstrOwnerName, L"Hili_Fire Attack1", PYRO, STRONG,		false,	fAtk * 1.0f };
	m_Attack2_Desc = { wstrOwnerName, L"Hili_Fire Attack2", PYRO, POWERFUL,		false,	fAtk * 1.5f };
	m_Attack3_Desc = { wstrOwnerName, L"Hili_Fire Attack3", PYRO, WEAK,			false,	fAtk * 2.0f };
	m_Attack4_Desc = { wstrOwnerName, L"Hili_Fire Attack4", PYRO, WEAK,			false,	fAtk * 1.0f };

	m_Pattern1_Desc = {};
	m_Pattern2_Desc = {};
	m_Pattern3_Desc = {};
	m_Pattern4_Desc = {};

	m_wpOwnerActor.lock()->SetRotation(XMVectorSet(-90.0f, 90.0f, 0.0f, 0.0f));

	CMonsterStateMachine::Initialize();
}

_int CHili_Fire_StateMachine::PreUpdate(const _float& _fTimeDelta)
{
	return CMonsterStateMachine::PreUpdate(_fTimeDelta);
}

_int CHili_Fire_StateMachine::Update(const _float& _fTimeDelta)
{
	ManagementActorCoolTime(_fTimeDelta);
	ManagementActorState(_fTimeDelta);

	return CMonsterStateMachine::Update(_fTimeDelta);
}

_int CHili_Fire_StateMachine::LateUpdate(const _float& _fTimeDelta)
{
	return CMonsterStateMachine::LateUpdate(_fTimeDelta);
}

void CHili_Fire_StateMachine::Render()
{
	CMonsterStateMachine::Render();
}

void CHili_Fire_StateMachine::EndPlay()
{
	CMonsterStateMachine::EndPlay();
}

void CHili_Fire_StateMachine::Release()
{
	CMonsterStateMachine::Release();
}

void CHili_Fire_StateMachine::ManagementActorState(const _float& _fTimeDelta)
{
#pragma region 애니메이션 종료 처리
	_uint iCurAnim = m_wpAnimModel.lock()->GetAnimationIndex();
	if (m_wpAnimModel.lock()->GetAnimVector()[iCurAnim]->GetIsFinished())
	{
		// 애니메이션이 끝났고, 루프 애니메이션이 아니라면, IDLE로 돌아오길 요청한다.
		RequestChangeState(MONSTER_STATE::IDLE);
		m_wpAnimModel.lock()->GetAnimVector()[iCurAnim]->SetIsFinished(false);
	}
#pragma endregion 애니메이션 종료 처리

#pragma region 몬스터의 상태처리에 필요한 정보
	// 자신(몬스터) 포인터
	shared_ptr<CMonster> spMonster = dynamic_pointer_cast<CMonster>(m_wpOwnerActor.lock());

	// 자신(몬스터) 위치
	_vector vMonsterPos = spMonster->GetActorTranslation();

	// 자신(몬스터) 룩벡터
	_vector vMonsterLook = spMonster->GetLookVector();

	// 자신(몬스터) 네비 셀 인덱스
	_int& iCurCellIndex = spMonster->GetCurNaviCellIndex();

	// 플레이어 위치
	// _vector vPlayerPos = CObjectManager::GetInstance()->FindActor(m_wpOwnerActor.lock()->GetLevel().lock()->GetLevelIndex(), LAYER::LAYER_PLAYER, L"Player_Eula")->GetActorTranslation();
	_vector vPlayerPos = CObjectManager::GetInstance()->GetCurPlayer()->GetActorTranslation();

	// 자신(몬스터) -> 플레이어와 몬스터 사이 거리, y좌표 제외한 플레이어로 향하는 방향벡터(정규화)
	_vector vMonsterToPlayer = vPlayerPos - spMonster->GetActorTranslation();

	_float fMonsterToPlayerDistance = XMVector3Length(vMonsterToPlayer).m128_f32[0];

	_vector vMonsterToPlayer_Normal = XMVectorSetY(vMonsterToPlayer, 0.0f);
	vMonsterToPlayer_Normal = XMVector3Normalize(vMonsterToPlayer_Normal);

	// 몬스터 이동속도
	_float fMoveSpeed = spMonster->GetMoveSpeed();

	// 초기 스폰지점 기준으로 구한 랜덤한 방향벡터, 일단 임시로 그냥 랜덤하게 해봄
	_vector vRandomDir = XMVectorSet(_float(rand() % 1000), _float(rand() % 1000), _float(rand() % 1000), 0.f);
	vRandomDir = XMVector3Normalize(vRandomDir);
#pragma endregion 몬스터의 상태처리에 필요한 정보

#pragma region 회전
	_float fAngle = 0.0f;

	if (!XMVectorIsNaN(vMonsterToPlayer).m128_f32[0])
	{
		fAngle = CaculateMonsterRotation(vMonsterLook, vMonsterToPlayer);

		fAngle += 90.0f;

		fAngle = -(fAngle - m_fPrevAngle);
	}
#pragma endregion 회전

	if (spMonster->GetCurHp() <= 0.0f)
		RequestChangeState(MONSTER_STATE::DIE1);

	// 사망
	if (m_iCurrState & IS_DIE)
	{
		if (m_iCurrState & MONSTER_STATE::DIE1)
			RequestChangeState(MONSTER_STATE::DIE1);
		else if (m_iCurrState & MONSTER_STATE::DIE2)
			RequestChangeState(MONSTER_STATE::DIE2);
		else if (m_iCurrState & MONSTER_STATE::DIE3)
			RequestChangeState(MONSTER_STATE::DIE3);
	}
	// 피격
	else if (m_iCurrState & IS_HIT)
	{
		if (m_iCurrState & MONSTER_STATE::HIT1)
		{
			m_tInfo.m_fHit1RunTime += _fTimeDelta;
			if (m_tInfo.m_fHit1RunTime >= m_tInfo.m_fHit1MinTime)
				RequestChangeState(MONSTER_STATE::IDLE);
		}
		else if (m_iCurrState & MONSTER_STATE::HIT2)
		{
			m_tInfo.m_fHit2RunTime += _fTimeDelta;
			if (m_tInfo.m_fHit2RunTime >= m_tInfo.m_fHit2MinTime)
				RequestChangeState(MONSTER_STATE::IDLE);

		}
		else if (m_iCurrState & MONSTER_STATE::HIT3)
		{
			m_tInfo.m_fHit3RunTime += _fTimeDelta;
			if (m_tInfo.m_fHit2RunTime >= m_tInfo.m_fHit2MinTime)
				RequestChangeState(MONSTER_STATE::IDLE);
		}
	}
	// 움직임
	else if (m_iCurrState & IS_MOVE)
	{
		if (!isnan(fAngle))
		{
			spMonster->SetRotation(XMVectorSet(-90.0f, fAngle, 0.0f, 0.0f));

			// (보간용)이전의 방향을 저장
			m_fPrevAngle = fAngle;
		}

		if (m_iCurrState & MONSTER_STATE::PROWL)
		{
			switch (m_wpAnimModel.lock()->GetAnimationIndex())
			{
			case HILI_FIRE_ANIM::WALK_LEFT:
			{
				_matrix matRoationY90 = XMMatrixRotationY(XMConvertToRadians(90.0f));
				_vector vRotationY90 = XMVector3TransformNormal(vMonsterToPlayer_Normal, matRoationY90);
				spMonster->SetActorTranslation(vMonsterPos + (vRotationY90 * fMoveSpeed * _fTimeDelta), &iCurCellIndex);
				break;
			}
			case HILI_FIRE_ANIM::WALK_RIGHT:
			{
				_matrix matRoationYMinus90 = XMMatrixRotationY(XMConvertToRadians(90.0f));
				_vector vRotationYMinus90 = XMVector3TransformNormal(vMonsterToPlayer_Normal, matRoationYMinus90);
				spMonster->SetActorTranslation(vMonsterPos + (vRotationYMinus90 * fMoveSpeed * _fTimeDelta), &iCurCellIndex);
				break;
			}
			}

			if (fMonsterToPlayerDistance >= 600.f)
				RequestChangeState(MONSTER_STATE::TRACE);
		}
		else if (m_iCurrState & MONSTER_STATE::TRACE)
		{
			spMonster->SetActorTranslation(vMonsterPos + (vMonsterToPlayer_Normal * fMoveSpeed * _fTimeDelta), &iCurCellIndex);

			if (fMonsterToPlayerDistance < 450.f)
				RequestChangeState(MONSTER_STATE::PROWL);
		}

		/* ※ 공격 / 패턴의 우선순위는 숫자가 높을수록 높다고 가정한다. */
		// 움직이다가, 플레이어가 공격 범위 안으로 들어오고, 내부쿨이 지났다면, 공격 모션에 들어간다.
		if (fMonsterToPlayerDistance < m_tInfo.m_fAttack2Radius && m_tInfo.m_fAttack2CoolTime >= m_tInfo.m_fAttack2MinTime)
		{
			RequestChangeState(MONSTER_STATE::ATTACK2_START);
			m_tInfo.m_fAttack2CoolTime = 0.0f;
			return;
		}
		if (fMonsterToPlayerDistance < m_tInfo.m_fAttack1Radius && m_tInfo.m_fAttack1CoolTime >= m_tInfo.m_fAttack1MinTime)
		{
			RequestChangeState(MONSTER_STATE::ATTACK1_START);
			m_tInfo.m_fAttack1CoolTime = 0.0f;
			return;
		}
	}
	// 공격 및 패턴
	else if (m_iCurrState & IS_ATTACK)
	{
		if (m_iCurrState & IS_NORMALATTACK)
		{
			// [ NORMAL ATTACK ]
			if (m_iCurrState & IS_ATTACK1)
			{
				if (m_iCurrState & MONSTER_STATE::ATTACK1_START)
				{
					if (!isnan(fAngle))
					{
						spMonster->SetRotation(XMVectorSet(-90.0f, fAngle, 0.0f, 0.0f));

						// (보간용)이전의 방향을 저장
						m_fPrevAngle = fAngle;
					}

					_float fRatio = (m_tInfo.m_fAttack1RunTime - powf(m_tInfo.m_fAttack1CoolTime, 4)) / m_tInfo.m_fAttack1RunTime;
					if (fRatio < 0.0f)
						fRatio = 0.0f;

					spMonster->SetActorTranslation(vMonsterPos + (vMonsterToPlayer_Normal * 2.0f * fRatio  * fMoveSpeed * _fTimeDelta), &iCurCellIndex);
				}
			}
			// [ CHARGE ]
			else if (m_iCurrState & IS_ATTACK2)
			{
				if (m_iCurrState & MONSTER_STATE::ATTACK2_START)
				{

				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK2_CYCLE)
				{
					if (!isnan(fAngle))
					{
						spMonster->SetRotation(XMVectorSet(-90.0f, fAngle, 0.0f, 0.0f));

						// (보간용)이전의 방향을 저장
						m_fPrevAngle = fAngle;
					}

					spMonster->SetActorTranslation(vMonsterPos + (vMonsterToPlayer_Normal * 3.0f * fMoveSpeed * _fTimeDelta), &iCurCellIndex);
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK2_END)
				{

				}
			}
			else if (m_iCurrState & IS_ATTACK3)
			{

			}
			else if (m_iCurrState & IS_ATTACK4)
			{

			}
			SetAllAttackColliderOn();
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
		}
	}
	// 아이들
	else if (m_iCurrState == MONSTER_STATE::IDLE)
	{
		// 플레이어와 거리가 일정 이상 가까워지면 DETECTED 상태로 돌입한다.
		if (fMonsterToPlayerDistance < m_tInfo.m_fDetectingRadius)
			RequestChangeState(MONSTER_STATE::DETECTED);
	}
}

void CHili_Fire_StateMachine::ManagementActorCoolTime(const _float& _fTimeDelta)
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
}

void CHili_Fire_StateMachine::RequestChangeState(_ullong _iMonsterStateFlag, REQUEST_DIR _eDirection, ATTACK_DESC _AttackDesc)
{
	// 요청받은 상태 변화가 방향이 현재 상태와 같다면 return, 하지만 피격판정이라면 통과
	if ((_iMonsterStateFlag == m_iCurrState && _eDirection == m_eDirrection) && !(_iMonsterStateFlag & IS_HIT))
		return;

	// 요청받은 상태의 방향성
	if (_eDirection != REQUEST_DIR::DIR_NONE)
		m_eDirrection = _eDirection;

	///////////////////////////////////////////
	// ※ 요청받은 상태 변화의 조건을 체크 ※//
	///////////////////////////////////////////

	// 1. 요청받은 상태가 DIE라면 -> 최우선적으로 죽음 상태를 처리한다.
	if (_iMonsterStateFlag & IS_DIE && dynamic_pointer_cast<CMonster>(m_wpOwnerActor.lock())->GetCurHp() <= 0.0f)
	{
		m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::HIT_THROW_TO_DIE);
		m_iCurrState = _iMonsterStateFlag;

		SetAllAttackColliderOff();
		dynamic_pointer_cast<CMonster>(m_wpOwnerActor.lock())->RemoveMonsterCollider();
	}
	// 2. 요청받은 상태가 HIT라면 -> HIT_TYPE을 판단하여 차선적으로 처리한다.
	else if (_iMonsterStateFlag & IS_HIT)
	{
		// 2.1 어짜피 죽은놈을 또 때리지 말자
		if (m_iCurrState & IS_DIE)
			return;

		// 2.2 이전에 받았던 공격의 공격자, 공격 이름을 비교하여 같으면 return;
		if (m_HittedAttack_Desc.wstrOwnerName == _AttackDesc.wstrOwnerName &&
			m_HittedAttack_Desc.wstrAttackName == _AttackDesc.wstrAttackName &&
			false == _AttackDesc.bMultiHit)
			return;

		// 2.3 여러가지 피격 모션이 존재할 가능성을 열어둔다.
		switch (_iMonsterStateFlag)
		{
		case MONSTER_STATE::HIT1:
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::HIT1);
			m_tInfo.m_fHit1RunTime = 0.0f;
			break;

		case MONSTER_STATE::HIT2:
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::HIT2);
			m_tInfo.m_fHit2RunTime = 0.0f;
			break;

		case MONSTER_STATE::HIT3:
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::HIT_THROW_TO_GROUND);
			m_tInfo.m_fHit3RunTime = 0.0f;
			break;
		}

		// HP를 깎는다.
		Hitted(_AttackDesc, _AttackDesc.eDamage, XMVectorSet(0.f, 150.f, 0.f, 0.f));
		MakeHitEffect();
		dynamic_pointer_cast<CNormalMonster>(m_wpOwnerActor.lock())->SetHPBarEnable(true);

		// 콜라이더 off
		SetAllAttackColliderOff();

		// 맞은 ATTACK_DESC을 기록한다.
		m_HittedAttack_Desc = _AttackDesc;

		m_iCurrState = _iMonsterStateFlag;
	}
	// 3. 요청받은 상태가 움직임동작 중 하나라면
	else if (_iMonsterStateFlag & IS_MOVE)
	{
		// 3.1 죽거나 맞고있으면 움직일 수 없다.
		if (m_iCurrState & IS_DIE || m_iCurrState & IS_HIT)
			return;

		// 3.2 그렇지 않다면 상태 변화를 허용한다.
		if (_iMonsterStateFlag & MONSTER_STATE::PROWL)
		{
			switch (rand() % 2)
			{
			case 0:
			{
				m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::WALK_LEFT);
				break;
			}
			case 1:
			{
				m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::WALK_RIGHT);
				break;
			}
			}
		}
		else if (_iMonsterStateFlag & MONSTER_STATE::TRACE)
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::WALK_FORWARD);
		else if (_iMonsterStateFlag & MONSTER_STATE::DETECTED)
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::WALK_BACK);

		m_iCurrState = _iMonsterStateFlag;
	}
	// 4. 요청받은 상태가 공격동작 중 하나라면
	else if (_iMonsterStateFlag & IS_ATTACK)
	{
		// 4.1 현재 애니메이션이 끝난 상태가 아니면 움직임을 허용하지 않는다.
		// 몬스터는 플레이어와 다르게 한 동작이 끝나지 않으면 상태 변화x
		// _uint iCurAnim = m_wpAnimModel.lock()->GetAnimationIndex();
		// if (!m_wpAnimModel.lock()->GetAnimVector()[iCurAnim]->GetIsFinished())
			// return;

		// 4.2 DIE나 HIT 중에는 공격할 수 없다.
		if (m_iCurrState & IS_DIE || m_iCurrState & IS_HIT)
			return;

		// 4.3 일반 공격
		if (_iMonsterStateFlag & IS_NORMALATTACK)
		{
			if (_iMonsterStateFlag & IS_ATTACK_START)
			{
				switch (_iMonsterStateFlag)
				{
				case ATTACK1_START:
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::NORMAL_ATTACK);
					m_Attack1_Desc.wstrAttackName = L"Hili_Fire Attack1" + to_wstring(rand());
					SetAttackDescToAttackCollider(L"AttackCollider", m_Attack1_Desc);
					m_iCurrState = MONSTER_STATE::ATTACK1_START;
					break;

				case ATTACK2_START:
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::RUSH_ATTACK_START);
					m_Attack2_Desc.wstrAttackName = L"Hili_Fire Attack2" + to_wstring(rand());
					SetAttackDescToAttackCollider(L"AttackCollider", m_Attack2_Desc);
					m_iCurrState = MONSTER_STATE::ATTACK2_START;
					break;

				case ATTACK3_START:
					break;

				case ATTACK4_START:
					break;
				}
			}
			else if (_iMonsterStateFlag & IS_ATTACK_CYCLE)
			{
				switch (_iMonsterStateFlag)
				{
				case ATTACK1_CYCLE:
					break;

				case ATTACK2_CYCLE:
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::RUSH_ATTACK_CYCLE);
					m_iCurrState = MONSTER_STATE::ATTACK2_CYCLE;
					break;

				case ATTACK3_CYCLE:
					break;

				case ATTACK4_CYCLE:
					break;
				}
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
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::RUSH_ATTACK_END);
					m_iCurrState = MONSTER_STATE::ATTACK2_END;
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
			SetAllAttackColliderOn();
		}
		// 4.4 패턴 공격
		else if (_iMonsterStateFlag & IS_PATTERN)
		{
			// 나중에 보스 짤때 짭시다잇.
		}
	}
	// 5. 요청받은 상태가 DETECTED라면
	else if (_iMonsterStateFlag & MONSTER_STATE::DETECTED)
	{
		switch (rand() % 2)
		{
		case 0:
		{
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::WALK_LEFT);
			m_iCurrState = MONSTER_STATE::PROWL;
			break;
		}
		case 1:
		{
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::WALK_RIGHT);
			m_iCurrState = MONSTER_STATE::PROWL;
			break;
		}
		}
	}
	// 6. 요청받은 상태가 IDLE이라면
	else if (_iMonsterStateFlag == MONSTER_STATE::IDLE)
	{
		// 6.1 IDLE로 돌아갈 수 있는 상태인지 체크한다.
		if (m_iCurrState & IS_DIE)
			return;

		// 몬스터는 딱히 IDLE로 돌아가기 위한 조건이 많이 없어보인다.
		// 배회중이거나 IDLE이 아니였다면, 거의 플레이어와 전투 중 일 것이기 때문

		// 6.2 몬스터가 공격 중이였다면
		if (m_iCurrState & IS_ATTACK)
		{
			// 어떤 공격중이였는지 체크해서, 맞는 상태로 돌아간다.
			if (m_iCurrState & IS_ATTACK1)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::WALK_FORWARD);
				m_iCurrState = MONSTER_STATE::TRACE;
			}
			else if (m_iCurrState & IS_ATTACK2)
			{
				if (m_iCurrState & MONSTER_STATE::ATTACK2_START)
				{
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::RUSH_ATTACK_CYCLE);
					m_iCurrState = MONSTER_STATE::ATTACK2_CYCLE;
					return;
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK2_CYCLE)
				{
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::RUSH_ATTACK_END);
					m_iCurrState = MONSTER_STATE::ATTACK2_END;
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK2_END)
				{
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::WALK_FORWARD);
					m_iCurrState = MONSTER_STATE::TRACE;
				}
			}
		}
		// 6.3 몬스터가 HIT 모션 중 이였다면
		else if (m_iCurrState & IS_HIT)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::WALK_FORWARD);
			m_iCurrState = MONSTER_STATE::TRACE;
		}
		// 6.4 몬스터가 DETECTED 모션 중 이였다면
		else if (m_iCurrState & MONSTER_STATE::DETECTED)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_FIRE_ANIM::WALK_FORWARD);
			m_iCurrState = MONSTER_STATE::TRACE;
		}

		// 콜라이더 off
		SetAllAttackColliderOff();
	}
}
