#pragma once

// ※ Tool에 포함되어 있는 pch.h입니다. ※

#include "../../Client/Default/pch.h"

//// Windows 라이브러리
//#include <Windows.h>
//#include <wrl.h>
//using namespace Microsoft::WRL;
//
// imgui 라이브러리
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

// assimp 라이브러리
#include <assimp\postprocess.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
//
//// Direct3D 라이브러리
//#include <d3d11.h>
//#include <DirectXMath.h>
//#include <d3dcompiler.h>
//#include <DirectXCollision.h>
//#define DIRECTINPUT_VERSION 0x0800
//#include <dinput.h>
//
//#include <Effects11/d3dx11effect.h>
//#include <DirectXTK/DDSTextureLoader.h>
//#include <DirectXTK/WICTextureLoader.h>
//
//#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dcompiler.lib")
//
//using namespace DirectX;
//
//// C++ 라이브러리
//#include <string>
//#include <array>
//#include <vector>
//#include <list>
//#include <stack>
//#include <queue>
//#include <set>
//#include <map>
//#include <unordered_set>
//#include <unordered_map>
//
//#include <algorithm>
//#include <numeric>
//#include <utility>
//#include <functional>
//#include <memory>
//#include <filesystem>
//#include <fstream>
//#include <io.h>
//#include <iostream>
//using namespace std;
//
//// 사용자 정의 헤더
//#include "Engine_Enum.h"
//#include "Engine_Function.h"
//#include "Engine_Macro.h"
//#include "Engine_Struct.h"
//#include "Engine_Typedef.h"
//
//#ifdef _DEBUG
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#ifndef DBG_NEW 
//
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//
//#endif
//
//#endif // _DEBUG

namespace Tool
{
	static _int g_iWinSizeX = 1280;
	static _int g_iWinSizeY = 720;

	extern unsigned int g_iWidth;
	extern unsigned int g_iHeight;
}

extern HWND				g_hWnd;
extern HINSTANCE		g_hInst;