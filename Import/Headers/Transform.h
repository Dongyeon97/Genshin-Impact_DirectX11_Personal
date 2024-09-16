#pragma once

BEGIN(Engine)

class ENGINE_DLL CTransform final
{
public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

public:
	explicit CTransform() = default;
	virtual ~CTransform() = default;

public:
	HRESULT Initialize();

public:
	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_float3 Get_Scaled() {
		return _float3(
			XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
			XMVector3Length(Get_State(STATE_UP)).m128_f32[0],
			XMVector3Length(Get_State(STATE_LOOK)).m128_f32[0]
		);
	}

	_matrix Get_WorldMatrix() const {

		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	_matrix Get_WorldMatrix_Inverse() {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

	void Set_Scaling(_float fX, _float fY, _float fZ);

	void Set_State(STATE eState, _fvector vState)
	{
		_float4		vTmp;
		XMStoreFloat4(&vTmp, vState);
		memcpy(&m_WorldMatrix.m[eState], &vTmp, sizeof(_float4));
	}

	void SetSpeedPerSec(_float _fSpeed) { m_fSpeedPerSec = _fSpeed; }
	_float GetSpeedPerSec() { return m_fSpeedPerSec; }

	void SetRotationPerSec(_float _fRotateSpeed) { m_fRotationPerSec = _fRotateSpeed; }
	_float GetRotationPerSec() { return m_fRotationPerSec; }

public:
	HRESULT BindShaderResource(ID3DX11Effect* _pEffect, const _char* pContantName, const _float4x4* pMatrix);

public:
	void Go_Straight(_float fTimeDelta);
	void Go_Backward(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Turn(_fvector vAxis, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);
	void MoveTo(_fvector vPoint, _float fLimit, _float fTimeDelta);
	void LookAt(_fvector vPoint);
	void LookAt_ForLandObject(_fvector vPoint);

private:
	_float4x4			m_WorldMatrix = {};

	_float				m_fSpeedPerSec = { 0.0f };
	_float				m_fRotationPerSec = { 0.0f };
};

END