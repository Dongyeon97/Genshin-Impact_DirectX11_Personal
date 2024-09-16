#pragma once
#ifndef PCH_H
#define PCH_H

// ※ Engine에 포함되어 있는 pch.h입니다. ※

// Windows 라이브러리
#include <Windows.h>
#include <wrl.h>
using namespace Microsoft::WRL;

// Direct3D 라이브러리
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <Effects11/d3dx11effect.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK\ScreenGrab.h>

#include <DirectXTK\PrimitiveBatch.h>
#include <DirectXTK\VertexTypes.h>
#include <DirectXTK\Effects.h>
#include <DirectXTK\SpriteBatch.h>
#include <DirectXTK\SpriteFont.h>
#include <DirectXTK\SimpleMath.h>
using namespace DirectX::SimpleMath;

// #pragma comment (lib, "d3d11.lib")
// #pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;

// JSONDPP 라이브러리
#include <JSONCPP/forwards.h>
#include <JSONCPP/json.h>

#pragma comment(lib, "jsoncpp.lib")

// C++ 라이브러리
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
#include <filesystem>
#include <fstream>
#include <io.h>
#include <iostream>
using namespace std;

// 사용자 정의 헤더
#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"

// fmod
#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmodex_vc.lib")
#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodL_vc.lib")

#endif //PCH_H

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif // _DEBUG