#include "Engine_pch.h"
#include "StateMachine.h"

#include "Charactor.h"

#include "AnimModel.h"

void CStateMachine::Initialize()
{
	CActorComponent::Initialize();
}

_int CStateMachine::PreUpdate(const _float& _fTimeDelta)
{
	return CActorComponent::PreUpdate(_fTimeDelta);
}

_int CStateMachine::Update(const _float& _fTimeDelta)
{
	return CActorComponent::Update(_fTimeDelta);
}

_int CStateMachine::LateUpdate(const _float& _fTimeDelta)
{
	return CActorComponent::LateUpdate(_fTimeDelta);
}

void CStateMachine::Render()
{
	CActorComponent::Render();
}

void CStateMachine::EndPlay()
{
	CActorComponent::EndPlay();
}

void CStateMachine::Release()
{
	CActorComponent::Release();
}

_uint CStateMachine::Hitted(ATTACK_DESC AttackDesc, _float _fDamage)
{
	shared_ptr<CCharactor> spChractor = dynamic_pointer_cast<CCharactor>(m_wpOwnerActor.lock());

	_float fCurHp = spChractor->GetCurHp();
	_float fDef = spChractor->GetDef();

	spChractor->SetCurHp(fCurHp - (_fDamage - fDef));

	return _uint(_fDamage - fDef);
}
