#pragma once

/* CameraManager */
// 1. 카메라 컴포넌트를 보관
// 2. 현재 선택되어있는 카메라의 정보를 저장하고 반환

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
	// [ 카메라 매니저에 카메라를 추가 ]
	void AddCamera(const wstring& _wstrCameraTag, shared_ptr<CCameraComponent> _spCameraComponent);

	// [ 카메라 검색 ]
	shared_ptr<CCameraComponent> FindCamera(const wstring& _wstrCameraTag);

	// [ 카메라 삭제 ]
	void RemoveCamera(const wstring& _wstrCameraTag);

/********************
	Getter / Setter
********************/
public:
	// [ 현재 카메라 ]
	void SetCurCamera(const wstring& _wstrCameraTag);
	shared_ptr<CCameraComponent> GetCurCamera() { return m_wpCurCamera.lock(); }

	// [ 현재 카메라 위치 ]
	_vector GetCamPosition();

	// [ 카메라의 월드 ]
	const _matrix	GetCamWorldMatrix();
	const _float4x4 GetViewmatrix_Float4x4();

	// [ 뷰 ]
	const _matrix	GetViewMatrix();
	const _float4x4	GetViewMatrix_Float4x4();

	// [ 투영 ]
	const _matrix	GetProjMatrix();
	const _float4x4	GetProjMatrix_Float4x4();

/********************
	Data Members
********************/
private:
	// [ 현재 카메라 ]
	weak_ptr<CCameraComponent> m_wpCurCamera;

	// [ 카메라 매니저가 보관하고 있는 카메라 컴포넌트들 ]
	map<wstring, shared_ptr<CCameraComponent>> m_mapCameras;
};

END