#include "Engine_pch.h"
#include "Actor.h"

#include "DeviceManager.h"
#include "RenderManager.h"
#include "NavigationManager.h"
#include "CameraManager.h"

#include "Level.h"
#include "Layer.h"
#include "ActorComponent.h"
#include "Navigation.h"
#include "CameraComponent.h"

void CActor::Initialize()
{
	// ��� ���ʹ� Initialize �ܰ迡�� �ݵ�� ����̽��� �����մϴ�.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	// [���� ���], [���� ���], [�߰� ȸ�����]�� ������ķ� �ʱ�ȭ
	XMStoreFloat4x4(&m_matLocal, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matAdditionalRotation, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matMouseRotate, XMMatrixIdentity());

	UpdateActorTransform();

	for (auto& pair : m_umapActors)
		pair.second->Initialize();

	for (auto& pair : m_umapActorComponents)
		pair.second->Initialize();
}

_int CActor::PreUpdate(const _float& _fTimeDelta)
{
	for (auto& pair : m_umapActors)
		pair.second->PreUpdate(_fTimeDelta);

	for (auto& pair : m_umapActorComponents)
		pair.second->PreUpdate(_fTimeDelta);

	return S_OK;
}

_int CActor::Update(const _float& _fTimeDelta)
{
	UpdateActorTransform();

	for (auto& pair : m_umapActors)
		pair.second->Update(_fTimeDelta);

	for (auto& pair : m_umapActorComponents)
		pair.second->Update(_fTimeDelta);

	return S_OK;
}

_int CActor::LateUpdate(const _float& _fTimeDelta)
{
	// �� �����Ӹ��� Late_Update ������ ������ ���� �׷쿡 ���͸� �߰��մϴ�.
	if (FAILED(CRenderManager::GetInstance()->AddObjectToRenderGroup(m_eRenderGroup, dynamic_pointer_cast<CActor>(shared_from_this()))))
		return E_FAIL;

	for (auto& pair : m_umapActors)
		pair.second->LateUpdate(_fTimeDelta);

	for (auto& pair : m_umapActorComponents)
		pair.second->LateUpdate(_fTimeDelta);

	return S_OK;
}

void CActor::Render()
{
	// for (auto& pair : m_umapActors)
		// pair.second->Render();

	for (auto& pair : m_umapActorComponents)
		pair.second->Render();
}

void CActor::Render_Shadow()
{

}

void CActor::EndPlay()
{
	for (auto& pair : m_umapActors)
		pair.second->EndPlay();

	for (auto& pair : m_umapActorComponents)
		pair.second->EndPlay();
}

void CActor::Release()
{
	for (auto& pair : m_umapActorComponents)
	{
		pair.second->Release();
		pair.second.reset();
	}
	m_umapActorComponents.clear();

	for (auto& pair : m_umapActors)
	{
		pair.second->Release();
		pair.second.reset();
	}
	m_umapActors.clear();
}

_float CActor::CalculateDistanceFromCam()
{
	_vector vCamPos = CCameraManager::GetInstance()->GetCamPosition();

	_float fLength = XMVector3Length(vCamPos - GetActorTranslation()).m128_f32[0];

	return fLength;
}

_matrix CActor::CalculateWorldMatrix_Immediately()
{
	UpdateActorTransform();

	return XMLoadFloat4x4(&m_matWorld);
}

void CActor::LookAt(_vector _vPoint)
{
	// 1. ��,��,�� ������ ���� ���Ѵ�.
	_float vScaled[3] = {};

	vScaled[0] = XMVector3Length(GetRightVector()).m128_f32[0];
	vScaled[1] = XMVector3Length(GetUpVector()).m128_f32[0];
	vScaled[2] = XMVector3Length(GetLookVector()).m128_f32[0];

	// 2. ������ ���͸� ���Ѵ�.
	_vector	vPosition = GetActorTranslation();
	vPosition = XMVectorSetW(vPosition, 1.f);

	// 3. �ٶ󺸰� ���� �����ǿ��� �� �������� ���� ���� ���͸� ���Ѵ�.
	_vector	vLook = _vPoint - vPosition;

	// 4. XMVectorSet(0.f, 1.f, 0.f, 0.f)�� Look���͸� �����ؼ� Right���͸� ���Ѵ�.
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);

	// 5. �ٽ� Look�� Right�� �����ؼ� Up�� ���Ѵ�.
	_vector vUp = XMVector3Cross(vLook, vRight);

	_matrix matWorld = {};

	matWorld.r[0] = XMVector3Normalize(vRight) * vScaled[0];
	matWorld.r[1] = XMVector3Normalize(vUp) * vScaled[1];
	matWorld.r[2] = XMVector3Normalize(vLook) * vScaled[2];
	matWorld.r[3] = vPosition;

	SetWorldMatrix(matWorld);
}

shared_ptr<CActor> CActor::FindActor(const wstring& _wstrActorName)
{
	if (m_umapActors.find(_wstrActorName) == m_umapActors.end()) 
		return nullptr;

	return m_umapActors[_wstrActorName];
}

shared_ptr<CActorComponent> CActor::FindActorComponent(const std::wstring& _wstrActorComponent)
{
	if (m_umapActorComponents.find(_wstrActorComponent) == m_umapActorComponents.end())
		return nullptr;

	return m_umapActorComponents[_wstrActorComponent];
}

void CActor::UpdateActorTransform()
{
	// ũ��
	_matrix matScale;
	matScale = XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);

	// ȸ��
	_matrix matRotation[3];
	matRotation[0] = XMMatrixRotationX(XMConvertToRadians(m_vRotation.x));
	matRotation[1] = XMMatrixRotationY(XMConvertToRadians(m_vRotation.y));
	matRotation[2] = XMMatrixRotationZ(XMConvertToRadians(m_vRotation.z));

	// �̵�
	_matrix matTranslate;
	matTranslate = XMMatrixTranslation(m_vTranslate.x, m_vTranslate.y, m_vTranslate.z);

	_matrix matLocal;
	_matrix matTotalRotation;
	matLocal = XMMatrixIdentity();
	matTotalRotation = XMMatrixIdentity();

	matLocal = XMMatrixMultiply(matLocal, matScale);

	matTotalRotation = XMMatrixMultiply(matTotalRotation, matRotation[0]);
	matTotalRotation = XMMatrixMultiply(matTotalRotation, matRotation[1]);
	matTotalRotation = XMMatrixMultiply(matTotalRotation, matRotation[2]);

	_matrix matMouseRotation, matAdditionalRotation;

	matMouseRotation = XMLoadFloat4x4(&m_matMouseRotate);
	matAdditionalRotation = XMLoadFloat4x4(&m_matAdditionalRotation);

	matTotalRotation = XMMatrixMultiply(matTotalRotation, matMouseRotation);
	matTotalRotation = XMMatrixMultiply(matTotalRotation, matAdditionalRotation);

	matLocal = XMMatrixMultiply(matLocal, matTotalRotation);
	XMStoreFloat4x4(&m_matTotalRotation, matTotalRotation);

	matLocal = XMMatrixMultiply(matLocal, matTranslate);

	XMStoreFloat4x4(&m_matWorld, matLocal);
}

_int CActor::OnCollisionEnter(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
	return 0;
}

void CActor::OnCollisionStay(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
}

void CActor::OnCollisionExit(std::weak_ptr<CCollider> _wpSrcCollider, std::weak_ptr<CCollider> _wpDstCollider, ATTACK_DESC AttackDesc)
{
}

void CActor::SetActorTranslation(_fvector _vPosition, _int* _iCurNaviCellIndex)
{
	shared_ptr<CNavigation> spNavi = CNavigationManager::GetInstance()->GetCurNavigation();

	_float fPlaneY = 0.0f;

	if (nullptr == spNavi)
	{
		XMStoreFloat4(&m_vTranslate, _vPosition);
		return;
	}
	else if (true == spNavi->IsMove(_vPosition, _iCurNaviCellIndex, &fPlaneY))
	{
		_float4 vFinalPosition;
		XMStoreFloat4(&vFinalPosition, _vPosition);

		vFinalPosition.y = fPlaneY;

		XMStoreFloat4(&m_vTranslate, XMLoadFloat4(&vFinalPosition));
	}
}