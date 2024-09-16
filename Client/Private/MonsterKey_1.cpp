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

	// z�� ���⿡�� y���� �����ϰ� ����ȭ
	_vector vZ = XMVector3Normalize(XMVectorSet(0.f, 0.f, 1.f, 0.f));

	// ��ü -> �÷��̾� ���⺤�Ϳ��� y���� �����ϰ� ����ȭ
	vActorToPlayer = XMVector3Normalize(XMVectorSetY(vActorToPlayer, 0.0f));

	// �����Ͽ� ���� ���� ����
	_float fAngle = acos(XMVector3Dot(vActorToPlayer, vZ).m128_f32[0]) * (180.0f / DirectX::XM_PI);

	// Up ���Ϳ� vCamLook, vActorLook�� ������ ���͸� ��, ������ ���ٸ� ������ 180�� �����̴�.
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

	// �Ÿ��� ������ Ȱ��ȭ �˴ϴ�.
	if (650.f < XMVector3Length(vActorToPlayer).m128_f32[0])
		return;
	else
		m_wpMonsterKeyUI.lock()->SetEnable(false);

	if (m_bUse)
	{
		// 1. Ȱ��ȭ ������ ���� UI�� �����մϴ�.
		m_wpMonsterKeyUI.lock()->SetEnable(true);

		// 2. Ű �Է��� ������ Ʈ���Ÿ� �ߵ��ϰ� �ڽ��� ��Ȱ��ȭ �մϴ�.
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