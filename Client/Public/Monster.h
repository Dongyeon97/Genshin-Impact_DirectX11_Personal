#pragma once
#include "Charactor.h"
#include "MonsterStateDefine.h"

BEGIN(Engine)
class CCollider;
END

/* == ���� ���� ����ü == */
typedef struct
{
	MONSTER_TYPE m_eMonsterType;		// ���� Ÿ��(�Ϲ�,�߰�,�߰�����,����)
	HIT_TYPE m_eHitType;				// �ǰ� Ÿ��(�Ϲ�,���۾Ƹ�,����)
	ELEMENTAL_TYPE m_eElementalType;	// �Ӽ�

	_float m_fDetectingRadius;			// �νĹ���
#pragma region Attack
	_float m_fAttack1Radius;			// Attack1 ���ݹ���
	_float m_fAttack2Radius;			// Attack2 ���ݹ���
	_float m_fAttack3Radius;			// Attack3 ���ݹ���
	_float m_fAttack4Radius;			// Attack4 ���ݹ���
	_float m_fAttack5Radius;			// Attack5 ���ݹ���

	_float m_fAttack1RunTime;			// Attack1 ����ð�
	_float m_fAttack2RunTime;			// Attack2 ����ð�
	_float m_fAttack3RunTime;			// Attack3 ����ð�
	_float m_fAttack4RunTime;			// Attack4 ����ð�
	_float m_fAttack5RunTime;			// Attack5 ����ð�

	_float m_fAttack1CoolTime;			// Attack1 ��Ÿ��
	_float m_fAttack2CoolTime;			// Attack2 ��Ÿ��
	_float m_fAttack3CoolTime;			// Attack3 ��Ÿ��
	_float m_fAttack4CoolTime;			// Attack4 ��Ÿ��
	_float m_fAttack5CoolTime;			// Attack5 ��Ÿ��

	_float m_fAttack1MinTime;			// Attack1�� ����ϱ� ������ �ּ� �ð�
	_float m_fAttack2MinTime;			// Attack2�� ����ϱ� ������ �ּ� �ð�
	_float m_fAttack3MinTime;			// Attack3�� ����ϱ� ������ �ּ� �ð�
	_float m_fAttack4MinTime;			// Attack4�� ����ϱ� ������ �ּ� �ð�
	_float m_fAttack5MinTime;			// Attack5�� ����ϱ� ������ �ּ� �ð�
#pragma endregion Attack

#pragma region Pattern
	_float m_fPattern1Radius;			// Pattern1 ���ݹ���
	_float m_fPattern2Radius;			// Pattern2 ���ݹ���
	_float m_fPattern3Radius;			// Pattern3 ���ݹ���
	_float m_fPattern4Radius;			// Pattern4 ���ݹ���
	_float m_fPattern5Radius;			// Pattern5 ���ݹ���

	_float m_fPattern1RunTime;			// Pattern1 ����ð�
	_float m_fPattern2RunTime;			// Pattern2 ����ð�
	_float m_fPattern3RunTime;			// Pattern3 ����ð�
	_float m_fPattern4RunTime;			// Pattern4 ����ð�
	_float m_fPattern5RunTime;			// Pattern5 ����ð�

	_float m_fPattern1CoolTime;			// Pattern1 ��Ÿ��
	_float m_fPattern2CoolTime;			// Pattern2 ��Ÿ��
	_float m_fPattern3CoolTime;			// Pattern3 ��Ÿ��
	_float m_fPattern4CoolTime;			// Pattern4 ��Ÿ��
	_float m_fPattern5CoolTime;			// Pattern5 ��Ÿ��

	_float m_fPattern1MinTime;			// Pattern1�� ����ϱ� ������ �ּ� �ð�
	_float m_fPattern2MinTime;			// Pattern2�� ����ϱ� ������ �ּ� �ð�
	_float m_fPattern3MinTime;			// Pattern3�� ����ϱ� ������ �ּ� �ð�
	_float m_fPattern4MinTime;			// Pattern4�� ����ϱ� ������ �ּ� �ð�
	_float m_fPattern5MinTime;			// Pattern5�� ����ϱ� ������ �ּ� �ð�

	_float m_fHit1MinTime;				// HIT1���� ���ӽð�
	_float m_fHit1RunTime;				// HIT1���� ����ð�

	_float m_fHit2MinTime;				// HIT2���� ���ӽð�
	_float m_fHit2RunTime;				// HIT2���� ���ӽð�

	_float m_fHit3MinTime;				// HIT3���� ����ð�
	_float m_fHit3RunTime;				// HIT3���� ����ð�
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
	// [ ���� ���� ] -> ���͸��� ���� �������� ��!
	MONSTER_INFO m_tInfo;

	// [ ������ ǥ�� ��ŷ�� ]
	_bool m_bNahida_Marked = false;
	_float m_fMarkingTimeAcc = 0.f;
	_float m_fMarkingMinTime = 5.f;
};