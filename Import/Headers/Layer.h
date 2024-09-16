#pragma once

/* CLayer */
// 1. [���̾�]�� [����]�� �����մϴ�.
// 2. [����]�� [����], [�߰�], [�˻�], [����]�մϴ�.

BEGIN(Engine)

class CLevel;
class CActor;

class ENGINE_DLL CLayer final : public enable_shared_from_this<CLayer>
{
public:
	explicit CLayer() = default;
	virtual ~CLayer() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize();
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
	// [ ���� ���� ] + Initialize()
	// : ���� ���ڿ��� Ű(key)�� �ϴ� ������ �˻��մϴ�.
	// : �ش��ϴ� �̸��� ���Ͱ� ���� ���, �̸��� Ű ������ ���͸� �߰��մϴ�.
	// : ������ �����ϸ�, ������ ����� �����͸� ��ȯ�մϴ�.
	template<typename T, typename = enable_if_t<is_base_of_v<CActor, T>>>
	shared_ptr<T> CreateActor(const wstring& _wstrActor)
	{
		if (m_umapActors.find(_wstrActor) != m_umapActors.end()) { return nullptr; }

		shared_ptr<T> spActor = std::make_shared<T>();
		spActor->SetLevel(m_wpLevel);
		spActor->SetLayer(shared_from_this());
		spActor->SetActorName(_wstrActor);
		spActor->SetActorUse(true);

		// Client���� ���� �����ÿ� ���ʹ� ������ �ʱ�ȭ�� ���� Initialize() ���� �������� �߰��Ǳ� ������ ����->Initialize()���� �������ش�.
		// spActor->Initialize();

		// �����̳ʿ� ���͸� �߰��մϴ�.
		m_umapActors.insert({ _wstrActor, spActor });
		return spActor;
	}

	// [ ���͸� ������Ʈ Ǯ�� �߰� ]
	template<typename T, typename = enable_if_t<is_base_of_v<CActor, T>>>
	shared_ptr<T> CreateActorToPool(const wstring& _wstrActor)
	{
		if (m_umapObjectPool.find(_wstrActor) != m_umapObjectPool.end()) { return nullptr; }

		shared_ptr<T> spActor = std::make_shared<T>();
		spActor->SetLevel(m_wpLevel);
		spActor->SetLayer(shared_from_this());
		spActor->SetActorName(_wstrActor);
		spActor->SetActorUse(false);

		wstring wstrActorVariation = L"";
		vector<shared_ptr<CActor>> vecActors;

		for (_uint i = 0; i < _wstrActor.size(); ++i)
		{
			if (isdigit(_wstrActor[i])) break;
			wstrActorVariation += _wstrActor[i];
		}

		// ������ƮǮ�� ���͸� �߰��մϴ�.
		m_umapObjectPool.emplace(wstrActorVariation, vecActors);
		m_umapObjectPool[wstrActorVariation].push_back(spActor);

		return spActor;
	}

	shared_ptr<CActor> RecycleFromPool(const wstring& _wstrActorVariation);

	// [ (�ܼ�) ���� ���� ]
	void InsertActor(const wstring& _wstrActor, shared_ptr<CActor> _spActor)
	{
		m_umapActors.insert({ _wstrActor, _spActor });
	}

	// [ ���� �˻� ]
	// : �̸��� Ű ������ ���͸� �˻��մϴ�.
	// : �˻��� �����ϸ�, ������ ����� �����͸� ��ȯ�մϴ�.
	shared_ptr<CActor> FindActor(const wstring& _wstrActor);
	shared_ptr<CActor> FindActor_Pool (const wstring& _wstrActorVariation, const wstring& _wstrActorName);

	// [ ���� ���� ]
	// : �̸��� Ű ������ ���͸� �����մϴ�.
	void RemoveActor(const wstring& _wstrActor);
	void RemoveActor_Pool(const wstring& _wstrActor);

/********************
	Getter/Setter
********************/
public:
	// [ ���̾ ���� ���� ]
	void SetLevel(weak_ptr<CLevel> _wpLevel) { m_wpLevel = _wpLevel; }
	const weak_ptr<CLevel> GetLevel() { return m_wpLevel; }

	// [ ���̾� �ε��� ]
	void SetLayerIndex(_uint _iLayerIndex) { m_iLayerIndex = _iLayerIndex; }
	const _uint GetLayerIndex() { return m_iLayerIndex; }

	// [ ������ ���͵��� �����ִ� umap ]
	const unordered_map<wstring, shared_ptr<CActor>>& GetActors() { return m_umapActors; }
	const unordered_map<wstring, vector<shared_ptr<CActor>>>& GetActors_Pool() { return m_umapObjectPool; }

/********************
	Data Members
********************/
protected:
	// [ ����̽� ]
	ID3D11Device* m_pDevice = { nullptr };

	// [ ����̽� ���ؽ�Ʈ ]
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };

protected:
	// [ ���̾ ���� ���� ]
	weak_ptr<CLevel> m_wpLevel;

	// [ ���̾��� �ε��� ]
	_int m_iLayerIndex = -1; // �⺻ ���� -1�� �־ ���̾ �Ҵ���� ���� ��츦 �Ǵ��մϴ�.

	// [ ���̾ �����ϰ� �ִ� ���͵� ]
	unordered_map<wstring, shared_ptr<CActor>> m_umapActors;

	// [ ���̾��� ������Ʈ Ǯ ] -> Ű : ������ ū ��з�(ex.������ ���̽� : Hili_Ice) / ��� : ����
	unordered_map<wstring, vector<shared_ptr<CActor>>> m_umapObjectPool;
};

END