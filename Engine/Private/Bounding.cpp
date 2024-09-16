#include "Engine_pch.h"
#include "Bounding.h"

#include "DeviceManager.h"

HRESULT CBounding::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	return S_OK;
}

_int CBounding::Update(_fmatrix _matOwnerWorld)
{
	return S_OK;
}

void CBounding::Release()
{
}
