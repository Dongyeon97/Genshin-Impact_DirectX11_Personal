#include "pch.h"
#include "DungeonExit.h"

#include "ObjectManager.h"

#include "DungeonExitUI.h"

void CDungeonExit::Initialize()
{
	CEffectMeshActor::Initialize();
}

HRESULT CDungeonExit::Initialize(const wstring& _wstrJsonFileName)
{
	m_wpDungeonExitUI = CreateActor<CDungeonExitUI>(L"DungeonExitUI");
	if (m_wpDungeonExitUI.expired())
		return E_FAIL;

	m_wpDungeonExitUI.lock()->SetShaderFileName(L"Shader_UIBase");
	m_wpDungeonExitUI.lock()->SetTextureFileName(L"UI_Img_AkaFesAkaFes_RewardBtn_Bg");
	m_wpDungeonExitUI.lock()->SetSize({ 256.f, 40.f });
	m_wpDungeonExitUI.lock()->SetCurStageIndex(m_iLevelIndex);
	m_wpDungeonExitUI.lock()->SetEnable(false);

	return CEffectMeshActor::Initialize(_wstrJsonFileName);
}

void CDungeonExit::RecycleBegin()
{
	CEffectMeshActor::RecycleBegin();
}

_int CDungeonExit::PreUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::PreUpdate(_fTimeDelta);
}

_int CDungeonExit::Update(const _float& _fTimeDelta)
{
	CheckTrigger();

	return CEffectMeshActor::Update(_fTimeDelta);
}

_int CDungeonExit::LateUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::LateUpdate(_fTimeDelta);
}

void CDungeonExit::Render()
{
	CEffectMeshActor::Render();
}

void CDungeonExit::EndPlay()
{
	CEffectMeshActor::EndPlay();
}

void CDungeonExit::Release()
{
	CEffectMeshActor::Release();
}

void CDungeonExit::CheckTrigger()
{
	shared_ptr<CActor> spPlayer = CObjectManager::GetInstance()->GetCurPlayer();
	_vector vPlayerPos = spPlayer->GetActorTranslation();

	_vector vActorToPlayer = vPlayerPos - GetActorTranslation();

	// 거리가 가까우면 활성화 됩니다.
	if (650.f < XMVector3Length(vActorToPlayer).m128_f32[0])
		return;
	else
		m_wpDungeonExitUI.lock()->SetEnable(false);

	if (m_bUse)
	{
		// 1. 활성화 문구를 띄우는 UI를 렌더합니다.
		m_wpDungeonExitUI.lock()->SetEnable(true);
	}
}