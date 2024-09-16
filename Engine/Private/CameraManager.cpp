#include "Engine_pch.h"
#include "CameraManager.h"

#include "CameraComponent.h"

IMPLEMENT_SINGLETON(CCameraManager)

HRESULT CCameraManager::Initialize()
{
	return S_OK;
}

void CCameraManager::Release()
{
	for (auto& Pair : m_mapCameras)
	{
		if (Pair.second)
		{
			Pair.second->Release();
			Pair.second.reset();
		}
	}

	m_mapCameras.clear();

	// 카메라 매니저 파괴
	m_upInstance.reset();
}

void CCameraManager::ClearCurrentScene()
{
	// [ 카메라 매니저가 보관하고 있는 카메라 컴포넌트들 ]
	/*for (auto& Pair : m_mapCameras)
	{
		if (nullptr != Pair.second)
		{
			Pair.second->Release();
			Pair.second.reset();
		}
	}
	m_mapCameras.clear();*/
}

void CCameraManager::AddCamera(const wstring& _wstrCameraTag, shared_ptr<CCameraComponent> _spCameraComponent)
{
	if (m_mapCameras.end() != m_mapCameras.find(_wstrCameraTag))
		return;

	m_mapCameras.emplace(_wstrCameraTag, _spCameraComponent);
}

shared_ptr<CCameraComponent> CCameraManager::FindCamera(const wstring& _wstrCameraTag)
{
	auto iter = m_mapCameras.find(_wstrCameraTag);

	if (m_mapCameras.end() == iter)
		return nullptr;

	return iter->second;
}

void CCameraManager::RemoveCamera(const wstring& _wstrCameraTag)
{
	auto iter = m_mapCameras.find(_wstrCameraTag);

	if (m_mapCameras.end() != iter)
	{
		(*iter).second.reset();
		m_mapCameras.erase(iter);
	}
}

void CCameraManager::SetCurCamera(const wstring& _wstrCameraTag)
{
	auto iter = m_mapCameras.find(_wstrCameraTag);

	if (m_mapCameras.end() == iter)
		return;

	m_wpCurCamera = iter->second;
}

_vector CCameraManager::GetCamPosition()
{
	if (!m_wpCurCamera.expired())
	{
		_matrix matCamWorld;

		matCamWorld = m_wpCurCamera.lock()->GetWorldMatrix();

		_vector vCamPosition = matCamWorld.r[3];
		
		return vCamPosition;
	}

	return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

const _matrix CCameraManager::GetCamWorldMatrix()
{
	if (!m_wpCurCamera.expired())
		return m_wpCurCamera.lock()->GetWorldMatrix();

	// 카메라 없으면 항등행렬 반환
	return XMMatrixIdentity();
}

const _float4x4 CCameraManager::GetViewmatrix_Float4x4()
{
	if (!m_wpCurCamera.expired())
	{
		_float4x4 matCamWorld;
		XMStoreFloat4x4(&matCamWorld, m_wpCurCamera.lock()->GetWorldMatrix());
		return matCamWorld;
	}

	_float4x4 matIdentity;
	XMStoreFloat4x4(&matIdentity, XMMatrixIdentity());

	return matIdentity;
}

const _matrix CCameraManager::GetViewMatrix()
{
	if (!m_wpCurCamera.expired())
	{
		return m_wpCurCamera.lock()->GetViewMatrix();
	}

	// MSG_BOX("현재 카메라가 존재하지 않습니다.");

	// 현재 카메라가 존재하지 않으면 항등행렬 반환
	return XMMatrixIdentity();
}

const _float4x4 CCameraManager::GetViewMatrix_Float4x4()
{
	if (!m_wpCurCamera.expired())
	{
		_float4x4 matCameraView;
		XMStoreFloat4x4(&matCameraView, m_wpCurCamera.lock()->GetViewMatrix());

		return matCameraView;
	}

	// MSG_BOX("현재 카메라가 존재하지 않습니다.");

	// 현재 카메라가 존재하지 않으면 항등행렬 반환
	_float4x4 matIdentity;
	XMStoreFloat4x4(&matIdentity, XMMatrixIdentity());

	return matIdentity;
}

const _matrix CCameraManager::GetProjMatrix()
{
	if (!m_wpCurCamera.expired())
	{
		_float fFovY = m_wpCurCamera.lock()->GetFovY();
		_float fNear = m_wpCurCamera.lock()->GetNear();
		_float fFar = m_wpCurCamera.lock()->GetFar();
		_float fAspect = m_wpCurCamera.lock()->GetAspect();

		return XMMatrixPerspectiveFovLH(fFovY, fAspect, fNear, fFar);
	}

	// MSG_BOX("현재 카메라가 존재하지 않습니다.");

	// 현재 카메라가 존재하지 않으면 항등행렬 반환
	return XMMatrixIdentity();
}

const _float4x4 CCameraManager::GetProjMatrix_Float4x4()
{
	if (!m_wpCurCamera.expired())
	{
		_float fFovY = m_wpCurCamera.lock()->GetFovY();
		_float fNear = m_wpCurCamera.lock()->GetNear();
		_float fFar = m_wpCurCamera.lock()->GetFar();
		_float fAspect = m_wpCurCamera.lock()->GetAspect();

		_float4x4 matCameraProj;
		XMStoreFloat4x4(&matCameraProj, XMMatrixPerspectiveFovLH(fFovY, fAspect, fNear, fFar));

		return matCameraProj;
	}


	// MSG_BOX("현재 카메라가 존재하지 않습니다.");

	// 현재 카메라가 존재하지 않으면 항등행렬 반환
	_float4x4 matIdentity;
	XMStoreFloat4x4(&matIdentity, XMMatrixIdentity());

	return matIdentity;
}
