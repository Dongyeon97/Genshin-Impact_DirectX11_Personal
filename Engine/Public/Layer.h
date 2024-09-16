#pragma once

/* CLayer */
// 1. [레이어]는 [액터]를 소유합니다.
// 2. [액터]를 [생성], [추가], [검색], [삭제]합니다.

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
	// [ 액터 생성 ] + Initialize()
	// : 액터 문자열을 키(key)로 하는 액터을 검색합니다.
	// : 해당하는 이름의 액터가 없는 경우, 이름을 키 값으로 액터를 추가합니다.
	// : 생성이 성공하면, 액터의 쉐어드 포인터를 반환합니다.
	template<typename T, typename = enable_if_t<is_base_of_v<CActor, T>>>
	shared_ptr<T> CreateActor(const wstring& _wstrActor)
	{
		if (m_umapActors.find(_wstrActor) != m_umapActors.end()) { return nullptr; }

		shared_ptr<T> spActor = std::make_shared<T>();
		spActor->SetLevel(m_wpLevel);
		spActor->SetLayer(shared_from_this());
		spActor->SetActorName(_wstrActor);
		spActor->SetActorUse(true);

		// Client에서 레벨 생성시에 액터는 레벨의 초기화가 끝난 Initialize() 이후 시점에서 추가되기 때문에 액터->Initialize()까지 수행해준다.
		// spActor->Initialize();

		// 컨테이너에 액터를 추가합니다.
		m_umapActors.insert({ _wstrActor, spActor });
		return spActor;
	}

	// [ 액터를 오브젝트 풀에 추가 ]
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

		// 오브젝트풀에 액터를 추가합니다.
		m_umapObjectPool.emplace(wstrActorVariation, vecActors);
		m_umapObjectPool[wstrActorVariation].push_back(spActor);

		return spActor;
	}

	shared_ptr<CActor> RecycleFromPool(const wstring& _wstrActorVariation);

	// [ (단순) 액터 삽입 ]
	void InsertActor(const wstring& _wstrActor, shared_ptr<CActor> _spActor)
	{
		m_umapActors.insert({ _wstrActor, _spActor });
	}

	// [ 액터 검색 ]
	// : 이름을 키 값으로 액터를 검색합니다.
	// : 검색이 성공하면, 액터의 쉐어드 포인터를 반환합니다.
	shared_ptr<CActor> FindActor(const wstring& _wstrActor);
	shared_ptr<CActor> FindActor_Pool (const wstring& _wstrActorVariation, const wstring& _wstrActorName);

	// [ 액터 삭제 ]
	// : 이름을 키 값으로 액터를 삭제합니다.
	void RemoveActor(const wstring& _wstrActor);
	void RemoveActor_Pool(const wstring& _wstrActor);

/********************
	Getter/Setter
********************/
public:
	// [ 레이어가 속한 레벨 ]
	void SetLevel(weak_ptr<CLevel> _wpLevel) { m_wpLevel = _wpLevel; }
	const weak_ptr<CLevel> GetLevel() { return m_wpLevel; }

	// [ 레이어 인덱스 ]
	void SetLayerIndex(_uint _iLayerIndex) { m_iLayerIndex = _iLayerIndex; }
	const _uint GetLayerIndex() { return m_iLayerIndex; }

	// [ 소유한 액터들이 속해있는 umap ]
	const unordered_map<wstring, shared_ptr<CActor>>& GetActors() { return m_umapActors; }
	const unordered_map<wstring, vector<shared_ptr<CActor>>>& GetActors_Pool() { return m_umapObjectPool; }

/********************
	Data Members
********************/
protected:
	// [ 디바이스 ]
	ID3D11Device* m_pDevice = { nullptr };

	// [ 디바이스 컨텍스트 ]
	ID3D11DeviceContext* m_pDeviceContext = { nullptr };

protected:
	// [ 레이어가 속한 레벨 ]
	weak_ptr<CLevel> m_wpLevel;

	// [ 레이어의 인덱스 ]
	_int m_iLayerIndex = -1; // 기본 값을 -1로 주어서 레이어가 할당되지 않은 경우를 판단합니다.

	// [ 레이어가 소유하고 있는 액터들 ]
	unordered_map<wstring, shared_ptr<CActor>> m_umapActors;

	// [ 레이어의 오브젝트 풀 ] -> 키 : 액터의 큰 대분류(ex.츄츄족 아이스 : Hili_Ice) / 밸류 : 액터
	unordered_map<wstring, vector<shared_ptr<CActor>>> m_umapObjectPool;
};

END