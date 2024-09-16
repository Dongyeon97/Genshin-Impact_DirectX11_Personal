#pragma once
#include "AnimActor.h"

/* Charactor Ŭ������ AnimActor�� �߿��� StateMachine�� ���� �����̴� ��ü���� ���� Ŭ�����Դϴ�. */
/* �⺻������ �̷� ��ü���� �浹ó���� �ʿ�� �ϱ� ������, ������ Collider�� �����ϰ� �¾�ϴ�. */
/* ������Ʈ �ӽ��� �߻� Ŭ�����̱� ������ ���� �ڽ�Ŭ���� �鿡�� ����� ���� ������Ʈ �ӽ����� Create���Ѿ� �մϴ�. */

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
	// [ ������Ʈ �ӽ� ]
	weak_ptr<CStateMachine> m_wpStateMachine;

	// [ �ݶ��̴� ]
	weak_ptr<CCollider> m_wpCollider;

protected:
	/* ���� �ɷ�ġ */
	_float m_fMaxHp = 0.0f;
	_float m_fCurHp	= 0.0f;
	_float m_fAtk	= 0.0f;
	_float m_fDef	= 0.0f;
	_float m_fMoveSpeed = 0.0f;
};

END