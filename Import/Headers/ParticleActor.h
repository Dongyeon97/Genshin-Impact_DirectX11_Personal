#pragma once
#include "Actor.h"
#include "Particle_Point.h"

BEGIN(Engine)

class CParticle_Point;

class ENGINE_DLL CParticleActor : public CActor
{
public:
	explicit CParticleActor() = default;
	virtual ~CParticleActor() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	virtual void Render() override;
	virtual void EndPlay() override;
	virtual void Release() override;

	// �� ��� �̰ɷ� Initialize ��Ű�� ��
	HRESULT Initialize(const wstring& _wstrJsonFileName);

	virtual void RecycleBegin();

/********************
	Methods
********************/
public:
	HRESULT ReadParticleFile_JSON(const wstring& _wstrJsonFileName);

private:
	HRESULT BindDefaultShaderResources();

/********************
	Getter / Setter
********************/
public:
	// [ JSON ���ϸ� ]
	void SetJsonFileName(const wstring& _wstrJsonFileName) { m_wstrJSONFileName = _wstrJsonFileName; }
	const wstring& GetJsonFileName() { return m_wstrJSONFileName; }

	// [ ��ƼŬ ����Ʈ ������Ʈ ]
	shared_ptr<CParticle_Point> GetParticleComponent() { return m_wpParticle_Point.lock(); }

	// [ �ν��Ͻ� ���� ]
	void SetNumInstance(_uint _iNumInstance) { m_InstanceDesc.iNumInstance = _iNumInstance; }
	_uint GetNumInstance() { return m_InstanceDesc.iNumInstance; }

	// [ �ν��Ͻ� DESC ]
	void SetInstanceDesc(INSTANCE_DESC _InstanceDesc) { m_InstanceDesc = _InstanceDesc; }
	INSTANCE_DESC& GetInstanceDesc() { return m_InstanceDesc; }

	// [ ��ƼŬ ������Ʈ ���� ]
	void SetShaderFileInfo(const wstring& _wstrShaderFileName) { m_wstrShaderFileName = _wstrShaderFileName; }
	void SetTextureFileName(const wstring& _wstrTextureFileName) { m_wstrTextureFileName = _wstrTextureFileName; }

	// [ �Ͻ� ���� ]
	void SetPause(_bool _bPause) { m_wpParticle_Point.lock()->SetPause(_bPause); }
	bool GetPause() { return m_wpParticle_Point.lock()->GetPause(); }

	// [ LifeTime ]
	void SetLifeTime(_float2 _vLifeTime) { m_wpParticle_Point.lock()->SetLifeTime(_vLifeTime); }
	_float2 GetLifeTime() { return m_wpParticle_Point.lock()->GetLifeTime(); }

/********************
	Data Members
********************/
protected:
	weak_ptr<CParticle_Point> m_wpParticle_Point;

	wstring m_wstrJSONFileName = L"";

protected:
	// [ �ν��Ͻ� ������Ʈ�� �ʿ��� ���� ]
	wstring m_wstrShaderFileName = {};
	wstring m_wstrTextureFileName = {};

protected:
	// [ �ν��Ͻ� ���� ]
	INSTANCE_DESC	m_InstanceDesc;
};

END