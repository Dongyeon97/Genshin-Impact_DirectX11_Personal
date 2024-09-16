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

	// [ JSON 파싱 용]
	virtual HRESULT Initialize(const wstring& _wstrJsonFileName);

	// [ 오브젝트 풀 용 ]
	virtual void RecycleBegin();

/********************
	Methods
********************/
public:
	// [ UV Move Mode : off ]
	virtual HRESULT BindShaderResources();

	// [ Json 파싱 ]
	HRESULT ReadEffectMeshFile_JSON(const wstring& _wstrJsonFileName);

	// [ Transform Change ]
	void TransformChange(const _float& _fTimeDelta);

	/* 툴 용 */
	void Replay();

/********************
	Getter / Setter
********************/
public:
	// [ JSON 파일명 ]
	void SetJsonFileName(const wstring& _wstrJsonFileName) { m_wstrJSONFileName = _wstrJsonFileName; }
	const wstring& GetJsonFileName() { return m_wstrJSONFileName; }

	/* 툴 용 */
	// [ EFFECT_DESC 세팅 ]
	void SetEffectMeshDesc(EFFECTMESH_DESC _EffectMeshDesc) { m_EffectMeshDesc = _EffectMeshDesc; }
	EFFECTMESH_DESC& GetEffectMeshDesc() { return m_EffectMeshDesc; }

	// [ 일시 정지 ]
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
	// [ EffectMesh 컴포넌트 ]
	weak_ptr<CEffectMesh> m_wpEffectMesh;

	// [ EFFECTMESH_DESC ]
	EFFECTMESH_DESC m_EffectMeshDesc;

protected:
	// [ JSON 파일명 ]
	wstring m_wstrJSONFileName = L"";

protected:
	// [ 누적 LifeTime ]
	_float m_fLifeTimeAcc = 0.0f;

	_float m_fTimeDelta = 0.0f;

	// [ UV Move Distance ]
	_float2 m_vUVMoveDistance = {};

	// [ Transform Change ]
	_float4 m_vOriginScale = {};
	_float4 m_vOriginRotation = {};

	// [ 일시 정지 ]
	_bool m_bPause = false;
};

END