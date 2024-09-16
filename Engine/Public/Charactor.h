#pragma once
#include "AnimActor.h"

/* Charactor 클래스는 AnimActor들 중에서 StateMachine을 통해 움직이는 객체들을 위한 클래스입니다. */
/* 기본적으로 이런 객체들은 충돌처리를 필요로 하기 때문에, 본인의 Collider를 소유하고 태어납니다. */
/* 스테이트 머신은 추상 클래스이기 때문에 각자 자식클래스 들에서 상속을 내린 스테이트 머신으로 Create시켜야 합니다. */

BEGIN(Engine)

class CStateMachine;
class CCollider;

class ENGINE_DLL CCharactor : public CAnimActor
{
public:
	explicit CCharactor() = default;
	virtual ~CCharactor() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize();
	virtual _int PreUpdate(const _float& _fTimeDelta);
	virtual _int Update(const _float& _fTimeDelta);
	virtual _int LateUpdate(const _float& _fTimeDelta);
	virtual void Render();
	virtual void EndPlay();
	virtual void Release();

/********************
	Methods
********************/
protected:
	virtual HRESULT ReadyParticle(const wstring& _wstrJsonFileName, _uint _iParticleNum) { return S_OK; }
	virtual HRESULT ReadyEffectMesh(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum) { return S_OK; }

/********************
	Getter/Setter
********************/
public:
	shared_ptr<CStateMachine> GetStateMachine() { return m_wpStateMachine.lock(); }
	shared_ptr<CCollider> GetCollider() { return m_wpCollider.lock(); }

public:
	void SetMaxHp(_float _fMaxHp) { m_fMaxHp = _fMaxHp; }
	_float GetMaxHp() { return m_fMaxHp; }

	void SetCurHp(_float _fCurHp) { m_fCurHp = _fCurHp; }
	_float GetCurHp() { return m_fCurHp; }

	void SetAtk(_float _fAtk) { m_fAtk = _fAtk; }
	_float GetAtk() { return m_fAtk; }

	void SetDef(_float _fDef) { m_fDef = _fDef; }
	_float GetDef() { return m_fDef; }

	void SetMoveSpeed(_float _fMoveSpeed) { m_fMoveSpeed = _fMoveSpeed; }
	_float GetMoveSpeed() { return m_fMoveSpeed; }

/********************
	Data Members
********************/
protected:
	// [ 스테이트 머신 ]
	weak_ptr<CStateMachine> m_wpStateMachine;

	// [ 콜라이더 ]
	weak_ptr<CCollider> m_wpCollider;

protected:
	/* 전투 능력치 */
	_float m_fMaxHp = 0.0f;
	_float m_fCurHp	= 0.0f;
	_float m_fAtk	= 0.0f;
	_float m_fDef	= 0.0f;
	_float m_fMoveSpeed = 0.0f;
};

END