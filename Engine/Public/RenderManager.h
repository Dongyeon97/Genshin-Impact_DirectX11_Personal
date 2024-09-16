#pragma once

/* CRenderManager */
// 1. RenderManager는 반드시 DeviceManager의 초기화 이후에 생성되어야 합니다. 초기화된 장치를 가지고 있어야 하기 때문
// 2. 직교, 원근, 알파블렌드 여부 등에 따라서 렌더 순서를 결정하고, 렌더합니다.
// 3. 렌더할 오브젝트들을 삽입, 삭제가 용이한 리스트 컨테이너로 관리합니다.(매 프레임 삽입, 삭제가 이루어지기 때문)

BEGIN(Engine)

class CActor;
class CRect_RenderTarget;
class CPrimitiveComponent;

class ENGINE_DLL CRenderManager final
{
	DECLARE_SINGLETON(CRenderManager)

public:
	HRESULT Initialize();
	HRESULT Render();
	void Release();

public:
	HRESULT AddObjectToRenderGroup(RENDER_GROUP _eRenderGroup, shared_ptr<CActor> _spActor);

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_OutLine();
	HRESULT Render_Lights();
	HRESULT Render_Final();

	HRESULT Render_NonLight();
	HRESULT Render_Glow();
	HRESULT Render_Blend();

	HRESULT Render_PostProcessing();

	HRESULT Render_Orthogonal();
	HRESULT Render_UI();

#ifdef _DEBUG
public:
	HRESULT AddDebugComponent(shared_ptr<CPrimitiveComponent> _spPrimitiveComponent);

private:
	HRESULT Render_Debug();
#endif // _DEBUG

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:
	shared_ptr<CRect_RenderTarget> m_spRect_RenderTarget = nullptr;

	ID3D11DepthStencilView* m_pShadowDepthStencilView = nullptr;

private:
	// [ 렌더타겟 버퍼를 화면에 그려내기 위한 직교 투영을 위한 행렬들 ]
	_float4x4 m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

private:
	// [ 렌더 대상 액터들의 리스트 ]
	list<shared_ptr<CActor>> m_lstRenderActors[RENDER_END];

#ifdef _DEBUG
	// [ 디버그용으로 렌더링 할 컴포넌트들 ]
	list<shared_ptr<CPrimitiveComponent>> m_lstDebugRenderComponents;
#endif // _DEBUG
};

END