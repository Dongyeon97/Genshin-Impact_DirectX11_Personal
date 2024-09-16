
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
		char szModelFilePath[MAX_PATH]; // �� ������ ������
		char szModelFileName[MAX_PATH]; // �� ���� �̸�

		// �޽�
		unsigned int iNumMeshes;		// �޽� ����
		std::vector<MY_MESH> vecMeshs;	// �޽�

		// ���̴�
		char szShaderFileName[MAX_PATH];	// ���̴� ���� �̸�

		// �ؽ�ó
		bool bUseDiffuseTexture = false;	// ��ǻ�� �ؽ�ó ��� ����
		bool bUseNoiseTexture = false;		// ������ �ؽ�ó ��� ����
		bool bUseMaskTexture = true;		// ����ũ �ؽ�ó ��� ����

		char szDiffuseTextureFileName1[MAX_PATH];	// ��ǻ�� �ؽ�ó1 ���� �̸�
		char szDiffuseTextureFileName2[MAX_PATH];	// ��ǻ�� �ؽ�ó2 ���� �̸�
		char szNoiseTextureFileName[MAX_PATH];		// ������ �ؽ�ó ���� �̸�
		char szMaskTextureFileName[MAX_PATH];		// ����ũ �ؽ�ó ���� �̸�

		MODEL_TYPE		eModelType;			// �� Ÿ��
		RENDER_GROUP	eRenderGruop;		// ���� �׷�
		bool			bUVMoveMode;		// �ؽ�ó�� UV�� ������ ���ΰ�
		bool			bUVMoveReverse;		// UV�� �ݴ�� ������ ���ΰ�
		bool			bUVMoveLoop = true;	// UV�������� ������ų ���ΰ�
		XMFLOAT2		vUVMoveSpeed;		// UV������ �ӵ�

		bool			bTransformChange;	// Transform Change ����
		XMFLOAT3		vDestScale;			// ��ǥ �����ϰ�
		float			fScaleChangeRatio;	// ������ ��ȭ ����ġ
		XMFLOAT3		vRotatePerSecond;	// �ʴ� ȸ����

		XMFLOAT4 vDiffuseBaseColor;		// ��ǻ�� Base �÷�
		XMFLOAT4 vDiffuseLerpColor;		// ��ǻ�� Lerp�� �÷�

		XMFLOAT3 vScale;
		XMFLOAT3 vRotation;
		XMFLOAT3 vTranslate;

		XMFLOAT2 vLifeTime;
	};

	// [ INSTANCE_DESC ]
	struct INSTANCE_DESC
	{
		char			szDiffuseTextureFileName[MAX_PATH];	// �ؽ�ó ���� �̸�
		char			szShaderFileName[MAX_PATH];			// ���̴� ���� �̸�

		PARTICLE_TYPE	eParticleType;						// ��ƼŬ Ÿ��
		RENDER_GROUP	eRenderGroup;						// ���� �׷�
		bool			isLoop;								// ���� ����
		bool			isBillBoard;						// ������ ����

		unsigned int	iNumInstance; // ��ƼŬ ����
		XMFLOAT3		vPivot;		// �ǹ� ��ġ
		XMFLOAT3		vCenter;	// ���� ��ġ
		XMFLOAT3		vRange;		// ���� ����
		XMFLOAT3		vRotate;	// ȸ�� ����
		XMFLOAT2		vRadius;	// �ּ�/�ִ� ������(���� ��ġ)
		XMFLOAT3		vOffset;	// ������ (���ͷκ��� �󸶳� ������ ���� ���ΰ�)

		XMFLOAT2		vSize;		// ũ��
		XMFLOAT2		vSpeed;		// �ӵ�
		XMFLOAT4		vColor;		// ��ǻ��� ������ ����
		XMFLOAT2		vLifeTime;	// ������ Ÿ��
		float			fDuration;	// �� ���� �ð�
	};

	// [ LIGHT_DESC ]
	struct LIGHT_DESC
	{
		enum TYPE { TYPE_DIRECTIONAL, TYPE_POINT, TYPE_SPOTLIGHT, TYPE_END };

		TYPE			eLightType; // ���� Ÿ��(���⼺, ��, ����Ʈ����Ʈ)
		XMFLOAT4		vLightDir;	// ������ ���� ����
		XMFLOAT4		vLightPos;	// ������ ��ġ

		float			fRange;		// ������ ��ġ�� ����
		XMFLOAT4		vDiffuse;	// ������ Diffuse ����
		XMFLOAT4		vAmbient;	// ������ Ambient ����
		XMFLOAT4		vSpecular;	// ������ Specular ����
	};

	// [ ATTACK_DESC ]
	struct ATTACK_DESC
	{
		std::wstring wstrOwnerName;		// ������ �̸�
		std::wstring wstrAttackName;	// ���� �̸�
		ELEMENTAL_TYPE eElementalType;	// ���� �Ӽ�
		KNOCKBACK_TYPE eKnockbackType;	// �˹� Ÿ��(��, ��, �Ѿ�߸�)
		
		bool	bMultiHit;				// �ٴ���Ʈ �����Դϱ�?
		float	eDamage;				// ���� ������

		float fCriticalRatio = 40.f;	// ũ��Ƽ�� Ȯ��
	};

// ==����============================================================================== //

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

		/* �� ������ � ����(�ִ� 4��)�� ���¸� �޾Ƽ� ��ȯ�Ǿ���ϴ°�?! */
		/* iIndex = ������ �޽ð� ����ϴ� ������ �ε��� */
		XMUINT4			vBlendIndices;

		/* ������ ���� ���¸� �� �ۼ�Ʈ�� �̿�����?!(����ġ 0 ~ 1) */
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

// ==��Ÿ============================================================================== //

	struct ENGINE_DLL KEYFRAME
	{
		XMFLOAT3 vScale;
		XMFLOAT4 vRotation;
		XMFLOAT3 vPosition;
		double Time;
	};


// ==�ִϸ��̼�========================================================================== //

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

// ==��========================================================================== //

	//[ aiVertexWeight ]
	struct ENGINE_DLL MY_WEIGHT
	{
		unsigned int	iVertexId;	// ���� ������ �޴� �� ������ ��ü �������� ���°����

		float	fWeight;	// �� ������ ������ �󸶳� ������ �޴��� ����ġ
	};

	// [ aiNode ]
	struct ENGINE_DLL MY_AINODE
	{
		char			szBoneName[MAX_PATH];				// aiNode �̸�

		XMFLOAT4X4		matTransformation;					// Transformation Matrix
		unsigned int	iNumChildren;						// �ڽ� ���� ����
		std::vector<MY_AINODE> vecChildren;					// �ڽ� ����
	};

	// [ aiBone ]
	struct ENGINE_DLL MY_AIBONE
	{
		char			szBoneName[MAX_PATH];	// aiBone �̸�

		XMFLOAT4X4		matOffset;			// Offset Matrix
		unsigned int	iNumWeights;		// ���� ������ �ִ� ������ ����
		std::vector<MY_WEIGHT> vecWeights;	// ���� ������ �޴� ������
	};

// ==�޽�=========================================================================== //

	// [ aiFace ]
	struct ENGINE_DLL MY_AIFACE
	{
		unsigned int	iNumIndices;
		
		unsigned int	mIndices[3];
	};

	// [ Mesh ]
	struct ENGINE_DLL MY_MESH
	{
		char szName[MAX_PATH];	// �޽� �̸�

		unsigned int iMaterialIndex;	// �޽ð� ����ϴ� ������ �ε���
		unsigned int iNumVertices;		// ���� ����
		unsigned int iNumFaces;			// �ﰢ�� ����
		unsigned int iNumBones;			// �޽ÿ� ������ �ִ� ���� ����(_pAIMesh->mNumBones)

		std::vector<VTXANIMMESH> vecVertices;	// �޽ð� ���� ������
		std::vector<MY_AIFACE> vecFaces;		// aiFace
		std::vector<MY_AIBONE> vecAIBone;		// �޽ÿ� ������ �ִ� ��(_pAIMesh->mBones[i])
	};

// ==����=========================================================================== //

	// [ aiMaterial ]
	struct ENGINE_DLL MY_AIMATERIAL
	{
		// �ؽ�ó ������ ���(���� ���)
		char szTextureFilePath[TEXTURE_TYPE::TRANSMISSION][MAX_PATH];
	};

// ==���� ������========================================================================= //

		// [ ���� / Ŭ���̾�Ʈ ]
	struct ENGINE_DLL MY_PREFAB
	{
		unsigned int iLayerIndex;	// ���̾� Ÿ��

		char szActorName[64];		// ���� �̸�
		RENDER_GROUP eRenderGroup;	// ���� �׷�

		MODEL_TYPE eModelType;				// Anim or NonAnim
		MONSTER_VARIATION eMonsterVariation;// ���� ����
		char szShaderFileName[MAX_PATH];	// ���̴� ���� �̸�
		char szModelFileName[MAX_PATH];		// �� ���� �̸�(FBX)

		XMFLOAT3 vScale;
		XMFLOAT3 vRotation;
		XMFLOAT3 vTranslate;
	};

// ==AIScene========================================================================= //

	struct ENGINE_DLL MY_AISCENE
	{
		// ��
		MY_AINODE tRootBone;	// ��Ʈ ��

		// �ִϸ��̼�
		unsigned int iNumAnimations;	// �ִϸ��̼� ����
		std::vector<MY_ANIMATION> vecAnimations; // �ִϸ��̼�

		// �޽�
		unsigned int iNumMeshes;		// �޽� ����
		std::vector<MY_MESH> vecMeshs;	// �޽�

		// ����
		unsigned int iNumMaterials;				 // ���� ����
		std::vector<MY_AIMATERIAL> vecMaterials; // ����
	};

	// ==�� ������======================================================================= //

	// [ Assimp���� �ʿ��� �������� �����ؼ� ���� ������ �� ����ü ]
	struct ENGINE_DLL MODEL_DATA
	{
		char szModelFilePath[MAX_PATH]; // �� ������ ������
		char szModelFileName[MAX_PATH]; // �� ���� �̸�

		MODEL_TYPE eModelType;
		MY_AISCENE tAIScenes;
	};

	// [ ������ ���� �𵨵��� ���� ���� �����͸� ���� ����ü ]
	struct ENGINE_DLL SPAWN_DATA
	{
		MY_PREFAB tPrefab;
	};
}


#endif // Engine_Struct_h__
