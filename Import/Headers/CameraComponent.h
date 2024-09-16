#pragma once
#include "SceneComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCameraComponent : public CSceneComponent
{
public:
	explicit CCameraComponent() = default;
	virtual ~CCameraComponent() = default;

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
	// [ ������� ���� ���� ] -> Fov�־��� ��, �������� �� �����ؼ� �־��� ��! ���� ������ ��..;�̤�
	void SetCameraProjInfo(_float _fFovY, _float _fNear, _float _fFar, _float _fAspect);

	// [ ī�޶� ���� ������Ʈ ]
	void UpdateCameraTransform(const _float& _fTimeDelta);

/********************
	Getter / Setter
********************/
public:
	_matrix GetViewMatrix() { return XMLoadFloat4x4(&m_matView); }

	// [ ���� ��� ]
	void SetFovY(_float _fFovY) { m_fFovY = _fFovY; }
	_float GetFovY() { return m_fFovY; }

	void SetNear(_float _fNear) { m_fNear = _fNear; }
	_float GetNear() { return m_fNear; }

	void SetFar(_float _fFar) { m_fFar = _fFar; }
	_float GetFar() { return m_fFar; }

	void SetAspect(_float _fAspect) { m_fAspect = _fAspect; }
	_float GetAspect() { return m_fAspect; }

/********************
	Data Members
********************/
protected:
	// �� ���
	_float4x4 m_matView = {};

	// [ �þ� ���� ]
	_float m_fFovY = XMConvertToRadians(60.0f);

	// [ �������� �Ÿ� ]
	_float m_fNear = 0.2f;

	// [ �������� �Ÿ� ]
	_float m_fFar = 5000.0f;

	// [ ��Ⱦ�� ]
	_float m_fAspect = (_float)Engine::g_iWinSizeX / (_float)Engine::g_iWinSizeY;
};

END