#pragma once

/* CRenderManager */
// 1. RenderManager�� �ݵ�� DeviceManager�� �ʱ�ȭ ���Ŀ� �����Ǿ�� �մϴ�. �ʱ�ȭ�� ��ġ�� ������ �־�� �ϱ� ����
// 2. ����, ����, ���ĺ��� ���� � ���� ���� ������ �����ϰ�, �����մϴ�.
// 3. ������ ������Ʈ���� ����, ������ ������ ����Ʈ �����̳ʷ� �����մϴ�.(�� ������ ����, ������ �̷������ ����)

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
	// [ ����Ÿ�� ���۸� ȭ�鿡 �׷����� ���� ���� ������ ���� ��ĵ� ]
	_float4x4 m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

private:
	// [ ���� ��� ���͵��� ����Ʈ ]
	list<shared_ptr<CActor>> m_lstRenderActors[RENDER_END];

#ifdef _DEBUG
	// [ ����׿����� ������ �� ������Ʈ�� ]
	list<shared_ptr<CPrimitiveComponent>> m_lstDebugRenderComponents;
#endif // _DEBUG
};

END