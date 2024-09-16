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

	// 이 아래로 일단 복붙 해놓은 것들이니 나중에 Actor->Component로 변경할 것!
/********************
	Methods
********************/
public:
	// 메소드들 추가 예정


/********************
	Getter/Setter
********************/
public:
	// // [ 컴포넌트가 속한 레벨 ]
	// void SetLevel(weak_ptr<CLevel> _wpLevel) { m_wpLevel = _wpLevel; }
	// weak_ptr<CLevel> GetLevel() { assert(!m_wpLevel.expired()); return m_wpLevel; }
	// 
	// // [ 컴포넌트가 속한 레이어 ]
	// void SetLayer(weak_ptr<CLayer> _wpLayer) { m_wpLayer = _wpLayer; }
	// weak_ptr<CLayer> GetLayer() { assert(!m_wpLayer.expired()); return m_wpLayer; }
	// 
	// // [ 컴포넌트 이름 ]
	// void SetComponentName(const wstring _wstrComponentName) { m_wstrActorName = m_wstrComponentName; }
	// const wstring GetComponentName() { return m_wstrComponentName; }

/********************
	Data Members
********************/
protected:
	// [ 디바이스 ]
	// ID3D11Device* m_pDevice = { nullptr };
	// 
	// // [ 디바이스 컨텍스트 ]
	// ID3D11DeviceContext* m_pDeviceContext = { nullptr };

protected:
	// [ 컴포넌트가 속한 레벨 ]
	// weak_ptr<CLevel> m_wpLevel;

	// [ 컴포넌트가 속한 레이어 ]
	// weak_ptr<CLayer> m_wpLayer;

	// [ 컴포넌트 이름 ]
	// wstring m_wstrComponentName = L"";
};

END