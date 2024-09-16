#include "pch.h"
#include "Hili_Ice_StateMachine.h"

#include "ObjectManager.h"

#include "Level.h"
#include "Layer.h"

#include "NormalMonster.h"
#include "ProjectileActor.h"

#include "AnimModel.h"
#include "Animation.h"

using namespace MONSTER_STATE;

void CHili_Ice_StateMachine::Initialize()
{
	wstring wstrOwnerName = m_wpOwnerActor.lock()->GetActorName();
	_float fAtk = dynamic_pointer_cast<CCharactor>(m_wpOwnerActor.lock())->GetAtk();

	m_Attack1_Desc = { wstrOwnerName, L"Hili_Ice Attack1", CRYO, STRONG,	false, fAtk * 1.0f };
	m_Attack2_Desc = { wstrOwnerName, L"Hili_Ice Attack2", CRYO, STRONG,	false, fAtk * 1.5f };
	m_Attack3_Desc = { wstrOwnerName, L"Hili_Ice Attack3", CRYO, WEAK,		false, fAtk * 2.0f };
	m_Attack4_Desc = { wstrOwnerName, L"Hili_Ice Attack4", CRYO, WEAK,		false, fAtk * 1.0f };

	m_Pattern1_Desc = {};
	m_Pattern2_Desc = {};
	m_Pattern3_Desc = {};
	m_Pattern4_Desc = {};

	m_wpOwnerActor.lock()->SetRotation(XMVectorSet(-90.0f, 90.0f, 0.0f, 0.0f));

	CMonsterStateMachine::Initialize();
}

_int CHili_Ice_StateMachine::PreUpdate(const _float& _fTimeDelta)
{
	return CMonsterStateMachine::PreUpdate(_fTimeDelta);
}

_int CHili_Ice_StateMachine::Update(const _float& _fTimeDelta)
{
	ManagementActorCoolTime(_fTimeDelta);
	ManagementActorState(_fTimeDelta);

	return CMonsterStateMachine::Update(_fTimeDelta);
}

_int CHili_Ice_StateMachine::LateUpdate(const _float& _fTimeDelta)
{
	return CMonsterStateMachine::LateUpdate(_fTimeDelta);
}

void CHili_Ice_StateMachine::Render()
{
	CMonsterStateMachine::Render();
}

void CHili_Ice_StateMachine::EndPlay()
{
	CMonsterStateMachine::EndPlay();
}

void CHili_Ice_StateMachine::Release()
{
	CMonsterStateMachine::Release();
}

void CHili_Ice_StateMachine::ManagementActorState(const _float& _fTimeDelta)
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
	/*_vector vPlayerPos = CObjectManager::GetInstance()->FindActor(m_wpOwnerActor.lock()->GetLevel().lock()->GetLevelIndex(), LAYER::LAYER_PLAYER, L"Player_Eula")->GetActorTranslation();*/
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
		fAngle = CaculateMonsterRotation(vMonsterLook, vMonsterToPlayer);

		fAngle += 90.0f;

		fAngle = -(fAngle - m_fPrevAngle);
	}
#pragma endregion ȸ��

	if (spMonster->GetCurHp() <= 0.0f)
		RequestChangeState(MONSTER_STATE::DIE1);

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
	// ������
	else if (m_iCurrState & IS_MOVE)
	{
		if (!isnan(fAngle))
		{
			spMonster->SetRotation(XMVectorSet(-90.0f, fAngle, 0.0f, 0.0f));

			// (������)������ ������ ����
			m_fPrevAngle = fAngle;
		}

		if (m_iCurrState & MONSTER_STATE::PROWL)
		{
			switch (m_wpAnimModel.lock()->GetAnimationIndex())
			{
			case HILI_ICE_ANIM::WALK_LEFT:
			{
				_matrix matRoationY90 = XMMatrixRotationY(XMConvertToRadians(90.0f));
				_vector vRotationY90 = XMVector3TransformNormal(vMonsterToPlayer_Normal, matRoationY90);
				spMonster->SetActorTranslation(vMonsterPos + (vRotationY90 * fMoveSpeed * _fTimeDelta), &iCurCellIndex);
				break;
			}
			case HILI_ICE_ANIM::WALK_RIGHT:
			{
				_matrix matRoationYMinus90 = XMMatrixRotationY(XMConvertToRadians(-90.0f));
				_vector vRotationYMinus90 = XMVector3TransformNormal(vMonsterToPlayer_Normal, matRoationYMinus90);
				spMonster->SetActorTranslation(vMonsterPos + (vRotationYMinus90 * fMoveSpeed * _fTimeDelta), &iCurCellIndex);
				break;
			}
			}

			if (fMonsterToPlayerDistance >= 1200.0f)
				RequestChangeState(MONSTER_STATE::TRACE);
		}
		else if (m_iCurrState & MONSTER_STATE::TRACE)
		{
			spMonster->SetActorTranslation(vMonsterPos + (vMonsterToPlayer_Normal * fMoveSpeed * _fTimeDelta), &iCurCellIndex);

			if (fMonsterToPlayerDistance < 900.0f)
				RequestChangeState(MONSTER_STATE::PROWL);
		}

		/* �� ���� / ������ �켱������ ���ڰ� �������� ���ٰ� �����Ѵ�. */
		// �����̴ٰ�, �÷��̾ ���� ���� ������ ������, �������� �����ٸ�, ���� ��ǿ� ����.
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
	// ���� �� ����
	else if (m_iCurrState & IS_ATTACK)
	{
		if (m_iCurrState & IS_NORMALATTACK)
		{
			// [ NORMAL ATTACK ]
			if (m_iCurrState & IS_ATTACK1)
			{
				if (m_iCurrState & MONSTER_STATE::ATTACK1_START)
				{
					// ���ӽá� : ����ü ���� �׽�Ʈ
					if (0.2f <= m_tInfo.m_fAttack1CoolTime && m_tInfo.m_fAttack1CoolTime < 0.3f)
						MakeProjectile(vMonsterToPlayer_Normal, m_Attack1_Desc);

					if (m_tInfo.m_fAttack1CoolTime >= m_tInfo.m_fAttack1RunTime)
						RequestChangeState(MONSTER_STATE::IDLE);
				}
			}
			// [ CHARGE ]
			else if (m_iCurrState & IS_ATTACK2)
			{
				if (m_iCurrState & MONSTER_STATE::ATTACK2_START)
				{
					spMonster->SetActorTranslation(vMonsterPos - (vMonsterToPlayer_Normal * 8.f * fMoveSpeed *
						((m_tInfo.m_fAttack2RunTime - m_tInfo.m_fAttack2CoolTime) / m_tInfo.m_fAttack2RunTime) * _fTimeDelta), &iCurCellIndex);

					if (m_tInfo.m_fAttack2CoolTime >= m_tInfo.m_fAttack2RunTime)
						RequestChangeState(MONSTER_STATE::ATTACK2_CYCLE);
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK2_CYCLE)
				{
					if (m_tInfo.m_fAttack2CoolTime >= m_tInfo.m_fAttack2RunTime)
						MakeMultipleProjectile(vMonsterToPlayer_Normal, m_Attack2_Desc);
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
	// ���̵�
	else if (m_iCurrState == MONSTER_STATE::IDLE)
	{
		// �÷��̾�� �Ÿ��� ���� �̻� ��������� DETECTED ���·� �����Ѵ�.
		if (fMonsterToPlayerDistance < m_tInfo.m_fDetectingRadius)
			RequestChangeState(MONSTER_STATE::DETECTED);
	}
}

void CHili_Ice_StateMachine::ManagementActorCoolTime(const _float& _fTimeDelta)
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

void CHili_Ice_StateMachine::RequestChangeState(_ullong _iMonsterStateFlag, REQUEST_DIR _eDirection, ATTACK_DESC _AttackDesc)
{
	// ��û���� ���� ��ȭ�� ������ ���� ���¿� ���ٸ� return, ������ �ǰ������̶�� ���
	if ((_iMonsterStateFlag == m_iCurrState && _eDirection == m_eDirrection) && !(_iMonsterStateFlag & IS_HIT))
		return;

	// ��û���� ������ ���⼺
	if (_eDirection != REQUEST_DIR::DIR_NONE)
		m_eDirrection = _eDirection;

	///////////////////////////////////////////
	// �� ��û���� ���� ��ȭ�� ������ üũ ��//
	///////////////////////////////////////////

	// 1. ��û���� ���°� DIE��� -> �ֿ켱������ ���� ���¸� ó���Ѵ�.
	if (_iMonsterStateFlag & IS_DIE && dynamic_pointer_cast<CMonster>(m_wpOwnerActor.lock())->GetCurHp() <= 0.0f)
	{
		m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::HIT_THROW_TO_DIE);
		m_iCurrState = _iMonsterStateFlag;

		SetAllAttackColliderOff();
		dynamic_pointer_cast<CMonster>(m_wpOwnerActor.lock())->RemoveMonsterCollider();
	}
	// 2. ��û���� ���°� HIT��� -> HIT_TYPE�� �Ǵ��Ͽ� ���������� ó���Ѵ�.
	else if (_iMonsterStateFlag & IS_HIT)
	{
		// 2.1 ��¥�� �������� �� ������ ����
		if (m_iCurrState & IS_DIE)
			return;

		// 2.2 ������ �޾Ҵ� ������ ������, ���� �̸��� ���Ͽ� ������ return;
		if (m_HittedAttack_Desc.wstrOwnerName == _AttackDesc.wstrOwnerName &&
			m_HittedAttack_Desc.wstrAttackName == _AttackDesc.wstrAttackName &&
			false == _AttackDesc.bMultiHit)
			return;

		// 2.3 �������� �ǰ� ����� ������ ���ɼ��� ����д�.
		switch (_iMonsterStateFlag)
		{
		case MONSTER_STATE::HIT1:
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::HIT1);
			m_tInfo.m_fHit1RunTime = 0.0f;
			break;

		case MONSTER_STATE::HIT2:
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::HIT2);
			m_tInfo.m_fHit2RunTime = 0.0f;
			break;

		case MONSTER_STATE::HIT3:
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::HIT_THROW_TO_GROUND);
			m_tInfo.m_fHit3RunTime = 0.0f;
			break;
		}

		// HP�� ��´�.
		Hitted(_AttackDesc, _AttackDesc.eDamage, XMVectorSet(0.f, 150.f, 0.f, 0.f));
		MakeHitEffect();
		dynamic_pointer_cast<CNormalMonster>(m_wpOwnerActor.lock())->SetHPBarEnable(true);

		// �ݶ��̴� off
		SetAllAttackColliderOff();

		// ���� ATTACK_DESC�� ����Ѵ�.
		m_HittedAttack_Desc = _AttackDesc;

		m_iCurrState = _iMonsterStateFlag;
	}
	// 3. ��û���� ���°� �����ӵ��� �� �ϳ����
	else if (_iMonsterStateFlag & IS_MOVE)
	{
		// 3.1 �װų� �°������� ������ �� ����.
		if (m_iCurrState & IS_DIE || m_iCurrState & IS_HIT)
			return;

		// 3.2 �׷��� �ʴٸ� ���� ��ȭ�� ����Ѵ�.
		if (_iMonsterStateFlag & MONSTER_STATE::PROWL)
		{
			switch (rand() % 2)
			{
			case 0:
			{
				m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::WALK_LEFT);
				break;
			}
			case 1:
			{
				m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::WALK_RIGHT);
				break;
			}
			}
		}
		else if (_iMonsterStateFlag & MONSTER_STATE::TRACE)
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::WALK_FORWARD);
		else if (_iMonsterStateFlag & MONSTER_STATE::DETECTED)
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::WALK_BACK);

		m_iCurrState = _iMonsterStateFlag;
	}
	// 4. ��û���� ���°� ���ݵ��� �� �ϳ����
	else if (_iMonsterStateFlag & IS_ATTACK)
	{
		// 4.1 ���� �ִϸ��̼��� ���� ���°� �ƴϸ� �������� ������� �ʴ´�.
		// ���ʹ� �÷��̾�� �ٸ��� �� ������ ������ ������ ���� ��ȭx
		// _uint iCurAnim = m_wpAnimModel.lock()->GetAnimationIndex();
		// if (!m_wpAnimModel.lock()->GetAnimVector()[iCurAnim]->GetIsFinished())
			// return;

		// 4.2 DIE�� HIT �߿��� ������ �� ����.
		if (m_iCurrState & IS_DIE || m_iCurrState & IS_HIT)
			return;

		// 4.3 �Ϲ� ����
		if (_iMonsterStateFlag & IS_NORMALATTACK)
		{
			if (_iMonsterStateFlag & IS_ATTACK_START)
			{
				switch (_iMonsterStateFlag)
				{
				case ATTACK1_START:
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::ATTACK);
					m_Attack1_Desc.wstrAttackName = L"Hili_Ice Attack1" + to_wstring(rand());
					SetAttackDescToAttackCollider(L"AttackCollider", m_Attack1_Desc);
					m_iCurrState = MONSTER_STATE::ATTACK1_START;
					break;

				case ATTACK2_START:
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::DODGE);
					m_Attack2_Desc.wstrAttackName = L"Hili_Ice Attack2" + to_wstring(rand());
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
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::ATTACK);
					SetAttackDescToAttackCollider(L"AttackCollider", m_Attack2_Desc);
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
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::ATTACK);
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
		// 4.4 ���� ����
		else if (_iMonsterStateFlag & IS_PATTERN)
		{
			// ���߿� ���� ©�� ¬�ô���.
		}
	}
	// 5. ��û���� ���°� DETECTED���
	else if (_iMonsterStateFlag & MONSTER_STATE::DETECTED)
	{
		m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::DODGE);
		m_iCurrState = MONSTER_STATE::DETECTED;
		return;
	}
	// 6. ��û���� ���°� IDLE�̶��
	else if (_iMonsterStateFlag == MONSTER_STATE::IDLE)
	{
		// 6.1 IDLE�� ���ư� �� �ִ� �������� üũ�Ѵ�.
		if (m_iCurrState & IS_DIE)
			return;

		// ���ʹ� ���� IDLE�� ���ư��� ���� ������ ���� ����δ�.
		// ��ȸ���̰ų� IDLE�� �ƴϿ��ٸ�, ���� �÷��̾�� ���� �� �� ���̱� ����

		// 6.2 ���Ͱ� ���� ���̿��ٸ�
		if (m_iCurrState & IS_ATTACK)
		{
			// � �������̿����� üũ�ؼ�, �´� ���·� ���ư���.
			if (m_iCurrState & IS_ATTACK1)
			{
				m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::WALK_FORWARD);
				m_iCurrState = MONSTER_STATE::TRACE;
			}
			else if (m_iCurrState & IS_ATTACK2)
			{
				if (m_iCurrState & MONSTER_STATE::ATTACK2_START)
				{
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::ATTACK);
					m_iCurrState = MONSTER_STATE::ATTACK2_CYCLE;
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK2_CYCLE)
				{
					m_iCurrState = MONSTER_STATE::ATTACK2_END;
				}
				else if (m_iCurrState & MONSTER_STATE::ATTACK2_END)
				{
					m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::WALK_FORWARD);
					m_iCurrState = MONSTER_STATE::TRACE;
				}
			}
		}
		// 6.3 ���Ͱ� HIT ��� �� �̿��ٸ�
		else if (m_iCurrState & IS_HIT)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::WALK_FORWARD);
			m_iCurrState = MONSTER_STATE::TRACE;
		}
		// 6.4 ���Ͱ� DETECTED ��� �� �̿��ٸ�
		else if (m_iCurrState & MONSTER_STATE::DETECTED)
		{
			m_wpAnimModel.lock()->SetAnimationIndex(HILI_ICE_ANIM::WALK_FORWARD);
			m_iCurrState = MONSTER_STATE::TRACE;
		}

		// �ݶ��̴� off
		SetAllAttackColliderOff();
	}
}

void CHili_Ice_StateMachine::MakeProjectile(_vector _vShootDirection, ATTACK_DESC _AttackDesc)
{
	// ������Ʈ ���̾ ���Ѵ�.
	shared_ptr<CLayer> spObjectLayer = m_wpOwnerActor.lock()->GetLevel().lock()->GetLayer(LAYER::LAYER_OBJECT);

	// ���� �ĺ��� �̸����� ����ü�� �����Ѵ�.
	shared_ptr<CProjectileActor> spProjectile = spObjectLayer->CreateActor<CProjectileActor>(_AttackDesc.wstrOwnerName + _AttackDesc.wstrAttackName);

	if (nullptr != spProjectile)
	{
		// ȸ�� ����
		_float fAngle = CaculateMonsterRotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), _vShootDirection);

		// ����ü�� �⺻ ������ �������ش�.
		shared_ptr<CProjectileActor> spProjectile = dynamic_pointer_cast<CProjectileActor>(spObjectLayer->FindActor(_AttackDesc.wstrOwnerName + _AttackDesc.wstrAttackName));

		// 1. ����ü ������ ����
		spProjectile->SetModelFileName(L"Cs_Item_Bullet_Arrow_01 (merge)");

		// 2. SRT ���� ����
		spProjectile->SetScale(XMVectorSet(300.0f, 300.0f, 300.0f, 0.0f));
		spProjectile->SetRotation(XMVectorSet(0.f, - fAngle, 0.f, 0.f));
		spProjectile->SetActorTranslation_NoNavigation(m_wpOwnerActor.lock()->GetActorTranslation());

		// 3. ����ü �ݶ��̴� ����
		spProjectile->SetAttackColliderType(COLLIDER_TYPE::SPHERE);
		spProjectile->SetColliderRadius(0.25f);
		spProjectile->SetAttackDesc(_AttackDesc);

		// 4. ����ü �߻� ���� ����
		spProjectile->SetShootDirrection(XMVector3Normalize(_vShootDirection));
		spProjectile->SetShootSpeed(25.0f);
		spProjectile->SetAcceleration(1.0f);
		spProjectile->SetLifeTime(5.0f);
		spProjectile->SetIsDeleted(true);

		spProjectile->Initialize();
	}
}

void CHili_Ice_StateMachine::MakeMultipleProjectile(_vector _vShootDirection, ATTACK_DESC _AttackDesc)
{
	_matrix matRotationY10 = XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMConvertToRadians(10.f));
	_matrix matRotationY20 = XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMConvertToRadians(20.f));
	_matrix matRotationMinusY10 = XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMConvertToRadians(-10.f));
	_matrix matRotationMinusY20 = XMMatrixRotationAxis(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), XMConvertToRadians(-20.f));

	_vector vMonsterToPlayerRot10 = XMVector3TransformNormal(_vShootDirection, matRotationY10);
	_vector vMonsterToPlayerRot20 = XMVector3TransformNormal(_vShootDirection, matRotationY20);
	_vector vMonsterToPlayerRotMinus10 = XMVector3TransformNormal(_vShootDirection, matRotationMinusY10);
	_vector vMonsterToPlayerRotMinus20 = XMVector3TransformNormal(_vShootDirection, matRotationMinusY20);

	m_Attack2_Desc.wstrAttackName = L"Hili_Ice Attack2";
	MakeProjectile(_vShootDirection, m_Attack2_Desc);

	m_Attack2_Desc.wstrAttackName = L"Hili_Ice Attack2_" + to_wstring(1);
	MakeProjectile(vMonsterToPlayerRot10, m_Attack2_Desc);

	m_Attack2_Desc.wstrAttackName = L"Hili_Ice Attack2_" + to_wstring(2);
	MakeProjectile(vMonsterToPlayerRot20, m_Attack2_Desc);

	m_Attack2_Desc.wstrAttackName = L"Hili_Ice Attack2_" + to_wstring(3);
	MakeProjectile(vMonsterToPlayerRotMinus10, m_Attack2_Desc);

	m_Attack2_Desc.wstrAttackName = L"Hili_Ice Attack2_" + to_wstring(4);
	MakeProjectile(vMonsterToPlayerRotMinus20, m_Attack2_Desc);
}
