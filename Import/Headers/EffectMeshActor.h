#pragma once
#include "MeshActor.h"

BEGIN(Engine)

class CEffectMesh;

class ENGINE_DLL CEffectMeshActor : public CMeshActor
{
public:
	explicit CEffectMeshActor() = default;
	virtual ~CEffectMeshActor() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	virtual void Render();
	virtual void EndPlay() override;
	virtual void Release() override;

	// [ JSON �Ľ� ��]
	virtual HRESULT Initialize(const wstring& _wstrJsonFileName);

	// [ ������Ʈ Ǯ �� ]
	virtual void RecycleBegin();

/********************
	Methods
********************/
public:
	// [ UV Move Mode : off ]
	virtual HRESULT BindShaderResources();

	// [ Json �Ľ� ]
	HRESULT ReadEffectMeshFile_JSON(const wstring& _wstrJsonFileName);

	// [ Transform Change ]
	void TransformChange(const _float& _fTimeDelta);

	/* �� �� */
	void Replay();

/********************
	Getter / Setter
********************/
public:
	// [ JSON ���ϸ� ]
	void SetJsonFileName(const wstring& _wstrJsonFileName) { m_wstrJSONFileName = _wstrJsonFileName; }
	const wstring& GetJsonFileName() { return m_wstrJSONFileName; }

	/* �� �� */
	// [ EFFECT_DESC ���� ]
	void SetEffectMeshDesc(EFFECTMESH_DESC _EffectMeshDesc) { m_EffectMeshDesc = _EffectMeshDesc; }
	EFFECTMESH_DESC& GetEffectMeshDesc() { return m_EffectMeshDesc; }

	// [ �Ͻ� ���� ]
	void SetPause(_bool _bPause) { m_bPause = _bPause; }
	bool GetPause() { return m_bPause; }

	// [ LifeTime ]
	void SetLifeTime(_float2 _vLifeTime) { m_EffectMeshDesc.vLifeTime = _vLifeTime; }
	_float2 GetLifeTime() { return m_EffectMeshDesc.vLifeTime; }

	// [ Origin Transform ]
	void SetOriginScale(_float4 _vOriginScale) { m_vOriginScale = _vOriginScale; }
	_float4 GetOriginScale() { return m_vOriginScale; }

	void SetOriginRotate(_float4 _vOriginRotate) { m_vOriginRotation = _vOriginRotate; }
	_float4 GetOriginRotate() { return m_vOriginRotation; }

/********************
	Data Members
********************/
protected:
	// [ EffectMesh ������Ʈ ]
	weak_ptr<CEffectMesh> m_wpEffectMesh;

	// [ EFFECTMESH_DESC ]
	EFFECTMESH_DESC m_EffectMeshDesc;

protected:
	// [ JSON ���ϸ� ]
	wstring m_wstrJSONFileName = L"";

protected:
	// [ ���� LifeTime ]
	_float m_fLifeTimeAcc = 0.0f;

	_float m_fTimeDelta = 0.0f;

	// [ UV Move Distance ]
	_float2 m_vUVMoveDistance = {};

	// [ Transform Change ]
	_float4 m_vOriginScale = {};
	_float4 m_vOriginRotation = {};

	// [ �Ͻ� ���� ]
	_bool m_bPause = false;
};

END