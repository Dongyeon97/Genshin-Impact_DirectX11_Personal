#pragma once
#include "Object.h"

/* ActorComponent�� �پ��� ������ ���Ϳ� �߰��� �� �ִ� ���� ������ ������ �����ϴ� ���� ����� �⺻ Ŭ�����Դϴ�. */

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
	// [ ������Ʈ�� ���� ���� ]
	void SetLevel(weak_ptr<CLevel> _wpLevel) { m_wpLevel = _wpLevel; }
	weak_ptr<CLevel> GetLevel() { assert(!m_wpLevel.expired()); return m_wpLevel; }
 
	 // [ ������Ʈ�� ���� ���̾� ]
	 void SetLayer(weak_ptr<CLayer> _wpLayer) { m_wpLayer = _wpLayer; }
	 weak_ptr<CLayer> GetLayer() { assert(!m_wpLayer.expired()); return m_wpLayer; }

	 // [ ������Ʈ�� ���� ���� ]
	 void SetOwnerActor(shared_ptr<CActor> _spOwnerActor) { m_wpOwnerActor = _spOwnerActor; }
	 weak_ptr<CActor> const GetOwnerActor() { assert(!m_wpOwnerActor.expired()); return m_wpOwnerActor; }
	 
	 // [ ������Ʈ �̸� ]
	 void SetComponentName(const wstring& _wstrComponentName) { m_wstrComponentName = _wstrComponentName; }
	 const wstring& GetComponentName() { return m_wstrComponentName; }

/********************
	Data Members
********************/
protected:
	

protected:
	// [ ������Ʈ�� ���� ���� ]
	weak_ptr<CLevel> m_wpLevel;

	// [ ������Ʈ�� ���� ���̾� ]
	weak_ptr<CLayer> m_wpLayer;

	// [ ������Ʈ�� ���� ���� ]
	weak_ptr<CActor> m_wpOwnerActor;

	// [ ������Ʈ �̸� ]
	wstring m_wstrComponentName = L"";
};

END