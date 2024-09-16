#pragma once
#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	// ���� �׷�
	enum RENDER_GROUP
	{
		RENDER_PRIORITY,
		RENDER_NONBLEND,
		RENDER_NONLIGHT,
		RENDER_BLEND,
		RENDER_ORTHOGONAL,
		RENDER_UI,
		RENDER_SHADOW,
		RENDER_GLOW,
		RENDER_END
	};

	// ������Ʈ
	enum PROJECT_TYPE
	{
		PROJECT_CLIENT,
		PROJECT_ENGINE,
		PROJECT_TOOL,
		PROJECT_END
	};

	// ���ҽ� Ÿ��
	enum RESOURCE_TYPE
	{
		RESOURCE_DIFFUSETEXTURE,
		RESOURCE_NOISETEXTURE,
		RESOURCE_MASKTEXTURE,
		RESOURCE_SOUND,
		RESOURCE_SHADER,
		RESOURCE_NONANIMMODEL,
		RESOURCE_ANIMMODEL,
		RESOURCE_EFFECTMESH,
		RESOURCE_END
	};

	// �ؽ�ó Ÿ��
	enum TEXTURE_TYPE
	{
		NONE,
		DIFFUSE,
		SPECULAR,
		AMBIENT,
		EMISSIVE,
		HEIGHT,
		NORMALS,
		SHININESS,
		OPACITY,
		DISPLACEMENT,
		LIGHTMAP,
		REFLECTION,
		BASE_COLOR,
		NORMAL_CAMERA,
		EMISSION_COLOR,
		METALNESS,
		DIFFUSE_ROUGHNESS,
		AMBIENT_OCCLUSION,
		UNKNOWN,
		SHEEN,
		CLEARCOAT,
		TRANSMISSION,
	};

	// ����Ʈ �ؽ�ó Ÿ��
	enum EFFECT_TEXTURE_TYPE
	{
		DIFFUSE_TEXTURE1,
		DIFFUSE_TEXTURE2,
		NOISE_TEXTURE,
		MASK_TEXTURE,
		EFFECT_TEXTURE_END
	};

	// ���� ä��
	enum SOUND_TYPE
	{
		EFFECT_1,
		EFFECT_2,
		EFFECT_3,
		EFFECT_4,
		EFFECT_5,
		EFFECT_6,
		EFFECT_7,
		EFFECT_8,
		EFFECT_9,
		EFFECT_10,

		VOICE,
		BGM,
		SOUND_END
	};

	// �� Ÿ��
	enum MODEL_TYPE
	{
		MODEL_NONANIM,	// 0
		MODEL_ANIM,		// 1
		MODEL_END
	};

	// ���� Ÿ�� -> ���ҽ��� ����ƽ �������� ������ ������
	enum LEVEL_TYPE
	{
		LEVEL_STATIC,
		LEVEL_DYNAMIC,
		LEVEL_END
	};

	// �ݶ��̴� Ÿ��
	enum COLLIDER_TYPE
	{
		SPHERE,
		AABB,
		OBB,
		COLLIDER_END
	};

	// ���� ����
	enum MONSTER_VARIATION
	{
		NONE_MONSTER,
		SLIME_ICE,
		SLIME_FIRE,
		HILI_ICE,
		HILI_FIRE,
		DEFENDER,
		MONSTER_END
	};

	// �ǰ� Ÿ��
	enum HIT_TYPE 
	{ 
		KNOCKBACKED,
		SUPERARMOR,
		INVINCIBLE,
		HITTYPE_END 
	};

	// ��ġ�� Ÿ��
	enum KNOCKBACK_TYPE
	{
		WEAK,
		STRONG,
		POWERFUL,
	};

	// �Ӽ�
	enum ELEMENTAL_TYPE
	{
		NONE_ELEMENTAL,	// ���Ӽ�
		PYRO,			// ��
		HYDRO,			// ��
		ANEMO,			// �ٶ�
		ELECTRO,		// ����
		DENDRO,			// Ǯ
		CRYO,			// ����
		GEO				// ����
	};

	// ���� Ÿ��
	enum MONSTER_TYPE
	{
		COMMON,
		MIDDLE,
		MIDDLE_BOSS,
		BOSS
	};

	// ��ƼŬ Ÿ��
	enum PARTICLE_TYPE
	{
		DROP,
		SPREAD,
		SPREAD_RADIUS,
		SPRITE,
		PARTICLE_END
	};

	// ���콺 �Է�
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

/************************
	�Է� ����ü
************************/
	enum class EMouseState
	{
		DOWN,
		PRESSED,
		UP,
		RELEASED,
		ENUM_END
	};

	enum class EKeyState
	{
		DOWN,
		PRESSED,
		UP,
		RELEASED,
		ENUM_END
	};

	enum class EMouseButtonType
	{
		LEFT_BUTTON,
		RIGHT_BUTTON,
		SCROLL_WHEEL_BUTTON,
		ENUM_END
	};

	enum class EAxisType
	{
		X_AXIS,
		Y_AXIS,
		Z_AXIS,
		ENUM_END
	};
}
#endif // Engine_Enum_h__