#pragma once
#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	// 렌더 그룹
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

	// 프로젝트
	enum PROJECT_TYPE
	{
		PROJECT_CLIENT,
		PROJECT_ENGINE,
		PROJECT_TOOL,
		PROJECT_END
	};

	// 리소스 타입
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

	// 텍스처 타입
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

	// 이펙트 텍스처 타입
	enum EFFECT_TEXTURE_TYPE
	{
		DIFFUSE_TEXTURE1,
		DIFFUSE_TEXTURE2,
		NOISE_TEXTURE,
		MASK_TEXTURE,
		EFFECT_TEXTURE_END
	};

	// 사운드 채널
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

	// 모델 타입
	enum MODEL_TYPE
	{
		MODEL_NONANIM,	// 0
		MODEL_ANIM,		// 1
		MODEL_END
	};

	// 레벨 타입 -> 리소스를 스태틱 레벨에서 가져올 것인지
	enum LEVEL_TYPE
	{
		LEVEL_STATIC,
		LEVEL_DYNAMIC,
		LEVEL_END
	};

	// 콜라이더 타입
	enum COLLIDER_TYPE
	{
		SPHERE,
		AABB,
		OBB,
		COLLIDER_END
	};

	// 몬스터 종류
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

	// 피격 타입
	enum HIT_TYPE 
	{ 
		KNOCKBACKED,
		SUPERARMOR,
		INVINCIBLE,
		HITTYPE_END 
	};

	// 밀치기 타입
	enum KNOCKBACK_TYPE
	{
		WEAK,
		STRONG,
		POWERFUL,
	};

	// 속성
	enum ELEMENTAL_TYPE
	{
		NONE_ELEMENTAL,	// 무속성
		PYRO,			// 불
		HYDRO,			// 물
		ANEMO,			// 바람
		ELECTRO,		// 번개
		DENDRO,			// 풀
		CRYO,			// 얼음
		GEO				// 바위
	};

	// 몬스터 타입
	enum MONSTER_TYPE
	{
		COMMON,
		MIDDLE,
		MIDDLE_BOSS,
		BOSS
	};

	// 파티클 타입
	enum PARTICLE_TYPE
	{
		DROP,
		SPREAD,
		SPREAD_RADIUS,
		SPRITE,
		PARTICLE_END
	};

	// 마우스 입력
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

/************************
	입력 열거체
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