
#pragma once
#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Enum.h"

namespace Engine
{
	// [ GRAPHIC_DESC ]
	struct GRAPHIC_DESC
	{
		HWND			hWnd;
		unsigned int	iWinSizeX, iWinSizeY;
		bool			isWindowed;
	};

	struct MY_MESH;

	// [ EFFECTMESH_DESC ]
	struct ENGINE_DLL EFFECTMESH_DESC
	{
		char szModelFilePath[MAX_PATH]; // 모델 파일의 절대경로
		char szModelFileName[MAX_PATH]; // 모델 파일 이름

		// 메시
		unsigned int iNumMeshes;		// 메시 개수
		std::vector<MY_MESH> vecMeshs;	// 메시

		// 셰이더
		char szShaderFileName[MAX_PATH];	// 셰이더 파일 이름

		// 텍스처
		bool bUseDiffuseTexture = false;	// 디퓨즈 텍스처 사용 여부
		bool bUseNoiseTexture = false;		// 노이즈 텍스처 사용 여부
		bool bUseMaskTexture = true;		// 마스크 텍스처 사용 여부

		char szDiffuseTextureFileName1[MAX_PATH];	// 디퓨즈 텍스처1 파일 이름
		char szDiffuseTextureFileName2[MAX_PATH];	// 디퓨즈 텍스처2 파일 이름
		char szNoiseTextureFileName[MAX_PATH];		// 노이즈 텍스처 파일 이름
		char szMaskTextureFileName[MAX_PATH];		// 마스크 텍스처 파일 이름

		MODEL_TYPE		eModelType;			// 모델 타입
		RENDER_GROUP	eRenderGruop;		// 렌더 그룹
		bool			bUVMoveMode;		// 텍스처의 UV를 움직일 것인가
		bool			bUVMoveReverse;		// UV를 반대로 움직일 것인가
		bool			bUVMoveLoop = true;	// UV움직임을 루프시킬 것인가
		XMFLOAT2		vUVMoveSpeed;		// UV움직임 속도

		bool			bTransformChange;	// Transform Change 여부
		XMFLOAT3		vDestScale;			// 목표 스케일값
		float			fScaleChangeRatio;	// 스케일 변화 가중치
		XMFLOAT3		vRotatePerSecond;	// 초당 회전값

		XMFLOAT4 vDiffuseBaseColor;		// 디퓨즈 Base 컬러
		XMFLOAT4 vDiffuseLerpColor;		// 디퓨즈 Lerp용 컬러

		XMFLOAT3 vScale;
		XMFLOAT3 vRotation;
		XMFLOAT3 vTranslate;

		XMFLOAT2 vLifeTime;
	};

	// [ INSTANCE_DESC ]
	struct INSTANCE_DESC
	{
		char			szDiffuseTextureFileName[MAX_PATH];	// 텍스처 파일 이름
		char			szShaderFileName[MAX_PATH];			// 셰이더 파일 이름

		PARTICLE_TYPE	eParticleType;						// 파티클 타입
		RENDER_GROUP	eRenderGroup;						// 렌더 그룹
		bool			isLoop;								// 루프 여부
		bool			isBillBoard;						// 빌보드 여부

		unsigned int	iNumInstance; // 파티클 개수
		XMFLOAT3		vPivot;		// 피벗 위치
		XMFLOAT3		vCenter;	// 센터 위치
		XMFLOAT3		vRange;		// 생성 범위
		XMFLOAT3		vRotate;	// 회전 정도
		XMFLOAT2		vRadius;	// 최소/최대 반지름(생성 위치)
		XMFLOAT3		vOffset;	// 오프셋 (액터로부터 얼마나 떨어져 있을 것인가)

		XMFLOAT2		vSize;		// 크기
		XMFLOAT2		vSpeed;		// 속도
		XMFLOAT4		vColor;		// 디퓨즈와 섞어줄 색상
		XMFLOAT2		vLifeTime;	// 라이프 타임
		float			fDuration;	// 총 진행 시간
	};

	// [ LIGHT_DESC ]
	struct LIGHT_DESC
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_SPOTLIGHT, TYPE_END };

		TYPE			eLightType; // 광원 타입(방향성, 점, 스포트라이트)
		XMFLOAT4		vLightDir;	// 광원의 방향 벡터
		XMFLOAT4		vLightPos;	// 광원의 위치

		float			fRange;		// 광원이 미치는 범위
		XMFLOAT4		vDiffuse;	// 광원의 Diffuse 벡터
		XMFLOAT4		vAmbient;	// 광원의 Ambient 벡터
		XMFLOAT4		vSpecular;	// 광원의 Specular 벡터
	};

	// [ ATTACK_DESC ]
	struct ATTACK_DESC
	{
		std::wstring wstrOwnerName;		// 공격자 이름
		std::wstring wstrAttackName;	// 공격 이름
		ELEMENTAL_TYPE eElementalType;	// 공격 속성
		KNOCKBACK_TYPE eKnockbackType;	// 넉백 타입(약, 강, 넘어뜨림)
		
		bool	bMultiHit;				// 다단히트 공격입니까?
		float	eDamage;				// 공격 데미지

		float fCriticalRatio = 40.f;	// 크리티컬 확률
	};

// ==정점============================================================================== //

	// [ VTXPOS ]
	struct ENGINE_DLL VTXPOS
	{
		XMFLOAT3 vPosition;

		static const unsigned int	iNumElements = { 1 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[1];
	};

	// [ VTXPOINT ]
	struct ENGINE_DLL VTXPOINT
	{
		XMFLOAT3 vPosition;
		XMFLOAT2 vPSize;

		static const unsigned int	iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[2];
	};

	// [ VTXPOSTEX ]
	struct ENGINE_DLL VTXPOSTEX
	{
		XMFLOAT3 vPosition;
		XMFLOAT2 vTexcoord;

		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[2];
	};

	// [ VTXNORTEX ]
	struct ENGINE_DLL VTXNORTEX
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;

		static const unsigned int	iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[3];
	};

	// [ VTXMESH ]
	struct ENGINE_DLL VTXMESH
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		static const unsigned int	iNumElements = { 4 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[4];
	};

	// [ VTXANIMMESH ]
	struct ENGINE_DLL VTXANIMMESH
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		/* 이 정점은 어떤 뼈들(최대 4개)의 상태를 받아서 변환되어야하는가?! */
		/* iIndex = 각각의 메시가 사용하는 뼈들의 인덱스 */
		XMUINT4			vBlendIndices;

		/* 각각의 뼈의 상태를 몇 퍼센트나 이용할지?!(가중치 0 ~ 1) */
		/* 1 = x + y + z + w */
		XMFLOAT4		vBlendWeights;


		static const unsigned int	iNumElements = { 6 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[6];
	};

	// [ VTXCUBE ]
	struct ENGINE_DLL VTXCUBE
	{
		XMFLOAT3 vPosition;
		XMFLOAT3 vTexcoord;

		static const unsigned int iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC Elements[2];
	};

	// [ VTXINSTANCE ]
	struct ENGINE_DLL VTXINSTANCE
	{
		XMFLOAT4 vRight;
		XMFLOAT4 vUp;
		XMFLOAT4 vLook;
		XMFLOAT4 vTranslation;

		XMFLOAT4 vColor;
	};

	// [ VTX_RECT_INSTANCE ]
	struct ENGINE_DLL VTX_RECT_INSTANCE
	{
		static const unsigned int	iNumElements = { 7 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[7];
	};

	// [ VTX_POINT_INSTANCE ]
	struct ENGINE_DLL VTX_POINT_INSTANCE
	{
		static const unsigned int	iNumElements = { 7 };
		static const D3D11_INPUT_ELEMENT_DESC		Elements[7];
	};

// ==기타============================================================================== //

	struct ENGINE_DLL KEYFRAME
	{
		XMFLOAT3 vScale;
		XMFLOAT4 vRotation;
		XMFLOAT3 vPosition;
		double Time;
	};


// ==애니메이션========================================================================== //

	// [ aiVectorKey ]
	struct ENGINE_DLL MY_VECTORKEY
	{
		double mTime;

		XMFLOAT3 mValue;
	};

	// [ aiQuaternion ]
	struct ENGINE_DLL MY_QUATERNION
	{
		float w;
		float x;
		float y;
		float z;
	};

	// [ aiQuatKey ]
	struct ENGINE_DLL MY_QUATKEY
	{
		double mTime;

		MY_QUATERNION mValue;
	};

	// [ aiNodeAnim ]
	struct ENGINE_DLL MY_NODEANIM
	{
		char szName[MAX_PATH];

		unsigned int iNumScalingKeys;
		unsigned int iNumRotationKeys;
		unsigned int iNumPositionKeys;

		std::vector<MY_VECTORKEY> vecScalingKeys;
		std::vector<MY_QUATKEY> vecRotationKeys;
		std::vector<MY_VECTORKEY> vecPositionKeys;
	};

	// [ aiAnimation ]
	struct ENGINE_DLL MY_ANIMATION
	{
		char szName[MAX_PATH];

		double Duration;
		double TickPerSecond;
		unsigned int iNumChannels;
		std::vector<MY_NODEANIM> vecChannels;
	};

// ==본========================================================================== //

	//[ aiVertexWeight ]
	struct ENGINE_DLL MY_WEIGHT
	{
		unsigned int	iVertexId;	// 뼈의 영향을 받는 이 정점이 전체 정점에서 몇번째인지

		float	fWeight;	// 이 정점이 뼈에게 얼마나 영향을 받는지 가중치
	};

	// [ aiNode ]
	struct ENGINE_DLL MY_AINODE
	{
		char			szBoneName[MAX_PATH];				// aiNode 이름

		XMFLOAT4X4		matTransformation;					// Transformation Matrix
		unsigned int	iNumChildren;						// 자식 뼈의 개수
		std::vector<MY_AINODE> vecChildren;					// 자식 뼈들
	};

	// [ aiBone ]
	struct ENGINE_DLL MY_AIBONE
	{
		char			szBoneName[MAX_PATH];	// aiBone 이름

		XMFLOAT4X4		matOffset;			// Offset Matrix
		unsigned int	iNumWeights;		// 뼈가 영향을 주는 정점의 개수
		std::vector<MY_WEIGHT> vecWeights;	// 뼈의 영향을 받는 정점들
	};

// ==메시=========================================================================== //

	// [ aiFace ]
	struct ENGINE_DLL MY_AIFACE
	{
		unsigned int	iNumIndices;
		
		unsigned int	mIndices[3];
	};

	// [ Mesh ]
	struct ENGINE_DLL MY_MESH
	{
		char szName[MAX_PATH];	// 메시 이름

		unsigned int iMaterialIndex;	// 메시가 사용하는 재질의 인덱스
		unsigned int iNumVertices;		// 정점 개수
		unsigned int iNumFaces;			// 삼각형 개수
		unsigned int iNumBones;			// 메시에 영향을 주는 본의 개수(_pAIMesh->mNumBones)

		std::vector<VTXANIMMESH> vecVertices;	// 메시가 가진 정점들
		std::vector<MY_AIFACE> vecFaces;		// aiFace
		std::vector<MY_AIBONE> vecAIBone;		// 메시에 영향을 주는 본(_pAIMesh->mBones[i])
	};

// ==재질=========================================================================== //

	// [ aiMaterial ]
	struct ENGINE_DLL MY_AIMATERIAL
	{
		// 텍스처 파일의 경로(절대 경로)
		char szTextureFilePath[TEXTURE_TYPE::TRANSMISSION][MAX_PATH];
	};

// ==마이 프리팹========================================================================= //

		// [ 엔진 / 클라이언트 ]
	struct ENGINE_DLL MY_PREFAB
	{
		unsigned int iLayerIndex;	// 레이어 타입

		char szActorName[64];		// 액터 이름
		RENDER_GROUP eRenderGroup;	// 렌더 그룹

		MODEL_TYPE eModelType;				// Anim or NonAnim
		MONSTER_VARIATION eMonsterVariation;// 몬스터 종류
		char szShaderFileName[MAX_PATH];	// 셰이더 파일 이름
		char szModelFileName[MAX_PATH];		// 모델 파일 이름(FBX)

		XMFLOAT3 vScale;
		XMFLOAT3 vRotation;
		XMFLOAT3 vTranslate;
	};

// ==AIScene========================================================================= //

	struct ENGINE_DLL MY_AISCENE
	{
		// 본
		MY_AINODE tRootBone;	// 루트 본

		// 애니메이션
		unsigned int iNumAnimations;	// 애니메이션 개수
		std::vector<MY_ANIMATION> vecAnimations; // 애니메이션

		// 메시
		unsigned int iNumMeshes;		// 메시 개수
		std::vector<MY_MESH> vecMeshs;	// 메시

		// 재질
		unsigned int iNumMaterials;				 // 재질 개수
		std::vector<MY_AIMATERIAL> vecMaterials; // 재질
	};

	// ==모델 데이터======================================================================= //

	// [ Assimp에서 필요한 정보만을 추출해서 만든 나만의 모델 구조체 ]
	struct ENGINE_DLL MODEL_DATA
	{
		char szModelFilePath[MAX_PATH]; // 모델 파일의 절대경로
		char szModelFileName[MAX_PATH]; // 모델 파일 이름

		MODEL_TYPE eModelType;
		MY_AISCENE tAIScenes;
	};

	// [ 툴에서 찍은 모델들의 실제 세부 데이터를 담은 구조체 ]
	struct ENGINE_DLL SPAWN_DATA
	{
		MY_PREFAB tPrefab;
	};
}


#endif // Engine_Struct_h__
