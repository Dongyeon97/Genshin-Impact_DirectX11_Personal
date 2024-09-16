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
	// [ ������ Ʈ������ ������Ʈ ]
	void UpdateWeaponTransform();

	// [ ���̴� ���ҽ��� ���� ]
	HRESULT BindShaderResources();

	// [ ���� ������ �ʿ��� �� ���� �� ���� ]
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
	// [ ���� �ݶ��̴� ]
	shared_ptr<CAttackCollider> m_spAttackCollider;

protected:
	// [ ���Ⱑ ���� ������ �� ]
	const _float4x4* m_pSocketMatrix = nullptr;

	// [ ���������� TransformCopy �����̼� �� ] -> �ʱ� ȸ���� ������ ��, �� ȸ������ ��ȣ�� �ٲ� ȸ������ ���ƾ� ��
	_float3 m_vTransformCopy;
};