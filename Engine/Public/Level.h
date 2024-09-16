#pragma once

/* CLevel */
// 1. [����]�� [���̾�]�� �����մϴ�.

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
	// [ ���̾� �ʱ�ȭ ]
	// : ������ ���̾� ������ ���� ������ ���̾ �����մϴ�.
	HRESULT ReserveLayers(_uint _iNumLayers);

	// [ ���̾� ���� ]
	// : ���̾ �����Ͽ� ���� �����̳ʿ� �߰��մϴ�.
	shared_ptr<CLayer> CreateLayer(_uint _iLayerIndex);

	// [ ���̾� �˻� ]
	// : �ε��� ��ȣ�� �´� ���̾ ã�Ƽ� ����� �����͸� ��ȯ�մϴ�.
	shared_ptr<CLayer> FindLayer(_uint _iLayerIndex);

public:
	HRESULT LoadLevelDataFromFileName(const wstring& _wstrLevelName);
		void LoadPrefab(const wstring& _wstrLevelName);
		virtual void LoadSpawn(const wstring& _wstrLevelName);

protected:
	// [ ���� ���� ]
	// : ���� ������ �ʿ��� ������ �����մϴ�.
	virtual HRESULT ReadyLights() { return S_OK; };

/********************
	Getter/Setter                                                                           
********************/
public:
	// [ ���� �ε��� ]
	void SetLevelIndex(_uint _iLevelIndex) { m_iLevelIndex = _iLevelIndex; }
	const _uint GetLevelIndex() { return m_iLevelIndex; }

public:
	shared_ptr<CLayer> GetLayer(_uint _iLayerIndex) { return m_vecLayers[_iLayerIndex]; }

/********************
	Data Members
********************/
protected:
	// [ ����̽� ]
	ID3D11Device* m_pDevice = nullptr;

	// [ ����̽� ���ؽ�Ʈ ]
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

protected:
	// [ ���� �ε��� ]
	_int m_iLevelIndex = -1; // �⺻ ���� -1�� �־ ������ �Ҵ���� ���� ��츦 �Ǵ��մϴ�.

	// [ ������ ���̾� ���� ]
	_uint m_iNumLayers = 0;

	// [ ������ ���̾�� ]
	vector<shared_ptr<CLayer>> m_vecLayers;
};

END