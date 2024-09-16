#include "pch.h"
#include "MonsterKey_4.h"

#include "ObjectManager.h"

void CMonsterKey_4::Initialize()
{
	CEffectMeshActor::Initialize();
}

HRESULT CMonsterKey_4::Initialize(const wstring& _wstrJsonFileName)
{
	return CEffectMeshActor::Initialize(_wstrJsonFileName);
}

void CMonsterKey_4::RecycleBegin()
{
	CEffectMeshActor::RecycleBegin();
}

_int CMonsterKey_4::PreUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::PreUpdate(_fTimeDelta);
}

_int CMonsterKey_4::Update(const _float& _fTimeDelta)
{
	return CEffectMeshActor::Update(_fTimeDelta);
}

_int CMonsterKey_4::LateUpdate(const _float& _fTimeDelta)
{
	return CEffectMeshActor::LateUpdate(_fTimeDelta);
}

void CMonsterKey_4::Render()
{
	CEffectMeshActor::Render();
}

void CMonsterKey_4::EndPlay()
{
	CEffectMeshActor::EndPlay();
}

void CMonsterKey_4::Release()
{
	CEffectMeshActor::Release();
}

void CMonsterKey_4::LookForPlayer()
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