#include "Engine_pch.h"
#include "SceneComponent.h"

#include "Actor.h"

void CSceneComponent::Initialize()
{
	// [���� ���], [���� ���], [�߰� ȸ�����]�� ������ķ� �ʱ�ȭ
	XMStoreFloat4x4(&m_matLocal, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matAdditionalRotation, XMMatrixIdentity());

	CActorComponent::Initialize();
}

_int CSceneComponent::PreUpdate(const _float& _fTimeDelta)
{
	return CActorComponent::PreUpdate(_fTimeDelta);
}

_int CSceneComponent::Update(const _float& _fTimeDelta)
{
	UpdateSceneComponentTransform();

	return CActorComponent::Update(_fTimeDelta);
}

_int CSceneComponent::LateUpdate(const _float& _fTimeDelta)
{
	return CActorComponent::LateUpdate(_fTimeDelta);
}

void CSceneComponent::Render()
{
	CActorComponent::Render();
}

void CSceneComponent::EndPlay()
{
	CActorComponent::EndPlay();
}

void CSceneComponent::Release()
{
	CActorComponent::Release();
}

void CSceneComponent::UpdateSceneComponentTransform()
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
	matLocal = XMMatrixIdentity();

	matLocal = XMMatrixMultiply(matLocal, matScale);

	_matrix matTotalRotation = XMMatrixIdentity();;

	matTotalRotation = XMMatrixMultiply(matTotalRotation, matRotation[0]);
	matTotalRotation = XMMatrixMultiply(matTotalRotation, matRotation[1]);
	matTotalRotation = XMMatrixMultiply(matTotalRotation, matRotation[2]);

	_matrix matAdditionalRotation;

	matAdditionalRotation = XMLoadFloat4x4(&m_matAdditionalRotation);

	matTotalRotation = XMMatrixMultiply(matTotalRotation, matAdditionalRotation);
	XMStoreFloat4x4(&m_matTotalRotation, matTotalRotation);

	matLocal = XMMatrixMultiply(matLocal, matTotalRotation);

	matLocal = XMMatrixMultiply(matLocal, matTranslate);

	XMStoreFloat4x4(&m_matWorld, matLocal);

	_matrix matOwnerWorld;

	// �θ� �ִٸ� �ϼ��� ���� ��Ŀ� �θ��� ��������� �����ش�.
	if (!m_wpOwnerActor.expired())
	{
		matOwnerWorld = GetOwnerActor().lock()->GetWorldMatrix();

		matLocal = XMMatrixMultiply(matLocal, matOwnerWorld);

		XMStoreFloat4x4(&m_matWorld, matLocal);
	}

	m_vWorldPosition = { m_matWorld._41, m_matWorld._42, m_matWorld._43, 1.0f };
}
