#pragma once
#include "Charactor.h"
#include "MonsterStateDefine.h"

BEGIN(Engine)
class CCollider;
END

/* == 몬스터 정보 구조체 == */
typedef struct
{
	MONSTER_TYPE m_eMonsterType;		// 몬스터 타입(일반,중간,중간보스,보스)
	HIT_TYPE m_eHitType;				// 피격 타입(일반,수퍼아머,무적)
	ELEMENTAL_TYPE m_eElementalType;	// 속성

	_float m_fDetectingRadius;			// 인식범위
#pragma region Attack
	_float m_fAttack1Radius;			// Attack1 공격범위
	_float m_fAttack2Radius;			// Attack2 공격범위
	_float m_fAttack3Radius;			// Attack3 공격범위
	_float m_fAttack4Radius;			// Attack4 공격범위
	_float m_fAttack5Radius;			// Attack5 공격범위

	_float m_fAttack1RunTime;			// Attack1 진행시간
	_float m_fAttack2RunTime;			// Attack2 진행시간
	_float m_fAttack3RunTime;			// Attack3 진행시간
	_float m_fAttack4RunTime;			// Attack4 진행시간
	_float m_fAttack5RunTime;			// Attack5 진행시간

	_float m_fAttack1CoolTime;			// Attack1 쿨타임
	_float m_fAttack2CoolTime;			// Attack2 쿨타임
	_float m_fAttack3CoolTime;			// Attack3 쿨타임
	_float m_fAttack4CoolTime;			// Attack4 쿨타임
	_float m_fAttack5CoolTime;			// Attack5 쿨타임

	_float m_fAttack1MinTime;			// Attack1을 사용하기 까지의 최소 시간
	_float m_fAttack2MinTime;			// Attack2을 사용하기 까지의 최소 시간
	_float m_fAttack3MinTime;			// Attack3을 사용하기 까지의 최소 시간
	_float m_fAttack4MinTime;			// Attack4을 사용하기 까지의 최소 시간
	_float m_fAttack5MinTime;			// Attack5을 사용하기 까지의 최소 시간
#pragma endregion Attack

#pragma region Pattern
	_float m_fPattern1Radius;			// Pattern1 공격범위
	_float m_fPattern2Radius;			// Pattern2 공격범위
	_float m_fPattern3Radius;			// Pattern3 공격범위
	_float m_fPattern4Radius;			// Pattern4 공격범위
	_float m_fPattern5Radius;			// Pattern5 공격범위

	_float m_fPattern1RunTime;			// Pattern1 진행시간
	_float m_fPattern2RunTime;			// Pattern2 진행시간
	_float m_fPattern3RunTime;			// Pattern3 진행시간
	_float m_fPattern4RunTime;			// Pattern4 진행시간
	_float m_fPattern5RunTime;			// Pattern5 진행시간

	_float m_fPattern1CoolTime;			// Pattern1 쿨타임
	_float m_fPattern2CoolTime;			// Pattern2 쿨타임
	_float m_fPattern3CoolTime;			// Pattern3 쿨타임
	_float m_fPattern4CoolTime;			// Pattern4 쿨타임
	_float m_fPattern5CoolTime;			// Pattern5 쿨타임

	_float m_fPattern1MinTime;			// Pattern1을 사용하기 까지의 최소 시간
	_float m_fPattern2MinTime;			// Pattern2을 사용하기 까지의 최소 시간
	_float m_fPattern3MinTime;			// Pattern3을 사용하기 까지의 최소 시간
	_float m_fPattern4MinTime;			// Pattern4을 사용하기 까지의 최소 시간
	_float m_fPattern5MinTime;			// Pattern5을 사용하기 까지의 최소 시간

	_float m_fHit1MinTime;				// HIT1상태 지속시간
	_float m_fHit1RunTime;				// HIT1상태 진행시간

	_float m_fHit2MinTime;				// HIT2상태 지속시간
	_float m_fHit2RunTime;				// HIT2상태 지속시간

	_float m_fHit3MinTime;				// HIT3상태 진행시간
	_float m_fHit3RunTime;				// HIT3상태 진행시간
#pragma endregion Pattern

}MONSTER_INFO;

class CMonster : public CCharactor
{
public:
	explicit CMonster() = default;
	virtual ~CMonster() = default;

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
public:
	void RemoveMonsterCollider();

protected:
	virtual void ReadyEffect() {};
		virtual HRESULT ReadyParticle(const wstring& _wstrJsonFileName, _uint _iParticleNum) { return S_OK; }
		virtual HRESULT ReadyEffectMesh(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum) { return S_OK; }
		virtual HRESULT ReadyMovingEffect(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float2 _vMinMaxSpeed) { return S_OK; }
		virtual HRESULT ReadyAttackEffect(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius) { return S_OK; }
		virtual HRESULT ReadyProjectileEffect_OBB(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float3 _vExtents, _float3 _vRotate) { return S_OK; }
		virtual HRESULT ReadySpriteEffects(const wstring& _wstrJsonFileName, _uint _iEffectsNum, _uint _iRow, _uint _iCol, _float _fSpeedRatio) { return S_OK; }

protected:
	HRESULT MakeSeal(const wstring& _wstrEffectName);

private:
	virtual _int OnCollisionEnter(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc = {});

	virtual void OnCollisionStay(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc = {});

	virtual void OnCollisionExit(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc = {});

/********************
	Getter / Setter
********************/
public:
	

/********************
	Data Members
********************/
protected:
	static _uint g_iNumMonster;

protected:
	// [ 몬스터 정보 ] -> 몬스터마다 각자 세팅해줄 것!
	MONSTER_INFO m_tInfo;

	// [ 나히다 표식 마킹용 ]
	_bool m_bNahida_Marked = false;
	_float m_fMarkingTimeAcc = 0.f;
	_float m_fMarkingMinTime = 5.f;
};