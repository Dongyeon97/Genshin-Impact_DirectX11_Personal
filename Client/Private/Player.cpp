#include "pch.h"
#include "Player.h"
#include "PlayerStateDefine.h"
#include "PlayerStateMachine.h"

#include "RenderManager.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "LightManager.h"
#include "PrefabManager.h"
#include "SpawnManager.h"
#include "NavigationManager.h"
#include "FontManager.h"

#include "Level.h"
#include "Layer.h"

#include "UserInstance.h"
#include "ParticleActor.h"
#include "EffectMeshActor.h"
#include "SkillIcon.h"
#include "SpriteActor.h"

#include "Navigation.h"
#include "ProductionCamera.h"
#include "AnimModel.h"
#include "Weapon.h"
#include "Animation.h"
#include "ColliderComponent.h"
#include "AttackCollider.h"

void CPlayer::Initialize()
{
	CCharactor::Initialize();

	// [ Component - Parts ]
	ReadyBoneMatrixToWeapon();

	SetHPBarEnable(true);
}

_int CPlayer::PreUpdate(const _float& _fTimeDelta)
{
	return CCharactor::PreUpdate(_fTimeDelta);
}

_int CPlayer::Update(const _float& _fTimeDelta)
{
	KeyInput(_fTimeDelta);
	MouseHold(_fTimeDelta);
	BindCoolTimeToSkillIcon();

	return CCharactor::Update(_fTimeDelta);
}

_int CPlayer::LateUpdate(const _float& _fTimeDelta)
{
	UpdateCollidersWorldMatrix();

	// m_pDeviceContext->PSSetConstantBuffers()

	return CCharactor::LateUpdate(_fTimeDelta);
}

void CPlayer::Render()
{
	CCharactor::Render();

	m_wpWeapon.lock()->Render(0);
}

void CPlayer::EndPlay()
{
	CCharactor::EndPlay();
}

void CPlayer::Release()
{
	if(!m_wpCollider.expired())
		CCollisionManager::GetInstance()->RemovePlayerBodyCollider(m_wpCollider);

	if (!m_wpSkillCollider.expired())
		CCollisionManager::GetInstance()->RemovePlayerAttackCollider(m_wpSkillCollider);

	CCharactor::Release();
}

void CPlayer::KeyInput(const _float& _fTimeDelta)
{
	if (!CInputManager::GetInstance()->GetMouseHold()) { return; }

#pragma region 키입력 정리
	/* 키보드 */
	_bool W_Down = CInputManager::GetInstance()->IsKeyDown(DIK_W);
	_bool A_Down = CInputManager::GetInstance()->IsKeyDown(DIK_A);
	_bool S_Down = CInputManager::GetInstance()->IsKeyDown(DIK_S);
	_bool D_Down = CInputManager::GetInstance()->IsKeyDown(DIK_D);

	_bool E_Down = CInputManager::GetInstance()->IsKeyDown(DIK_E);
	_bool Q_Down = CInputManager::GetInstance()->IsKeyDown(DIK_Q);
	_bool Shift_Down = CInputManager::GetInstance()->IsKeyDown(DIK_LSHIFT);
	_bool Space_Down = CInputManager::GetInstance()->IsKeyDown(DIK_SPACE);

	_bool W_Press = CInputManager::GetInstance()->IsKeyPressed(DIK_W);
	_bool A_Press = CInputManager::GetInstance()->IsKeyPressed(DIK_A);
	_bool S_Press = CInputManager::GetInstance()->IsKeyPressed(DIK_S);
	_bool D_Press = CInputManager::GetInstance()->IsKeyPressed(DIK_D);

	_bool E_Press = CInputManager::GetInstance()->IsKeyPressed(DIK_E);
	_bool Q_Press = CInputManager::GetInstance()->IsKeyPressed(DIK_Q);
	_bool Shift_Press = CInputManager::GetInstance()->IsKeyPressed(DIK_LSHIFT);
	_bool Space_Press = CInputManager::GetInstance()->IsKeyPressed(DIK_SPACE);

	_bool W_Released = CInputManager::GetInstance()->IsKeyReleased(DIK_W);
	_bool A_Released = CInputManager::GetInstance()->IsKeyReleased(DIK_A);
	_bool S_Released = CInputManager::GetInstance()->IsKeyReleased(DIK_S);
	_bool D_Released = CInputManager::GetInstance()->IsKeyReleased(DIK_D);

	_bool E_Released = CInputManager::GetInstance()->IsKeyReleased(DIK_E);
	_bool Q_Released = CInputManager::GetInstance()->IsKeyReleased(DIK_Q);
	_bool Shift_Released = CInputManager::GetInstance()->IsKeyReleased(DIK_LSHIFT);
	_bool Space_Released = CInputManager::GetInstance()->IsKeyReleased(DIK_SPACE);

	_bool W_Up = CInputManager::GetInstance()->IsKeyUp(DIK_W);
	_bool A_Up = CInputManager::GetInstance()->IsKeyUp(DIK_A);
	_bool S_Up = CInputManager::GetInstance()->IsKeyUp(DIK_S);
	_bool D_Up = CInputManager::GetInstance()->IsKeyUp(DIK_D);

	_bool E_Up = CInputManager::GetInstance()->IsKeyUp(DIK_E);
	_bool Q_Up = CInputManager::GetInstance()->IsKeyUp(DIK_Q);
	_bool Shift_Up = CInputManager::GetInstance()->IsKeyUp(DIK_LSHIFT);
	_bool Space_Up = CInputManager::GetInstance()->IsKeyUp(DIK_SPACE);

	/* 마우스 */
	_bool MOUSE_DOWN_L = CInputManager::GetInstance()->IsMouseDown(EMouseButtonType::LEFT_BUTTON);
	_bool MOUSE_PRESS_L = CInputManager::GetInstance()->IsMousePressed(EMouseButtonType::LEFT_BUTTON);
	_bool MOUSE_UP_L = CInputManager::GetInstance()->IsMouseUp(EMouseButtonType::LEFT_BUTTON);
	_bool MOUSE_RELEASE_L = CInputManager::GetInstance()->IsMouseReleased (EMouseButtonType::LEFT_BUTTON);
#pragma endregion 키입력 정리

#pragma region 움직임

	// 이동
	if (W_Press && A_Press)
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::RUN_CYCLE, REQUEST_DIR::FOWARD_LEFT);
		m_eRequestDir = REQUEST_DIR::FOWARD_LEFT;
	}
	else if (W_Press && D_Press)
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::RUN_CYCLE, REQUEST_DIR::FOWARD_RIGHT);
		m_eRequestDir = REQUEST_DIR::FOWARD_RIGHT;
	}
	else if (S_Press && A_Press)
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::RUN_CYCLE, REQUEST_DIR::BACK_LEFT);
		m_eRequestDir = REQUEST_DIR::BACK_LEFT;
	}
	else if (S_Press && D_Press)
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::RUN_CYCLE, REQUEST_DIR::BACK_RIGHT);
		m_eRequestDir = REQUEST_DIR::BACK_RIGHT;
	}
	else if (W_Press)
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::RUN_CYCLE, REQUEST_DIR::FOWARD);
		m_eRequestDir = REQUEST_DIR::FOWARD;
	}
	else if (A_Press)
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::RUN_CYCLE, REQUEST_DIR::LEFT);
		m_eRequestDir = REQUEST_DIR::LEFT;
	}
	else if (S_Press)
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::RUN_CYCLE, REQUEST_DIR::BACK);
		m_eRequestDir = REQUEST_DIR::BACK;
	}
	else if (D_Press)
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::RUN_CYCLE, REQUEST_DIR::RIGHT);
		m_eRequestDir = REQUEST_DIR::RIGHT;
	}

	// 대쉬
	if (Shift_Down)
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::SPRINT_CYCLE);
	}

	// 점프
	if (Space_Down && m_wpStateMachine.lock()->GetCurState() & PLAYER_STATE::IS_WALK)
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::JUMP_FOR_WALK);
	else if (Space_Down && m_wpStateMachine.lock()->GetCurState() & PLAYER_STATE::IS_RUN)
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::JUMP_FOR_RUN);
	else if (Space_Down && m_wpStateMachine.lock()->GetCurState() & PLAYER_STATE::IS_SPRINT)
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::JUMP_FOR_SPRINT);
	else if (Space_Down && m_wpStateMachine.lock()->GetCurState() == PLAYER_STATE::STANDBY)
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::JUMP);
	// 활공
	else if (Space_Down && m_wpStateMachine.lock()->GetCurState() & PLAYER_STATE::IS_JUMP)
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::FLY_START);

	// 모든 방향키가 눌려있지 않으면 STANDBY를 요청
	if (W_Released && A_Released && S_Released && D_Released)
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::STANDBY);

#pragma endregion 움직임

#pragma region 공격 및 스킬

	// 평타
	if (MOUSE_DOWN_L)
	{
		_ullong State = m_wpStateMachine.lock()->GetCurState();

		switch (State)
		{
		case PLAYER_STATE::ATTACK_1:
		{
			m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::ATTACK_2, m_eRequestDir);
			break;
		}
		case PLAYER_STATE::ATTACK_2:
		{
			m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::ATTACK_3, m_eRequestDir);
			break;
		}
		case PLAYER_STATE::ATTACK_3:
		{
			m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::ATTACK_4, m_eRequestDir);
			break;
		}
		case PLAYER_STATE::ATTACK_4:
		{
			m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::ATTACK_5, m_eRequestDir);
			break;
		}
		default:
			m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::ATTACK_1, m_eRequestDir);
			break;
		}
	}

	if (E_Down)
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::ELEMENTAL_CLICK);
	}
	if (Q_Down)
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::ELEMENTAL_BURST);
	}

#pragma endregion 공격 및 스킬

#pragma region 기타

	if (0 != CInputManager::GetInstance()->GetMouseDeltaZ())
	{
		shared_ptr<CCameraComponent> spCam = CCameraManager::GetInstance()->GetCurCamera();
		if (L"ShoulderViewCameraComponent" == spCam->GetComponentName())
			return;

		shared_ptr<CProductionCamera> spProductionCam = dynamic_pointer_cast<CProductionCamera>(spCam);

		_float fRadius = spProductionCam->GetRadius();
		if (150.f < fRadius && fRadius < 600.0f)
		{
			// 마우스 휠 줌인 줌아웃
			_int iMouseWheelDelta = CInputManager::GetInstance()->GetMouseDeltaZ();

			_float iWheelRatio = (-1.0f * iMouseWheelDelta) / 2.0f;

			if (fRadius + iWheelRatio <= 150.0f || 600.0f <= fRadius + iWheelRatio)
				return;

			spProductionCam->SetRadius(lerp(fRadius, fRadius + iWheelRatio, 0.5f));
		}
	}

#pragma endregion 기타
}
void CPlayer::MouseHold(const _float& _fTimeDelta)
{
	if (CInputManager::GetInstance()->IsKeyDown(DIK_F2))
	{
		m_bMouseHold = !m_bMouseHold;

		CInputManager::GetInstance()->SetMouseHold(m_bMouseHold);
	}
}
void CPlayer::BindCoolTimeToSkillIcon()
{
	if (m_vecSkillIcons.empty())
		return;

	shared_ptr<CPlayerStateMachine> spStateMachine = dynamic_pointer_cast<CPlayerStateMachine>(GetStateMachine());

	_float2 fSkillCoolTime_E = spStateMachine->GetSkillCoolTime_E();
	_float2 fSkillCoolTime_Q = spStateMachine->GetSkillCoolTime_Q();

	m_vecSkillIcons[0]->SetSkillCoolTime(fSkillCoolTime_E);
	m_vecSkillIcons[1]->SetSkillCoolTime(fSkillCoolTime_Q);
}

void CPlayer::ReadyBoneMatrixToWeapon()
{
	m_pSocketMatrix_RightArm = m_wpAnimModel.lock()->GetCombinedBoneMatrixPointer("WeaponR");		// 오른손
	m_pSocketMatrix_LeftArm = m_wpAnimModel.lock()->GetCombinedBoneMatrixPointer("WeaponL");		// 오른손
	m_pSocketMatrix_Chest = m_wpAnimModel.lock()->GetCombinedBoneMatrixPointer("Bip001_Spine2");	// 위쪽 가슴

	m_wpWeapon.lock()->SetSocketMatrix(m_pSocketMatrix_Chest);
}
void CPlayer::BindWeaponToChest()
{
	m_wpWeapon.lock()->SetRotation(XMVectorSet(-90.0f, 90.0f, 100.f, 0.0f));

	m_wpWeapon.lock()->SetActorTranslation(XMVectorSet(25.0f, -50.0f, 0.0f, 1.0f));
	m_wpWeapon.lock()->SetSocketMatrix(m_pSocketMatrix_Chest);
}
void CPlayer::BindWeaponToLeftArm()
{
	m_wpWeapon.lock()->SetActorTranslation(XMVectorSet(0.0f, 0.0f, 0.f, 1.0f));

	_vector vTransformCopy = m_wpWeapon.lock()->GetTransformCopy();

	vTransformCopy = XMVectorSetX(vTransformCopy, XMVectorGetX(vTransformCopy) - 90.f);
	vTransformCopy = XMVectorSetZ(vTransformCopy, XMVectorGetZ(vTransformCopy) + 90.f);

	m_wpWeapon.lock()->SetRotation(vTransformCopy);
	m_wpWeapon.lock()->SetSocketMatrix(m_pSocketMatrix_LeftArm);
}
void CPlayer::BindWeaponToRightArm()
{
	m_wpWeapon.lock()->SetActorTranslation(XMVectorSet(0.0f, 0.0f, 0.f, 1.0f));

	_vector vTransformCopy = m_wpWeapon.lock()->GetTransformCopy();

	vTransformCopy = XMVectorSetX(vTransformCopy, XMVectorGetX(vTransformCopy) - 90.f);
	vTransformCopy = XMVectorSetZ(vTransformCopy, XMVectorGetZ(vTransformCopy) + 90.f);

	m_wpWeapon.lock()->SetRotation(vTransformCopy);
	m_wpWeapon.lock()->SetSocketMatrix(m_pSocketMatrix_RightArm);
}

HRESULT CPlayer::ReadyParticle(const wstring& _wstrJsonFileName, _uint _iParticleNum)
{
	shared_ptr<CParticleActor> spParticleActor = GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->CreateActorToPool<CParticleActor>(_wstrJsonFileName + to_wstring(_iParticleNum));
	if (nullptr == spParticleActor)
	{
		MSG_BOX("Failed to Create Particle : CPlayer");
		return E_FAIL;
	}

	spParticleActor->SetJsonFileName(_wstrJsonFileName);

	if (FAILED(spParticleActor->Initialize(_wstrJsonFileName)))
	{
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spParticleActor->GetActorName());
	}

	return S_OK;
}
HRESULT CPlayer::ReadySpriteEffect(const wstring& _wstrJsonFileName, _uint _iEffectsNum, _uint _iRow, _uint _iCol, _float _fSpeedRatio)
{
	shared_ptr<CSpriteActor> spSpriteActor = GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->CreateActorToPool<CSpriteActor>(_wstrJsonFileName + to_wstring(_iEffectsNum));
	if (nullptr == spSpriteActor)
	{
		MSG_BOX("Failed to Create HitEffects : CUserInstance");
		return E_FAIL;
	}

	spSpriteActor->SetJsonFileName(_wstrJsonFileName);
	spSpriteActor->SetRow(_iRow);
	spSpriteActor->SetCol(_iCol);
	spSpriteActor->SetSpeedRatio(_fSpeedRatio);

	if (FAILED(spSpriteActor->Initialize(_wstrJsonFileName)))
	{
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spSpriteActor->GetActorName());
	}

	return S_OK;
}
HRESULT CPlayer::ReadyEffectMesh(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum)
{
	shared_ptr<CEffectMeshActor> spEffectMeshActor = GetLevel().lock()->GetLayer(LAYER::LAYER_EFFECTMESH)->CreateActorToPool<CEffectMeshActor>(_wstrJsonFileName + to_wstring(_iEffectMeshNum));
	if (nullptr == spEffectMeshActor)
		return E_FAIL;

	spEffectMeshActor->SetJsonFileName(_wstrJsonFileName);

	if (FAILED(spEffectMeshActor->Initialize(_wstrJsonFileName)))
	{
		GetLevel().lock()->GetLayer(LAYER::LAYER_PARTICLE)->RemoveActor_Pool(spEffectMeshActor->GetActorName());
	}
	
	return S_OK;
}

_int CPlayer::OnCollisionEnter(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
	if (dynamic_pointer_cast<CActor>(shared_from_this()) == _wpDstCollider.lock()->GetOwnerActor().lock())
		return 0;

	if (!m_wpOwnerActor.expired())
	{
		shared_ptr<CUserInstance> spUserInstance = dynamic_pointer_cast<CUserInstance>(m_wpOwnerActor.lock());

		// 쉴드 중일 때는 공격받지 않는다.
		if (spUserInstance->GetIsShield())
			return 0;
	}

	switch (AttackDesc.eKnockbackType)
	{
		// 1. 약 공격(경직)
	case KNOCKBACK_TYPE::WEAK:
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::HIT_L, REQUEST_DIR::DIR_NONE, AttackDesc);
		break;
	}
	case KNOCKBACK_TYPE::STRONG:
	{
		// 2. 강 공격(밀침)
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::HIT_H, REQUEST_DIR::DIR_NONE, AttackDesc);
		break;
	}
		// 3. 다운 공격
	case KNOCKBACK_TYPE::POWERFUL:
	{
		m_wpStateMachine.lock()->RequestChangeState(PLAYER_STATE::HIT_FALLTOGROUND, REQUEST_DIR::DIR_NONE, AttackDesc);
		break;
	}
	}

	return 0;
}
void CPlayer::OnCollisionStay(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
}
void CPlayer::OnCollisionExit(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
}
void CPlayer::SetHPBarEnable(_bool _bEnable)
{
	for (auto& iter : m_vecHPbar)
		iter->SetEnable(_bEnable);
}
void CPlayer::SetActorTranslation(_fvector _vPosition, _int* _iCurCellIndex)
{
	shared_ptr<CNavigation> spNavi = CNavigationManager::GetInstance()->GetCurNavigation();

	_float fPlaneY = 0.0f;

	if (nullptr == spNavi)
	{
		XMStoreFloat4(&m_vTranslate, _vPosition);
		return;
	}
	else if (true == spNavi->IsMove(_vPosition, _iCurCellIndex, &fPlaneY))
	{
		_float4 vFinalPosition;
		XMStoreFloat4(&vFinalPosition, _vPosition);

		if (PLAYER_STATE::IS_JUMP & m_wpStateMachine.lock()->GetCurState() || PLAYER_STATE::IS_FLYING & m_wpStateMachine.lock()->GetCurState() || PLAYER_STATE::IS_CLIMB & m_wpStateMachine.lock()->GetCurState())
		{
		}
		else
			vFinalPosition.y = fPlaneY;

		XMStoreFloat4(&m_vTranslate, XMLoadFloat4(&vFinalPosition));
	}
}

void CPlayer::UpdateCollidersWorldMatrix()
{
	if(!m_wpCollider.expired())
		m_wpCollider.lock()->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));

	if (!m_wpSkillCollider.expired())
		m_wpSkillCollider.lock()->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));

//#ifdef _DEBUG
//	if (!m_wpCollider.expired())
//		CRenderManager::GetInstance()->AddDebugComponent(m_wpCollider.lock());
//
//	if (!m_wpSkillCollider.expired())
//		CRenderManager::GetInstance()->AddDebugComponent(m_wpSkillCollider.lock());
//#endif // _DEBUG
}
void CPlayer::RemovePlayerBodyCollider()
{
	if(!m_wpCollider.expired())
		CCollisionManager::GetInstance()->RemovePlayerBodyCollider(m_wpCollider);
}
void CPlayer::AddPlayerBodyCollider()
{
	if (!m_wpCollider.expired())
		CCollisionManager::GetInstance()->AddPlayerBodyCollider(m_wpCollider);
}
void CPlayer::SetStandby()
{
	if (!m_wpAnimModel.expired())
	{
		m_wpAnimModel.lock()->SetAnimationIndex(ANIM::STANDBY);
		m_wpStateMachine.lock()->SetCurState(PLAYER_STATE::STANDBY);
	}
}