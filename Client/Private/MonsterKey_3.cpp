#include "pch.h"
#include "MonsterKey_3.h"

#include "ObjectManager.h"

void CMonsterKey_3::Initialize()
{
	CEffectMeshActor::Initialize();
}

HRESULT CMonsterKey_3::Initialize(const wstring& _wstrJsonFileName)
{
	return CEffectMeshActor::Initialize(_wstrJsonFileName);
}

void CMonsterKey_3::RecycleBegin()
{
	CEffectMeshActor::RecycleBegin();
}

_int CMonsterKey_3::PreUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::PreUpdate(_fTimeDelta);
}

_int CMonsterKey_3::Update(const _float& _fTimeDelta)
{
	return CEffectMeshActor::Update(_fTimeDelta);
}

_int CMonsterKey_3::LateUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::LateUpdate(_fTimeDelta);
}

void CMonsterKey_3::Render()
{
	CEffectMeshActor::Render();
}

void CMonsterKey_3::EndPlay()
{
	CEffectMeshActor::EndPlay();
}

void CMonsterKey_3::Release()
{
	CEffectMeshActor::Release();
}

void CMonsterKey_3::LookForPlayer()
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