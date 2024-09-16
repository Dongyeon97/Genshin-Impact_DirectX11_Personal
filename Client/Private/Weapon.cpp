#include "pch.h"
#include "Weapon.h"

#include "RenderManager.h"
#include "LightManager.h"
#include "CameraManager.h"

#include "Actor.h"

#include "AttackCollider.h"

void CWeapon::Initialize()
{
	CNonAnimModel::Initialize();
}

_int CWeapon::PreUpdate(const _float& _fTimeDelta)
{
	return CNonAnimModel::PreUpdate(_fTimeDelta);
}

_int CWeapon::Update(const _float& _fTimeDelta)
{
	// �پ��ִ� ���� ����� �����´�.
	_matrix	BoneMatrix = XMLoadFloat4x4(m_pSocketMatrix);

	// ����� ����ȭ �Ѵ�.
	for (size_t i = 0; i < 3; i++)
	{
		BoneMatrix.r[i] = XMVector3Normalize(BoneMatrix.r[i]);
	}

	// ���θ��� ����� ���Ѵ�.
	UpdateWeaponTransform();

	// �ڱ� -> ��(1�� �θ�) -> ����(2�� �θ�) ������� ��� ������ �ؼ� ����� ������.
	XMStoreFloat4x4(&m_matWorld, XMLoadFloat4x4(&m_matWorld) * BoneMatrix * m_wpOwnerActor.lock()->GetWorldMatrix());

	return CActorComponent::Update(_fTimeDelta);
	// return CNonAnimModel::Update(_fTimeDelta);
}

_int CWeapon::LateUpdate(const _float& _fTimeDelta)
{
	if (nullptr != m_spAttackCollider)
	{
		m_spAttackCollider->SetOwnerWorld(XMLoadFloat4x4(&m_matWorld));
		m_spAttackCollider->LateUpdate(_fTimeDelta);
	}

//#ifdef _DEBUG
//	if (nullptr != m_spAttackCollider) 
//		CRenderManager::GetInstance()->AddDebugComponent(m_spAttackCollider);
//#endif // _DEBUG

	return CNonAnimModel::LateUpdate(_fTimeDelta);
}

void CWeapon::Render(_uint _iMeshIndex)
{
	if(m_bRender)
		RenderWeapon();
}

void CWeapon::EndPlay()
{
	CNonAnimModel::EndPlay();
}

void CWeapon::Release()
{
	if (nullptr != m_spAttackCollider)
	{
		m_spAttackCollider->Release();
		m_spAttackCollider.reset();
	}

	CNonAnimModel::Release();
}

void CWeapon::RenderWeapon()
{
	// ���� ������Ʈ�� ���θ��� ������ ���̴� ������ �������� �Ѵ�.
	if (FAILED(BindShaderResources()))
	{
		MSG_BOX("Failed to BindShaderResources : CWeapon");
		return;
	}

	_uint iNumMeshes = GetNumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(BindMaterialShaderResource(i, TEXTURE_TYPE::DIFFUSE, "g_DiffuseTexture")))
		{
			MSG_BOX("Failed to BindMaterialShaderResource : CWeapon");
			return;
		}

		if (FAILED(BeginShader(0)))
		{
			MSG_BOX("Failed to BeginShader : CWeapon");
			return;
		}

		CNonAnimModel::Render(i);
	}
}

HRESULT CWeapon::BindShaderResources()
{
	_float4x4		ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, CCameraManager::GetInstance()->GetViewMatrix());
	XMStoreFloat4x4(&ProjMatrix, CCameraManager::GetInstance()->GetProjMatrix());

	BindMatrix("g_WorldMatrix", &m_matWorld);
	BindMatrix("g_ViewMatrix", &ViewMatrix);
	BindMatrix("g_ProjMatrix", &ProjMatrix);

	return S_OK;
}

void CWeapon::UpdateWeaponTransform() // ���� ������Ʈ�� SceneComponent�� ������� ������ �и��Ѵ�.(�߰��� ������� �����ϱ� ����)
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
}
