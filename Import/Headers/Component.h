#pragma once
#include "Object.h"

BEGIN(Engine)

class CLevel;
class CActor;

class ENGINE_DLL CComponent : public CObject
{
public:
	explicit CComponent() = default;
	virtual ~CComponent() = default;

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

	// �� �Ʒ��� �ϴ� ���� �س��� �͵��̴� ���߿� Actor->Component�� ������ ��!
/********************
	Methods
********************/
public:
	// �޼ҵ�� �߰� ����


/********************
	Getter/Setter
********************/
public:
	// // [ ������Ʈ�� ���� ���� ]
	// void SetLevel(weak_ptr<CLevel> _wpLevel) { m_wpLevel = _wpLevel; }
	// weak_ptr<CLevel> GetLevel() { assert(!m_wpLevel.expired()); return m_wpLevel; }
	// 
	// // [ ������Ʈ�� ���� ���̾� ]
	// void SetLayer(weak_ptr<CLayer> _wpLayer) { m_wpLayer = _wpLayer; }
	// weak_ptr<CLayer> GetLayer() { assert(!m_wpLayer.expired()); return m_wpLayer; }
	// 
	// // [ ������Ʈ �̸� ]
	// void SetComponentName(const wstring _wstrComponentName) { m_wstrActorName = m_wstrComponentName; }
	// const wstring GetComponentName() { return m_wstrComponentName; }

/********************
	Data Members
********************/
protected:
	// [ ����̽� ]
	// ID3D11Device* m_pDevice = { nullptr };
	// 
	// // [ ����̽� ���ؽ�Ʈ ]
	// ID3D11DeviceContext* m_pDeviceContext = { nullptr };

protected:
	// [ ������Ʈ�� ���� ���� ]
	// weak_ptr<CLevel> m_wpLevel;

	// [ ������Ʈ�� ���� ���̾� ]
	// weak_ptr<CLayer> m_wpLayer;

	// [ ������Ʈ �̸� ]
	// wstring m_wstrComponentName = L"";
};

END