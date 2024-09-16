#include "Engine_pch.h"
#include "NonMeshComponent.h"

#include "DeviceManager.h"
#include "ResourceManager.h"

void CNonMeshComponent::Initialize()
{
	// 프리미티브 컴포넌트를 상속받은 컴포넌트는 초기화시에 반드시 디바이스를 세팅합니다.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	CreateShader(m_wstrShaderFileName, m_pElements, m_iNumElements);

	// 텍스처
	CNonMeshComponent::CreateTexture();

	CPrimitiveComponent::Initialize();
}

_int CNonMeshComponent::PreUpdate(const _float& _fTimeDelta)
{

	return CPrimitiveComponent::PreUpdate(_fTimeDelta);
}

_int CNonMeshComponent::Update(const _float& _fTimeDelta)
{

	return CPrimitiveComponent::Update(_fTimeDelta);
}

_int CNonMeshComponent::LateUpdate(const _float& _fTimeDelta)
{

	return CPrimitiveComponent::LateUpdate(_fTimeDelta);
}

void CNonMeshComponent::Render()
{
	CPrimitiveComponent::Render();

	// 맨 자식 컴포넌트에 이거 써주는거 까먹지 말자
	// m_pDeviceContext->DrawIndexed(m_iNumIndices, 0, 0);
}

void CNonMeshComponent::EndPlay()
{

	CPrimitiveComponent::EndPlay();
}

void CNonMeshComponent::Release()
{
	if (m_pEffect)
		Safe_Release(m_pEffect);

	for (auto& iter : m_vecInputLayouts)
		Safe_Release(iter);

	CPrimitiveComponent::Release();
}

HRESULT CNonMeshComponent::CreateTexture()
{
	m_vecSRV.resize(m_iEndIndex - m_iBeginIndex + 1);
	m_vecSRV.reserve(m_iEndIndex - m_iBeginIndex + 1);

	while (true)
	{
		if (!iswdigit(m_wstrTextureFileName.back())) { break; }
		m_wstrTextureFileName.pop_back();
	}

	if (LEVEL_DYNAMIC == m_eLevelType)
	{
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecSRV = CResourceManager::GetInstance()->GetSRVs();
		for (_uint iIndex = m_iBeginIndex; iIndex <= m_iEndIndex; ++iIndex)
		{
			const wstring& wstrFileName = m_wstrTextureFileName + to_wstring(iIndex);

			for (auto& pair : vecSRV)
			{
				if (pair.first == wstrFileName)
				{
					m_vecSRV[iIndex] = pair.second;
					break;
				}
			}
		}
	}
	else if (LEVEL_STATIC == m_eLevelType)
	{
		const vector<pair<wstring, ID3D11ShaderResourceView*>>& vecSRV_Static = CResourceManager::GetInstance()->GetSRVs_Static();
		for (_uint iIndex = m_iBeginIndex; iIndex <= m_iEndIndex; ++iIndex)
		{
			const wstring& wstrFileName = m_wstrTextureFileName + to_wstring(iIndex);

			for (auto& pair : vecSRV_Static)
			{
				if (pair.first == wstrFileName)
				{
					m_vecSRV[iIndex] = pair.second;
					break;
				}
			}
		}
	}

	return S_OK;
}

void CNonMeshComponent::UpdateVerticesTransform()
{
	if (!m_vecVertexPos.empty())
	{
		for (_uint i = 0; i < m_iNumVertices; ++i)
		{
			XMStoreFloat4(&m_vecVertexWorldPos[i], XMVector4Transform(XMLoadFloat4(&m_vecVertexPos[i]), XMLoadFloat4x4(&m_matWorld)));
		}
	}
}

void CNonMeshComponent::SetTextureFileInfo(const wstring& _wstrTextureFileName, _uint _iBeginIndex, _uint _iEndIndex)
{
	m_wstrTextureFileName = _wstrTextureFileName;
	m_iBeginIndex = _iBeginIndex;
	m_iEndIndex = _iEndIndex;
}

HRESULT CNonMeshComponent::BindShaderResource(const _char* _pConstantName, _uint _iIndex)
{
	if (_iIndex > m_iEndIndex ||
		_iIndex < 0)
		return E_FAIL;

	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	if (nullptr == m_vecSRV[_iIndex])
	{
		MSG_BOX("Failed To BindShaderResource. Texture is null. : NonMeshComponent");
		return E_FAIL;
	}

	return pSRVariable->SetResource(m_vecSRV[_iIndex]);
}

HRESULT CNonMeshComponent::BindShaderResources(const _char* _pConstantName)
{

	return S_OK;
}

HRESULT CNonMeshComponent::BeginShader(_uint _iPassIndex, _uint _iTechniqueIndex)
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

HRESULT CNonMeshComponent::BindRawValue(const _char* _pConstantName, const void* _pData, _uint _iLength)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(_pData, 0, _iLength);
}

HRESULT CNonMeshComponent::BindMatrix(const _char* _pConstantName, const _float4x4* _pMatrix)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix((_float*)_pMatrix);
}

HRESULT CNonMeshComponent::BindMatrices(const _char* _pConstantName, const _float4x4* _pMatrices, _uint _iNumMatrices)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(_pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrixArray((_float*)_pMatrices, 0, _iNumMatrices);
}

HRESULT CNonMeshComponent::CreateShader(const wstring& _wstrShaderFileName, const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements)
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

void CNonMeshComponent::SetShaderFileInfo(const wstring& _wstrShaderFileName)
{
	m_wstrShaderFileName = _wstrShaderFileName;
}

void CNonMeshComponent::SetVertexInfo(const D3D11_INPUT_ELEMENT_DESC* _pElements, _uint _iNumElements)
{
	m_pElements = _pElements;
	m_iNumElements = _iNumElements;
}
