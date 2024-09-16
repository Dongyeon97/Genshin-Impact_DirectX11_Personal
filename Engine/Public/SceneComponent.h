#pragma once
#include "ActorComponent.h"

/* SceneComponent에는 변환이 있지만 렌더링이나 충돌 기능은 없습니다 */

BEGIN(Engine)

class ENGINE_DLL CSceneComponent : public CActorComponent
{
public:
	explicit CSceneComponent() = default;
	virtual ~CSceneComponent() = default;

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
	void UpdateSceneComponentTransform();

/********************
	Getter/Setter
********************/
public:
	// [ 월드 행렬 ]
	void SetWorldMatrix(_matrix _matWorld) { XMStoreFloat4x4(&m_matWorld, _matWorld); }
	_matrix GetWorldMatrix() { return XMLoadFloat4x4(&m_matWorld); }

	_vector GetRightVector() { return XMLoadFloat4x4(&m_matWorld).r[0]; }
	_vector GetUpVector() { return XMLoadFloat4x4(&m_matWorld).r[1]; }
	_vector GetLookVector() { return XMLoadFloat4x4(&m_matWorld).r[2]; }
	_vector GetPositionVector() { return XMLoadFloat4x4(&m_matWorld).r[3]; }

	// [ 컴포넌트 월드 위치 ]
	_vector GetComponentWorldPosition() { return XMLoadFloat4(&m_vWorldPosition); }

	// [ Scale ]
	void SetScale(_vector _vScale) { XMStoreFloat4(&m_vScale, _vScale); }
	_vector GetScale() { return XMLoadFloat4(&m_vScale); }

	// [ Rotation ]
	void SetRotation(_vector _vRotation) { XMStoreFloat4(&m_vRotation, _vRotation); }
	_vector GetRotation() { return XMLoadFloat4(&m_vRotation); }

	void SetAdditionalRotationMatrix(_matrix _matAdditionalRotation) { XMStoreFloat4x4(&m_matAdditionalRotation, _matAdditionalRotation); }
	_matrix GetAdditionalRotationMatrix() { return XMLoadFloat4x4(&m_matAdditionalRotation); }

	// [ Translation ]
	void SetActorTranslation(_fvector _vPosition) { XMStoreFloat4(&m_vTranslate, _vPosition); }
	_vector GetActorTranslation() { return XMLoadFloat4(&m_vTranslate); }

/********************
	Data Members
********************/
protected:
	// 액터의 로컬 행렬
	_float4x4	m_matLocal = {};

	// 액터의 월드 행렬
	_float4x4	m_matWorld = {};

	// 액터의 크기
	_float4 m_vScale = { 1.0f, 1.0f, 1.0f, 0.0f };

	// 액터의 회전
	_float4 m_vRotation = { 0.0f, 0.0f, 0.0f, 0.0f };

	// 기타 회전 행렬
	_float4x4 m_matAdditionalRotation = {};

	// 최종 회전 행렬
	_float4x4 m_matTotalRotation = {};

	// 액터의 이동, 액터의 로컬 좌표
	_float4 m_vTranslate = { 0.0f, 0.0f, 0.0f, 1.0f };

	// 액터의 월드 좌표
	_float4 m_vWorldPosition = { 0.0f, 0.0f, 0.0f, 1.0f };
};

END