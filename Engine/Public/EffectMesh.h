
#pragma once
#include "ModelComponent.h"

BEGIN(Engine)

class ENGINE_DLL CEffectMesh : public CModelComponent
{
public:
	explicit CEffectMesh() = default;
	virtual ~CEffectMesh() = default;

/********************
	Framework
********************/
public:
	virtual void Initialize() override;
	virtual _int PreUpdate(const _float& _fTimeDelta) override;
	virtual _int Update(const _float& _fTimeDelta) override;
	virtual _int LateUpdate(const _float& _fTimeDelta) override;
	void Render(_uint _iMeshIndex);
	virtual void EndPlay() override;
	virtual void Release() override;

	// ※ 얘는 이걸로 Initialize 시키자 ※
	HRESULT Initialize(const wstring& _wstrJsonFileName);

/********************
	Methods
********************/
public:
	// 재질을 셰이더에 바인딩합니다.
	HRESULT BindEffectTexture(EFFECT_TEXTURE_TYPE _eEffectTextureType, const _char* _pConstantName);

private:
	virtual HRESULT ReadyMeshes();
	virtual HRESULT ReadyMaterials() { return S_OK; } // 임시
	virtual HRESULT ReadyTextures();

/********************
	Getter / Setter
********************/
public:
	void SetEffectMeshDesc(EFFECTMESH_DESC _EffectMeshDesc) { m_EffectMeshDesc = _EffectMeshDesc; }

/********************
	Data Members
********************/
protected:
	ID3D11ShaderResourceView* m_pDiffuseTexture1;
	ID3D11ShaderResourceView* m_pDiffuseTexture2;
	ID3D11ShaderResourceView* m_pNoiseTexture;
	ID3D11ShaderResourceView* m_pMaskTexture;

	wstring m_wstrDiffuseTextureName1 = L"";
	wstring m_wstrDiffuseTextureName2 = L"";
	wstring m_wstrNoiseTextureName = L"";
	wstring m_wstrMaskTextureName = L"";

protected:
	EFFECTMESH_DESC m_EffectMeshDesc;

protected:
	_float2 m_vLifeTime = {};
	_float2 m_vMaskUVSpeed = {};
};

END