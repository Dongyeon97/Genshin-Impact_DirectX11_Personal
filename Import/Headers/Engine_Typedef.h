#pragma once
#ifndef Engine_Typedef_h__
#define Engine_Typedef_h__

namespace Engine
{
	typedef		bool						_bool;

	typedef		signed char					_byte;
	typedef		unsigned char				_ubyte;
	typedef		char						_char;

	typedef		wchar_t						_tchar;
	
	typedef		signed short				_short;
	typedef		unsigned short				_ushort;	

	typedef		signed int					_int;
	typedef		unsigned int				_uint;

	typedef		signed long					_long;
	typedef		unsigned long				_ulong;
	typedef		unsigned long long			_ullong;

	typedef		float						_float;
	typedef		double						_double;
	
	/* �����͸� �����ϱ����� ���� .*/
	typedef		XMFLOAT2					_float2;
	typedef		XMFLOAT3					_float3;
	typedef		XMFLOAT4					_float4;

	/* ������ �ϱ����� ����. */
	typedef		XMVECTOR					_vector;
	typedef		FXMVECTOR					_fvector;
	typedef		GXMVECTOR					_gvector;
	typedef		HXMVECTOR					_hvector;
	typedef		CXMVECTOR					_cvector;

	/* �����͸� �����ϱ����� ���.*/
	typedef		XMFLOAT4X4					_float4x4;

	/* ������ �ϱ����� ���. */
	typedef		XMMATRIX					_matrix;
	typedef		FXMMATRIX					_fmatrix;
	typedef		CXMMATRIX					_cmatrix;

	/* ��Ʈ �÷��� */
	const unsigned char option0 = 1 << 0; // 0000 0001 
	const unsigned char option1 = 1 << 1; // 0000 0010
	const unsigned char option2 = 1 << 2; // 0000 0100
	const unsigned char option3 = 1 << 3; // 0000 1000
	const unsigned char option4 = 1 << 4; // 0001 0000
	const unsigned char option5 = 1 << 5; // 0010 0000
	const unsigned char option6 = 1 << 6; // 0100 0000
	const unsigned char option7 = 1 << 7; // 1000 0000

/********************
	������ ������
********************/
	static _int g_iWinSizeX = 1280;
	static _int g_iWinSizeY = 720;
}

using namespace Engine;

#endif // Engine_Typedef_h__
