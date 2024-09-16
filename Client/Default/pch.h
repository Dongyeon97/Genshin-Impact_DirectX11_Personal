#ifndef PCH_H
#define PCH_H

// �� Client�� ���ԵǾ� �ִ� pch.h�Դϴ�. ��

// Windows ���̺귯��
#include <Windows.h>
#include <wrl.h>

// Direct3D ���̺귯��
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <Effects11\d3dx11effect.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK\ScreenGrab.h>
#include <DirectXTK\PrimitiveBatch.h>
#include <DirectXTK\VertexTypes.h>
#include <DirectXTK\Effects.h>
#include <DirectXTK\SpriteBatch.h>
#include <DirectXTK\SpriteFont.h>
#include <DirectXTK\SimpleMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
using namespace DirectX;
using namespace DirectX::SimpleMath;

// JSONDPP ���̺귯��
#include <JSONCPP/forwards.h>
#include <JSONCPP/json.h>

#pragma comment(lib, "jsoncpp.lib")

// C++ ���̺귯��
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <algorithm>
#include <random>
#include <numeric>
#include <utility>
#include <functional>
#include <memory>
#include <process.h>
#include <filesystem>
#include <fstream>
#include <io.h>
#include <iostream>

#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <crtdbg.h>

// fmod
#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")

// ����� ���� ���
#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
#include "Engine_Enum.h"

// �޸� ����
//#define _CRTDEBG_MAP_ALLOC
//#ifdef _DEBUG
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif

// #define _CRTDBG_MAP_ALLOC

// Client ���ӽ����̽�
namespace Client
{
	enum LEVEL
	{
		LEVEL_STATIC,
		LEVEL_LOADING,
		LEVEL_LOGO,
		LEVEL_STAGE1,
		LEVEL_STAGE2,
		LEVEL_STAGE3,
		LEVEL_END
	};

	enum LAYER
	{
		LAYER_PLAYER,
		LAYER_MONSTER,
		LAYER_TERRAIN,
		LAYER_OBJECT,
		LAYER_NAVIGATION,
		LAYER_PARTICLE,
		LAYER_EFFECTMESH,
		LAYER_UI,
		LAYER_END
	};

	const unsigned int		g_iWinSizeX = 1280;
	const unsigned int		g_iWinSizeY = 720;
}

extern unsigned int g_iWidth;
extern unsigned int g_iHeight;

extern HWND				g_hWnd;
extern HINSTANCE		g_hInst;

using namespace std;
using namespace Client;

#endif //PCH_H
