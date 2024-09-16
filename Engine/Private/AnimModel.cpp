#include "Engine_pch.h"
#include "AnimModel.h"

#include "DeviceManager.h"
#include "ResourceManager.h"
#include "PrefabManager.h"

#include "Bone.h"
#include "Mesh.h"
#include "Animation.h"

void CAnimModel::Initialize()
{
	// 프리미티브 컴포넌트를 상속받은 컴포넌트는 초기화시에 반드시 디바이스를 세팅합니다.
	m_pDevice = CDeviceManager::GetInstance()->GetDevice11();
	m_pDeviceContext = CDeviceManager::GetInstance()->GetDeviceContext11();

	// -> 액터로부터 전달 받습니다.
	// m_ModelData = CPrefabManager::GetInstance()->GetModelData(m_wstrModelFileName);

	// 모델 데이터와 스폰데이터로부터 받아온 정보 세팅
	m_eModelType = m_SpawnData.tPrefab.eModelType;

	for (_uint i = 0; i < 64; ++i)
	{
		if (m_SpawnData.tPrefab.szShaderFileName[i] == '\0') { break; }
		m_wstrShaderFileName += m_SpawnData.tPrefab.szShaderFileName[i];
	}

	m_iNumElements = Engine::VTXANIMMESH::iNumElements;
	m_pElements = Engine::VTXANIMMESH::Elements;

	// ==================================================================================
	XMMATRIX matPivot = XMMatrixRotationY(XMConvertToRadians(180.f));
	XMStoreFloat4x4(&m_PivotMatrix, matPivot);

	if (FAILED(ReadyBones(&m_ModelData.tAIScenes.tRootBone, -1)))
	{
		MSG_BOX("Failed to ReadyBones : CAnimModel");
		return;
	}

	if (FAILED(ReadyMeshes()))
	{
		MSG_BOX("Failed to Ready_Meshes : CAnimModel");
		return;
	}

	if (FAILED(ReadyMaterials()))
	{
		MSG_BOX("Failed to Ready_Materials : CAnimModel");
		return;
	}

	if (FAILED(ReadyAnimations(&m_ModelData.tAIScenes)))
	{
		MSG_BOX("Failed to Ready_Materials : CAnimModel");
		return;
	}

	CModelComponent::Initialize();
}

_int CAnimModel::PreUpdate(const _float& _fTimeDelta)
{
	return CModelComponent::PreUpdate(_fTimeDelta);
}

_int CAnimModel::Update(const _float& _fTimeDelta)
{
	return CModelComponent::Update(_fTimeDelta);
}

_int CAnimModel::LateUpdate(const _float& _fTimeDelta)
{
	if (m_bAnimate)
		PlayAnimation(_fTimeDelta, &m_iLastAnimationIndex, &m_iCurrentAnimationIndex);

	return CModelComponent::LateUpdate(_fTimeDelta);
}

void CAnimModel::Render(_uint _iMeshIndex)
{
	CModelComponent::Render(_iMeshIndex);
}

void CAnimModel::EndPlay()
{
	CModelComponent::EndPlay();
}

void CAnimModel::Release()
{
	for (auto& spAnimation : m_vecAnimations)
	{
		spAnimation->Release();
		spAnimation.reset();
	}
	m_vecAnimations.clear();

	CModelComponent::Release();
}

void CAnimModel::PlayAnimation(_float _fTimeDelta, _uint* _iLastIndex, _uint* _iCurIndex)
{
	/* 현재 애니메이션 상태에 맞게 뼈들의 TransformationMatrix행렬을 갱신한다. */
	m_vecAnimations[m_iCurrentAnimationIndex]->Invalidate_TransformationMatrix(_fTimeDelta, m_vecBones, _iLastIndex, _iCurIndex);

	/* 모든 뼈들의 CombinedTransformationMatrix를 구해준다. */
	for (auto& pBone : m_vecBones)
		pBone->InvalidateCombinedTransformationMatrix(m_vecBones);
}

HRESULT CAnimModel::BindBoneMatrices(const _char* _pConstantName, _uint _iMeshIndex)
{
	_float4x4 BoneMatrices[500];

	m_vecMeshes[_iMeshIndex]->SetUpBoneMatrices(BoneMatrices, m_vecBones);

	return BindMatrices(_pConstantName, BoneMatrices, 500);
}

HRESULT CAnimModel::ReadyBones(MY_AINODE* _pNode, _int iParentBoneIndex)
{
	if (0 != m_AdditionalPivot._11)
		XMStoreFloat4x4(&m_PivotMatrix, XMLoadFloat4x4(&m_PivotMatrix) * XMLoadFloat4x4(&m_AdditionalPivot));

	shared_ptr<CBone> spBone = CBone::Create(_pNode, iParentBoneIndex, XMLoadFloat4x4(&m_PivotMatrix));
	if (nullptr == spBone)
		return E_FAIL;

	m_vecBones.push_back(spBone);

	_int iParentIndex = (_uint)(m_vecBones.size()) - 1;

	for (_uint i = 0; i < _pNode->iNumChildren; i++)
	{
		ReadyBones(&_pNode->vecChildren[i], iParentIndex);
	}

	return S_OK;
}

HRESULT CAnimModel::ReadyAnimations(MY_AISCENE* _pAIScene)
{
	m_iNumAnimations = _pAIScene->iNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		shared_ptr<CAnimation> spAnimation = CAnimation::Create(&_pAIScene->vecAnimations[i], dynamic_pointer_cast<CAnimModel>(shared_from_this()));
		if (nullptr == spAnimation)
			return E_FAIL;

		m_vecAnimations.push_back(spAnimation);
	}

	return S_OK;
}

HRESULT CAnimModel::ReadyMeshes()
{
	m_iNumMeshes = m_ModelData.tAIScenes.iNumMeshes;

	for (_uint i = 0; i < m_iNumMeshes; i++)
	{
		shared_ptr<CMesh> spMesh = CMesh::Create(m_eModelType, &m_ModelData.tAIScenes.vecMeshs[i], dynamic_pointer_cast<CModelComponent>(shared_from_this()), XMLoadFloat4x4(&m_PivotMatrix));
		if (nullptr == spMesh)
			return E_FAIL;

		m_vecMeshes.push_back(spMesh);
	}

	return S_OK;
}

HRESULT CAnimModel::ReadyMaterials()
{
	_char			szDrive[MAX_PATH] = "";
	_char			szDirectory[MAX_PATH] = "";

	string strFilePath;

	strFilePath = m_ModelData.szModelFilePath;

	// pModelFilePath = 모델 파일(FBX)의 풀 경로
	// pModelFilePath로부터 [드라이브 경로], [폴더 경로]를 얻어온다.
	_splitpath_s(strFilePath.c_str(), szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

	m_iNumMaterials = m_ModelData.tAIScenes.iNumMaterials;

	m_vecMaterials.resize(m_iNumMaterials);
	m_vecTextureBitFlags.assign(m_iNumMaterials, 0); // 벡터 크기 지정, 0으로 싹 초기화

	// 모델의 재질 개수만큼 순회
	for (_uint i = 0; i < m_iNumMaterials; i++)
	{
		MY_AIMATERIAL tMyAIMaterial = m_ModelData.tAIScenes.vecMaterials[i];
		m_vecMaterials[i].resize(TEXTURE_TYPE::TRANSMISSION); // 최대 텍스처 개수만큼 초기화

		// 재질의 최대 텍스처 개수인 21회만큼 순회
		for (_uint j = 0; j < TEXTURE_TYPE::TRANSMISSION; j++)
		{
			// 문자열이 비어잇다면 통과
			if (0 == strlen(&tMyAIMaterial.szTextureFilePath[j][0]))
				continue;

			// 텍스처 파일 경로에서 [파일명]과 [확장자]만 추출한다.
			_char				szFileName[MAX_PATH] = "";
			_char				szExt[MAX_PATH] = "";

			_splitpath_s(tMyAIMaterial.szTextureFilePath[j], nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			// [드라이브] + [폴더] + [파일명] + [확장자] 를 조합해 텍스처 파일의 총 경로를 만든다.
			_char				szTmp[MAX_PATH] = "";

			strcpy_s(szTmp, szDrive);
			strcat_s(szTmp, szDirectory);
			strcat_s(szTmp, szFileName);
			strcat_s(szTmp, szExt);

			// 멀티바이트를 와이드캐릭터형으로 형변환
			_tchar				szFullPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szTmp, (_uint)strlen(szTmp), szFullPath, MAX_PATH);

			ID3D11ShaderResourceView* pSRV = { nullptr };

			// 완성된 경로로부터 텍스처를 읽어와 생성한다.
			if (FAILED(CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV)))
				return E_FAIL;

			// 텍스처가 존재하는 경우 비트 플래그 on
			m_vecTextureBitFlags[i] |= (_ubyte)j;

			// 컨테이너에 저장한다.
			m_vecMaterials[i][j] = pSRV;
		}
	}

	return S_OK;
}

_int CAnimModel::GetBoneIndex(const _char* _pBoneName) const
{
	// 본이 한개 추가될 때 마다 인덱스를 1씩 증가시켜간다.
	_int iBoneIndex = -1;

	auto iter = find_if(m_vecBones.begin(), m_vecBones.end(), [&](shared_ptr<CBone> _spBone) {

		iBoneIndex++;

		if (false == strcmp(_pBoneName, _spBone->GetBoneName()))
			return true;
		return false;
		});

	if (iter == m_vecBones.end())
		return -1;

	return iBoneIndex;
}

const _float4x4* CAnimModel::GetCombinedBoneMatrixPointer(const _char* _pBoneName)
{
	auto	iter = find_if(m_vecBones.begin(), m_vecBones.end(), [&](shared_ptr<CBone> pBone)
		{
			if (false == strcmp(pBone->GetBoneName(), _pBoneName))
				return true;

			return false;
		});

	return (*iter)->GetCombinedTransformationMatrix();
}

void CAnimModel::SetAnimationIndex(_uint _iAnimIndex)
{
	// [임시]애니메이션을 강제로 변환시키면, 이전과 처음 트랙포지션을 모두 0으로 만든다.
	m_vecAnimations[m_iLastAnimationIndex]->SetTrackPosition(0);
	m_vecAnimations[m_iLastAnimationIndex]->SetIsFinished(false);

	m_vecAnimations[m_iCurrentAnimationIndex]->SetTrackPosition(0);
	m_vecAnimations[m_iCurrentAnimationIndex]->SetIsFinished(false);

	m_iCurrentAnimationIndex = _iAnimIndex;
}

void CAnimModel::SetIsLoop(_bool _bLoop, _uint _iAnimationIndex)
{
	m_vecAnimations[_iAnimationIndex]->SetIsLoop(_bLoop);
}

_bool CAnimModel::GetIsLoop(_uint _iAnimationIndex)
{
	return m_vecAnimations[_iAnimationIndex]->GetIsLoop();
}

void CAnimModel::SetIsRootMotion(_bool _bRootMotionEnable, _uint _iAnimIndex)
{
	m_vecAnimations[_iAnimIndex]->SetIsRootMotion(_bRootMotionEnable);
}

void CAnimModel::SetRootMotionRatio(_float3 _vRatio, _uint _iAnimIndex)
{
	m_vecAnimations[_iAnimIndex]->SetRootMotionRatio(_vRatio);
}

void CAnimModel::SetInterPolationDuration(_float _fDuration, _uint _iAnimIndex)
{
	m_vecAnimations[_iAnimIndex]->SetInterpolationDuration(_fDuration);
}

void CAnimModel::SetInterPolationRatio(_float _fRatio, _uint _iAnimIndex)
{
	m_vecAnimations[_iAnimIndex]->SetInterpolationRatio(_fRatio);
}
