#pragma once
#include "NonMeshComponent.h"

BEGIN(Engine)

class ENGINE_DLL CInstancing abstract : public CNonMeshComponent
{
public:
	explicit CInstancing() = default;
	virtual ~CInstancing() = default;

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

/********************
	Methods
********************/
public:
	virtual HRESULT BindBuffers() override;

	void RecycleBegin();

	void Tick_Drop(const _float& _fTimeDelta);
	void Tick_Spread(const _float& _fTimeDelta);
	void Tick_Spread_Radius(const _float& _fTimeDelta);
	void Tick_Sprite(const _float& _fTimeDelta);

/********************
	Getter / Setter
********************/
public:
	// [ 인스턴싱 할 개수 ]
	void SetNumInstance(_uint _iNumInstance) { m_InstanceDesc.iNumInstance = _iNumInstance; }
	_uint GetNumInstance() { return m_InstanceDesc.iNumInstance; }

	// [ 인스턴스 DESC ]
	void SetInstanceDesc(INSTANCE_DESC _InstanceDesc) { m_InstanceDesc = _InstanceDesc; }
	INSTANCE_DESC& GetInstanceDesc() { return m_InstanceDesc; }

	// [ 종료 ]
	void SetIsFinish(_bool _bFinished) { m_isFinished = _bFinished; }
	_bool GetIsFinish() { return m_isFinished; }

	// [ TimeAcc ]
	void SetTimeAcc(_float _fTimeAcc) { m_fTimeAcc = _fTimeAcc; }
	_float GetTimeAcc() { return m_fTimeAcc; }

	// [ LifeTime ]
	void SetLifeTime(_float2 _vLifeTime) { m_InstanceDesc.vLifeTime = _vLifeTime; }
	_float2 GetLifeTime() { return m_InstanceDesc.vLifeTime; }

	// [ 일시 정지 ]
	void SetPause(_bool _bPause) { m_isPause = _bPause; }
	bool GetPause() { return m_isPause; }

/********************
	Data Members
********************/
protected:
#pragma region 컴 객체
	ID3D11Buffer*	m_pVBInstance = nullptr;
	random_device	m_RandomDevice;
	mt19937_64		m_RandomNumber;
#pragma endregion 컴 객체

	// [ INSTANCE_DESC ]
	INSTANCE_DESC	m_InstanceDesc;

protected:
	_uint m_iInstanceStride = 0;
	_uint m_iIndexCountPerInstance = 0;

protected:
	_float*		m_pSpeed = { nullptr };
	_float*		m_pLifeTime = { nullptr };
	_float3*	m_pRotation = { nullptr };

	_float		m_fTimeAcc = 0.0f;
	_bool		m_isFinished = false;
	_bool		m_isPause = false;
};

END