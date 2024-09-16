#include "pch.h"
#include "MonsterKey_1.h"

#include "ObjectManager.h"
#include "InputManager.h"
#include "SoundManager.h"

#include "Level.h"
#include "Level_Stage1.h"
#include "Level_Stage2.h"
#include "Level_Stage3.h"
#include "Layer.h"

#include "MonsterKeyUI.h"
#include "Monster.h"

#include "AnimModel.h"

void CMonsterKey_1::Initialize()
{
	CEffectMeshActor::Initialize();
}

HRESULT CMonsterKey_1::Initialize(const wstring& _wstrJsonFileName)
{
	m_wpMonsterKeyUI = CreateActor<CMonsterKeyUI>(L"MonsterKeyUI");
	if (m_wpMonsterKeyUI.expired())
		return E_FAIL;

	m_wpMonsterKeyUI.lock()->SetShaderFileName(L"Shader_UIBase");
	m_wpMonsterKeyUI.lock()->SetTextureFileName(L"UI_Img_AkaFesAkaFes_RewardBtn_Bg");
	m_wpMonsterKeyUI.lock()->SetSize({ 256.f, 40.f });
	m_wpMonsterKeyUI.lock()->SetEnable(false);

	return CEffectMeshActor::Initialize(_wstrJsonFileName);
}

void CMonsterKey_1::RecycleBegin()
{
	CEffectMeshActor::RecycleBegin();
}

_int CMonsterKey_1::PreUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::PreUpdate(_fTimeDelta);
}

_int CMonsterKey_1::Update(const _float& _fTimeDelta)
{
	CheckTrigger();

	return CEffectMeshActor::Update(_fTimeDelta);
}

_int CMonsterKey_1::LateUpdate(const _float& _fTimeDelta)
{
	LookForPlayer();

	return CEffectMeshActor::LateUpdate(_fTimeDelta);
}

void CMonsterKey_1::Render()
{
	CEffectMeshActor::Render();
}

void CMonsterKey_1::EndPlay()
{
	CEffectMeshActor::EndPlay();
}

void CMonsterKey_1::Release()
{
	CEffectMeshActor::Release();
}

void CMonsterKey_1::LookForPlayer()
{
	shared_ptr<CActor> spPlayer = CObjectManager::GetInstance()->GetCurPlayer();
	_vector vPlayerPos = spPlayer->GetActorTranslation();

	_vector vActorToPlayer = vPlayerPos - GetActorTranslation();

	// z축 방향에서 y성분 제거하고 정규화
	_vector vZ = XMVector3Normalize(XMVectorSet(0.f, 0.f, 1.f, 0.f));

	// 객체 -> 플레이어 방향벡터에서 y성분 제거하고 정규화
	vActorToPlayer = XMVector3Normalize(XMVectorSetY(vActorToPlayer, 0.0f));

	// 내적하여 라디안 각도 도출
	_float fAngle = acos(XMVector3Dot(vActorToPlayer, vZ).m128_f32[0]) * (180.0f / DirectX::XM_PI);

	// Up 벡터와 vCamLook, vActorLook을 외적한 벡터를 비교, 방향이 같다면 각도는 180도 이하이다.
	if (XMVectorGetY(XMVector3Cross(vZ, vActorToPlayer)) > 0)
		fAngle = 360.0f - fAngle;

	SetRotation(XMVectorSet(0.f, -fAngle, 0.f, 0.f));
}

void CMonsterKey_1::CheckTrigger()
{
	shared_ptr<CLevel> spCurLevel = m_wpLevel.lock();

	shared_ptr<CActor> spPlayer = CObjectManager::GetInstance()->GetCurPlayer();
	_vector vPlayerPos = spPlayer->GetActorTranslation();

	_vector vActorToPlayer = vPlayerPos - GetActorTranslation();

	// 거리가 가까우면 활성화 됩니다.
	if (650.f < XMVector3Length(vActorToPlayer).m128_f32[0])
		return;
	else
		m_wpMonsterKeyUI.lock()->SetEnable(false);

	if (m_bUse)
	{
		// 1. 활성화 문구를 띄우는 UI를 렌더합니다.
		m_wpMonsterKeyUI.lock()->SetEnable(true);

		// 2. 키 입력을 받으면 트리거를 발동하고 자신을 비활성화 합니다.
		if (CInputManager::GetInstance()->IsKeyDown(DIK_F))
		{
			if (1 == m_iLevelIndex)
			{
				shared_ptr<CLevel_Stage1> spStage1 = dynamic_pointer_cast<CLevel_Stage1>(spCurLevel);
				spStage1->Stage1_SpawnWave1();
				CSoundManager::GetInstance()->PlaySoundEx("Trigger_Click.mp3", EFFECT_1, 0.2f);
			}
			else if (2 == m_iLevelIndex)
			{
				shared_ptr<CLevel_Stage2> spStage2 = dynamic_pointer_cast<CLevel_Stage2>(spCurLevel);
				spStage2->Stage2_SpawnWave1();
				CSoundManager::GetInstance()->PlaySoundEx("Trigger_Click.mp3", EFFECT_1, 0.2f);
			}

			m_bUse = false;

			shared_ptr<CActor> spKey = CObjectManager::GetInstance()->GetCurLevel()->FindLayer(LAYER_OBJECT)->FindActor(L"MonsterKey_2");
			if (nullptr != spKey)
				spKey->SetActorUse(false);

			spKey = CObjectManager::GetInstance()->GetCurLevel()->FindLayer(LAYER_OBJECT)->FindActor(L"MonsterKey_3");
			if (nullptr != spKey)
				spKey->SetActorUse(false);

			spKey = CObjectManager::GetInstance()->GetCurLevel()->FindLayer(LAYER_OBJECT)->FindActor(L"MonsterKey_4");
			if (nullptr != spKey)
				spKey->SetActorUse(false);
		}
	}
}