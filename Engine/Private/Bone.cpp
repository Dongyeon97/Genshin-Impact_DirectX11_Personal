#include "Engine_pch.h"
#include "Bone.h"

HRESULT CBone::Initialize(const MY_AINODE* pAIBone, _int iParentBoneIndex, _fmatrix PivotMatrix)
{
	strcpy_s(m_szName, pAIBone->szBoneName);
	memcpy(&m_TransformationMatrix, &pAIBone->matTransformation, sizeof(_float4x4));

	if (-1 == iParentBoneIndex)
	{
		XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)) * PivotMatrix);
	}
	else
	{
		XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
	}

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	m_iParentBoneIndex = iParentBoneIndex;

	return S_OK;
}

void CBone::Release()
{

}

shared_ptr<CBone> CBone::Create(const MY_AINODE* pAIBone, _int iParentBoneIndex, _fmatrix PivotMatrix)
{
	shared_ptr<CBone> spInstance = make_shared<CBone>();

	if (FAILED(spInstance->Initialize(pAIBone, iParentBoneIndex, PivotMatrix)))
	{
		MSG_BOX("Failed to Created : CBone");
		spInstance->Release();
		spInstance.reset();
		return nullptr;
	}

	return spInstance;
}

void CBone::InvalidateCombinedTransformationMatrix(const vector<shared_ptr<CBone>>& _vecBones)
{
	// 만약 첫 번째로 추가된 본이라면, 자기 자신의 TransformationMatrix가 CombinedTransformationMatrix가 된다.
	if (-1 == m_iParentBoneIndex)
	{
		m_CombinedTransformationMatrix = m_TransformationMatrix;
	}
	else if(0 == strcmp("Bip001", m_szName))
	{
		XMMATRIX matPivot = XMMatrixRotationY(XMConvertToRadians(180.f));

		XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * matPivot);
	}
	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
		(XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&_vecBones[m_iParentBoneIndex]->m_CombinedTransformationMatrix)));
	}
}
