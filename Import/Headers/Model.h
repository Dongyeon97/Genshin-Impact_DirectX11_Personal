#pragma once

BEGIN(Engine)

class CModel final : public enable_shared_from_this<CModel>
{
public:
	explicit CModel() = default;
	CModel(const CModel& rhs); // 복사 생성자
	virtual ~CModel() = default;

/********************
	Framework
********************/
public:
	HRESULT Initialize(MODEL_TYPE _eModelType, const _char* _pModelFilePath);
	void Release();

/********************
	Data Members
********************/
private:
	ID3D11Device* m_pDevice = nullptr;

	ID3D11DeviceContext* m_pDeviceContext = nullptr;

private:
	// const aiScene* m_pAIScene = nullptr;
	// Assimp::Importer m_Importer;
};

END