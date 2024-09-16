#include "pch.h"
#include "MonsterKey_2.h"

#include "ObjectManager.h"

void CMonsterKey_2::Initialize()
{
	CEffectMeshActor::Initialize();
}

HRESULT CMonsterKey_2::Initialize(const wstring& _wstrJsonFileName)
{
	return CEffectMeshActor::Initialize(_wstrJsonFileName);
}

void CMonsterKey_2::RecycleBegin()
{
	CEffectMeshActor::RecycleBegin();
}

_int CMonsterKey_2::PreUpdate(const _float& _fTimeDelta)
{
	LookForPlayer();

	return CEffectMeshActor::PreUpdate(_fTimeDelta);
}

_int CMonsterKey_2::Update(const _float& _fTimeDelta)
{
	return CEffectMeshActor::Update(_fTimeDelta);
}

_int CMonsterKey_2::LateUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::LateUpdate(_fTimeDelta);
}

void CMonsterKey_2::Render()
{
	CEffectMeshActor::Render();
}

void CMonsterKey_2::EndPlay()
{
	CEffectMeshActor::EndPlay();
}

void CMonsterKey_2::Release()
{
	CEffectMeshActor::Release();
}

void CMonsterKey_2::LookForPlayer()
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