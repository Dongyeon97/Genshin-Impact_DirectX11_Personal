#include "Engine_pch.h"
#include "VIBuffer_Rect_RenderTarget.h"

#include "ResourceManager.h"
#include "DeviceManager.h"

void CVIBuffer_Rect_RenderTarget::Initialize()
{
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	InitializeBuffer();
	CreateShader(m_wstrShaderFileName, m_pElements, m_iNumElements);

	CPrimitiveComponent::Initialize();
}

_int CVIBuffer_Rect_RenderTarget::PreUpdate(const _float& _fTimeDelta)
{
	return CPrimitiveComponent::PreUpdate(_fTimeDelta);
}

_int CVIBuffer_Rect_RenderTarget::Update(const _float& _fTimeDelta)
{
	return CPrimitiveComponent::Update(_fTimeDelta);
}

_int CVIBuffer_Rect_RenderTarget::LateUpdate(const _float& _fTimeDelta)
{
	return CPrimitiveComponent::LateUpdate(_fTimeDelta);
}

void CVIBuffer_Rect_RenderTarget::Render()
{
	CPrimitiveComponent::Render();
}

void CVIBuffer_Rect_RenderTarget::EndPlay()
{
	CPrimitiveComponent::EndPlay();
}

void CVIBuffer_Rect_RenderTarget::Release()
{
	for (auto& pInputLayout : m_vecInputLayouts)
		Safe_Release(pInputLayout);

	if(m_pEffect->IsValid())
		Safe_Release(m_pEffect);

	CPrimitiveComponent::Release();
}

CVIBuffer_Rect_RenderTarget* CVIBuffer_Rect_RenderTarget::Create()
{
	CVIBuffer_Rect_RenderTarget* pInstance = new CVIBuffer_Rect_RenderTarget();

	pInstance->Initialize();

	return pInstance;
}

HRESULT CVIBuffer_Rect_RenderTarget::BindShaderResource(const _char* _pConstantName, ID3D11ShaderResourceView* pSRV)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResource(pSRV);
}

HRESULT CVIBuffer_Rect_RenderTarget::BeginShader(_uint _iPassIndex, _uint _iTechniqueIndex)
{
	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(_iTechniqueIndex);
	if (nullptr == pTechnique)
		return E_FAIL;

	ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(_iPassIndex);
	if (nullptr == pPass)
		return E_FAIL;

	pPass->Apply(0, m_pDeviceContext);

	m_pDeviceContext->IASetInputLayout(m_vecInputLayouts[_iPassIndex]);

	return S_OK;
}

HRESULT CVIBuffer_Rect_RenderTarget::BindRawValue(const _char* _pConstantName, const void* _pData, _uint _iLength)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(_pData, 0, _iLength);
}

HRESULT CVIBuffer_Rect_RenderTarget::BindMatrix(const _char* _pConstantName, const _float4x4* _pMatrix)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix((_float*)_pMatrix);
}

HRESULT CVIBuffer_Rect_RenderTarget::BindMatrices(const _char* _pConstantName, const _float4x4* _pMatrices, _uint _iNumMatrices)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrixArray((_float*)_pMatrices, 0, _iNumMatrices);
}

HRESULT CVIBuffer_Rect_RenderTarget::InitializeBuffer()
{
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iNumVertices = 4;

	m_iIndexStride = 2;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_iNumIndices = 6;
	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	/* 정점버퍼를 만든다.*/
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::CreateBuffer(&m_pVertexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	/* 인덱스버퍼를 만든다.*/
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::CreateBuffer(&m_pIndexBuffer)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Rect_RenderTarget::CreateShader(const wstring& _wstrShaderFileName, const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements)
{
	_uint		iHlslFlag = { 0 };

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	const vector<wstring>& vecShaderFilePaths = CResourceManager::GetInstance()->GetFilePaths_Static(RESOURCE_SHADER);
	for (auto& wstrFilePath : vecShaderFilePaths)
	{
		// 파일 경로에서 확장자를 제외한 파일의 이름만 추출합니다.
		size_t iCursor = wstrFilePath.find_last_of(L"\\/");

		wstring wstrFileNameExt = L"";
		if (iCursor != string::npos) { wstrFileNameExt = wstrFilePath.substr(iCursor + 1); }
		else { wstrFileNameExt = wstrFilePath; }

		iCursor = wstrFileNameExt.find_last_of(L".");

		// 파일의 확장자(.png)
		const wstring& wstrFileExtension = wstrFileNameExt.substr(iCursor + 1);

		// 파일의 이름("WhiteWall0")
		const wstring& wstrFileName = wstrFileNameExt.substr(0, iCursor);

		// 확장자가 hlsl이고, Primitive의 m_wstrShaderFileName과 파일의 이름[wstrFileName]이 같다면
		if ((wstrFileExtension == L"hlsl" || wstrFileExtension == L"HLSL") &&
			wstrFileName == m_wstrShaderFileName)
		{
			// 경로로 받아온 셰이더파일을 컴파일하여 컴 객체화한다.
			if (FAILED(D3DX11CompileEffectFromFile(wstrFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
				return E_FAIL;

			D3DX11_EFFECT_DESC		EffectDesc = {};
			m_pEffect->GetDesc(&EffectDesc);

			for (_uint i = 0; i < EffectDesc.Techniques; i++)
			{
				ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(i);
				if (nullptr == pTechnique)
					return E_FAIL;

				D3DX11_TECHNIQUE_DESC		TechniqueDesc = {};
				pTechnique->GetDesc(&TechniqueDesc);

				for (_uint j = 0; j < TechniqueDesc.Passes; j++)
				{
					ID3D11InputLayout* pInputLayout = { nullptr };

					ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(j);
					if (nullptr == pPass)
						return E_FAIL;

					D3DX11_PASS_DESC		PassDesc = {};
					pPass->GetDesc(&PassDesc);

					if (FAILED(m_pDevice->CreateInputLayout(_pElements, _iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
						return E_FAIL;

					m_vecInputLayouts.push_back(pInputLayout);
				}
			}
		}
	}

	return S_OK;
}

void CVIBuffer_Rect_RenderTarget::SetShaderFileInfo(const wstring& _wstrShaderFileName)
{
	m_wstrShaderFileName = _wstrShaderFileName;
}

void CVIBuffer_Rect_RenderTarget::SetVertexInfo(const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements)
{
	m_pElements = _pElements;
	m_iNumElements = _iNumElements;
}
