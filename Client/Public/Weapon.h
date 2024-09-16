#pragma once
#include "NonAnimModel.h"

BEGIN(Engine)
class CNonAnimModel;
class CAttackCollider;
END

class CWeapon : public CNonAnimModel
{
public:
	explicit CWeapon() = default;
	virtual ~CWeapon() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	virtual void Render(_uint _iMeshIndex);
	virtual void EndPlay() override;
	virtual void Release() override;

/********************
	Methods
********************/
public:
	// [ 무기의 트랜스폼 업데이트 ]
	void UpdateWeaponTransform();

	// [ 셰이더 리소스를 던짐 ]
	HRESULT BindShaderResources();

	// [ 무기 렌더에 필요한 값 세팅 후 렌더 ]
	void RenderWeapon();

/********************
	Getter / Setter
********************/
public:
	void SetSocketMatrix(const _float4x4* _matSocketMatrix) { m_pSocketMatrix = _matSocketMatrix; }

	void SetTransformCopy(const _float3 _vTransformCopy) { m_vTransformCopy = _vTransformCopy; }

	_vector GetTransformCopy() { return XMLoadFloat3(&m_vTransformCopy); }

	shared_ptr<CAttackCollider> GetWeaponCollider() { return m_spAttackCollider; }

/********************
	Data Members
********************/
protected:
	// [ 공격 콜라이더 ]
	shared_ptr<CAttackCollider> m_spAttackCollider;

protected:
	// [ 무기가 현재 부착될 뼈 ]
	const _float4x4* m_pSocketMatrix = nullptr;

	// [ 블렌더에서의 TransformCopy 로테이션 값 ] -> 초기 회전값 세팅할 때, 이 회전값의 부호를 바꿔 회전시켜 놓아야 함
	_float3 m_vTransformCopy;
};