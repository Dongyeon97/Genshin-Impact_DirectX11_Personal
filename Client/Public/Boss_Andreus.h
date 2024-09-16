#pragma once
#include "BossMonster.h"

BEGIN(Engine)
class CAttackCollider;
END

class CBoss_Andreus final : public CBossMonster
{
public:
	explicit CBoss_Andreus() = default;
	virtual ~CBoss_Andreus() = default;

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
private:
	void ReadyBoneMatrix();
	void UpdateColliderTransform();

private:
	virtual void ReadyEffect() override;
		virtual HRESULT ReadyParticle(const wstring& _wstrJsonFileName, _uint _iParticleNum) override;
		virtual HRESULT ReadyEffectMesh(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum) override;
		virtual HRESULT ReadyMovingEffect(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float2 _vMinMaxSpeed) override;
		virtual HRESULT ReadyAttackEffect(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float _fRadius) override;
		virtual HRESULT ReadyProjectileEffect_OBB(const wstring& _wstrJsonFileName, _uint _iEffectMeshNum, _float3 _vExtents, _float3 _vCenter) override;
		virtual HRESULT ReadySpriteEffects(const wstring& _wstrJsonFileName, _uint _iEffectsNum, _uint _iRow, _uint _iCol, _float _fSpeedRatio) override;

/********************
	Getter / Setter
********************/
public:


/********************
	Data Members
********************/
private:
	// [ ���� ���� �ݶ��̴� ]
	shared_ptr<CAttackCollider> m_spLeftArm_AttackCollider;

	// [ ������ ���� �ݶ��̴� ]
	shared_ptr<CAttackCollider> m_spRightArm_AttackCollider;

	// [ ���� ���� �ݶ��̴� ]
	shared_ptr<CAttackCollider> m_spTail_AttackCollider;

	// [ ���� �� ]
	const _float4x4* m_pSocketMatrix_LeftArm = nullptr;
	// [ ������ �� ]
	const _float4x4* m_pSocketMatrix_RightArm = nullptr;
	// [ ���� �� ]
	const _float4x4* m_pSocketMatrix_Tail = nullptr;

	// [ ���� ������� ]
	_float4x4 m_matLeftArm = {};

	// [ ������ ������� ]
	_float4x4 m_matRightArm = {};

	// [ ���� ������� ]
	_float4x4 m_matTail = {};

private:
	// [ ���� ���� �÷��� ]
	_ullong m_State = MONSTER_STATE::IDLE;
};

