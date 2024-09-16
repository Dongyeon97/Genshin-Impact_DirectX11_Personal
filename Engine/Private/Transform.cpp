#include "Engine_pch.h"
#include "Transform.h"

void CTransform::Set_Scaling(_float fX, _float fY, _float fZ)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * fX);
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * fY);
	Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * fZ);
}

HRESULT CTransform::Initialize()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	
	return S_OK;
}

HRESULT CTransform::BindShaderResource(ID3DX11Effect* _pEffect, const _char* pContantName, const _float4x4* pMatrix)
{
	ID3DX11EffectVariable* pVariable = _pEffect->GetVariableByName(pContantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix((_float*)pMatrix);
}

void CTransform::Go_Straight(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backward(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	for (size_t i = STATE_RIGHT; i < STATE_POSITION; i++)
	{
		_vector		vStateDir = Get_State(STATE(i));

		Set_State(STATE(i), XMVector4Transform(vStateDir, RotationMatrix));
	}
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);
	_float3		vScaled = Get_Scaled();

	for (size_t i = STATE_RIGHT; i < STATE_POSITION; i++)
	{
		_float4		vTmp = _float4(0.f, 0.f, 0.f, 0.f);

		*((_float*)&vTmp + i) = 1.f * *((_float*)&vScaled + i);

		_vector		vStateDir = XMLoadFloat4(&vTmp);

		Set_State(STATE(i), XMVector4Transform(vStateDir, RotationMatrix));
	}
}

void CTransform::MoveTo(_fvector vPoint, _float fLimit, _float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vDir = vPoint - vPosition;

	_float		fDistance = XMVector3Length(vDir).m128_f32[0];

	if (fDistance >= fLimit)
		vPosition += XMVector3Normalize(vDir) * m_fSpeedPerSec * fTimeDelta;
}

void CTransform::LookAt(_fvector vPoint)
{
	_float3	vScaled = Get_Scaled();

	_vector	vPosition = Get_State(CTransform::STATE_POSITION);

	_vector	vLook = vPoint - vPosition;
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);

}

void CTransform::LookAt_ForLandObject(_fvector vPoint)
{
	_float3	vScaled = Get_Scaled();

	_vector	vPosition = Get_State(CTransform::STATE_POSITION);

	_vector	vLook = vPoint - vPosition;
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	vLook = XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);
}