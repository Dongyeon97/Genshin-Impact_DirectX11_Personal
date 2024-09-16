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