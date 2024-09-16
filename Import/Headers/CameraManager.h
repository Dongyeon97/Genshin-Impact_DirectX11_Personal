#pragma once

/* CameraManager */
// 1. ī�޶� ������Ʈ�� ����
// 2. ���� ���õǾ��ִ� ī�޶��� ������ �����ϰ� ��ȯ

BEGIN(Engine)

class CCameraComponent;

class ENGINE_DLL CCameraManager final
{
	DECLARE_SINGLETON(CCameraManager)
/********************
	Framework
********************/
public:
	HRESULT Initialize();
	void Release();

/********************
	Methods
********************/
public:
	void ClearCurrentScene();

public:
	// [ ī�޶� �Ŵ����� ī�޶� �߰� ]
	void AddCamera(const wstring& _wstrCameraTag, shared_ptr<CCameraComponent> _spCameraComponent);

	// [ ī�޶� �˻� ]
	shared_ptr<CCameraComponent> FindCamera(const wstring& _wstrCameraTag);

	// [ ī�޶� ���� ]
	void RemoveCamera(const wstring& _wstrCameraTag);

/********************
	Getter / Setter
********************/
public:
	// [ ���� ī�޶� ]
	void SetCurCamera(const wstring& _wstrCameraTag);
	shared_ptr<CCameraComponent> GetCurCamera() { return m_wpCurCamera.lock(); }

	// [ ���� ī�޶� ��ġ ]
	_vector GetCamPosition();

	// [ ī�޶��� ���� ]
	const _matrix	GetCamWorldMatrix();
	const _float4x4 GetViewmatrix_Float4x4();

	// [ �� ]
	const _matrix	GetViewMatrix();
	const _float4x4	GetViewMatrix_Float4x4();

	// [ ���� ]
	const _matrix	GetProjMatrix();
	const _float4x4	GetProjMatrix_Float4x4();

/********************
	Data Members
********************/
private:
	// [ ���� ī�޶� ]
	weak_ptr<CCameraComponent> m_wpCurCamera;

	// [ ī�޶� �Ŵ����� �����ϰ� �ִ� ī�޶� ������Ʈ�� ]
	map<wstring, shared_ptr<CCameraComponent>> m_mapCameras;
};

END