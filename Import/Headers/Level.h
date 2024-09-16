#pragma once

/* CLevel */
// 1. [레벨]은 [레이어]를 소유합니다.

BEGIN(Engine)

class CLayer;
class CActor;

class ENGINE_DLL CLevel : public enable_shared_from_this<CLevel>
{
public:
	explicit CLevel() = default;
	virtual ~CLevel() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize(_uint _iNumLayers);
	virtual _int PreUpdate(const _float& _fTimeDelta);
	virtual _int Update(const _float& _fTimeDelta);
	virtual _int LateUpdate(const _float& _fTimeDelta);
	virtual void Render();
	virtual void EndPlay();
	virtual void Release();

/********************
	Methods
********************/
public:
	// [ 레이어 초기화 ]
	// : 정해진 레이어 개수에 따라 레벨의 레이어를 세팅합니다.
	HRESULT ReserveLayers(_uint _iNumLayers);

	// [ 레이어 생성 ]
	// : 레이어를 생성하여 벡터 컨테이너에 추가합니다.
	shared_ptr<CLayer> CreateLayer(_uint _iLayerIndex);

	// [ 레이어 검색 ]
	// : 인덱스 번호에 맞는 레이어를 찾아서 쉐어드 포인터를 반환합니다.
	shared_ptr<CLayer> FindLayer(_uint _iLayerIndex);

public:
	HRESULT LoadLevelDataFromFileName(const wstring& _wstrLevelName);
		void LoadPrefab(const wstring& _wstrLevelName);
		virtual void LoadSpawn(const wstring& _wstrLevelName);

protected:
	// [ 조명 세팅 ]
	// : 현재 레벨에 필요한 조명을 세팅합니다.
	virtual HRESULT ReadyLights() { return S_OK; };

/********************
	Getter/Setter                                                                           
********************/
public:
	// [ 레벨 인덱스 ]
	void SetLevelIndex(_uint _iLevelIndex) { m_iLevelIndex = _iLevelIndex; }
	const _uint GetLevelIndex() { return m_iLevelIndex; }

public:
	shared_ptr<CLayer> GetLayer(_uint _iLayerIndex) { return m_vecLayers[_iLayerIndex]; }

/********************
	Data Members
********************/
protected:
	// [ 디바이스 ]
	ID3D11Device* m_pDevice = nullptr;

	// [ 디바이스 컨텍스트 ]
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

protected:
	// [ 레벨 인덱스 ]
	_int m_iLevelIndex = -1; // 기본 값을 -1로 주어서 레벨이 할당되지 않은 경우를 판단합니다.

	// [ 레벨의 레이어 개수 ]
	_uint m_iNumLayers = 0;

	// [ 레벨의 레이어들 ]
	vector<shared_ptr<CLayer>> m_vecLayers;
};

END