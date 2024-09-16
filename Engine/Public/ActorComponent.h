#pragma once
#include "Object.h"

/* ActorComponent는 다양한 유형의 액터에 추가할 수 있는 재사용 가능한 동작을 정의하는 구성 요소의 기본 클래스입니다. */

BEGIN(Engine)

class CLevel;
class CLayer;
class CActor;

class ENGINE_DLL CActorComponent : public CObject
{
public:
	explicit CActorComponent() = default;
	virtual ~CActorComponent() = default;

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


/********************
	Getter/Setter
********************/
public:
	// [ 컴포넌트가 속한 레벨 ]
	void SetLevel(weak_ptr<CLevel> _wpLevel) { m_wpLevel = _wpLevel; }
	weak_ptr<CLevel> GetLevel() { assert(!m_wpLevel.expired()); return m_wpLevel; }
 
	 // [ 컴포넌트가 속한 레이어 ]
	 void SetLayer(weak_ptr<CLayer> _wpLayer) { m_wpLayer = _wpLayer; }
	 weak_ptr<CLayer> GetLayer() { assert(!m_wpLayer.expired()); return m_wpLayer; }

	 // [ 컴포넌트가 속한 액터 ]
	 void SetOwnerActor(shared_ptr<CActor> _spOwnerActor) { m_wpOwnerActor = _spOwnerActor; }
	 weak_ptr<CActor> const GetOwnerActor() { assert(!m_wpOwnerActor.expired()); return m_wpOwnerActor; }
	 
	 // [ 컴포넌트 이름 ]
	 void SetComponentName(const wstring& _wstrComponentName) { m_wstrComponentName = _wstrComponentName; }
	 const wstring& GetComponentName() { return m_wstrComponentName; }

/********************
	Data Members
********************/
protected:
	

protected:
	// [ 컴포넌트가 속한 레벨 ]
	weak_ptr<CLevel> m_wpLevel;

	// [ 컴포넌트가 속한 레이어 ]
	weak_ptr<CLayer> m_wpLayer;

	// [ 컴포넌트가 속한 액터 ]
	weak_ptr<CActor> m_wpOwnerActor;

	// [ 컴포넌트 이름 ]
	wstring m_wstrComponentName = L"";
};

END