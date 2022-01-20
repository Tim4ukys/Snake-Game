#pragma once

#ifdef SNAKERENDER_EXPORTS
#define SNAKERENDER_API __declspec(dllexport)
#else
#define SNAKERENDER_API __declspec(dllimport)
#endif

enum class DIRECTX_VERSION : char {
	DIRECTX9 = 0,
	DIRECTX10 = 1
};

//template<typename P>
//inline P PointCast(void* p_old)
//{
//	return reinterpret_cast<P>(p_old);
//}

extern "C" SNAKERENDER_API void DirectXDraw();

struct IDirect3DDevice9;
struct ID3D10Device;

union directxDevice 
{
	IDirect3DDevice9* pD3D9Device;
	ID3D10Device* pD3D10Device;

	PVOID UNKOWN;
};

#include <functional>

/**
* @breaf Инициализирует DirectX API
* @param vers Версия DirectX
* @param hWnd Описание окна
* @return Успех операции
*/
extern "C" SNAKERENDER_API HRESULT InitDirectX(DIRECTX_VERSION vers, HWND hWnd);

typedef void(__cdecl CALLBACK_ADDRENDER)(PVOID pDevice);

/**
* @breaf Добавляет адрес функции в массив
* @param fnc Адрес функции
*/
extern "C" SNAKERENDER_API void AddRenderCallBack(std::function < CALLBACK_ADDRENDER > fnc);

typedef void(__cdecl CALLBACK_LOSTRESET)();

extern "C" SNAKERENDER_API void AddLostResetCallBack(std::function < CALLBACK_LOSTRESET> fnc);

typedef void(__cdecl CALLBACK_RESET)(PVOID pDevice);

extern "C" SNAKERENDER_API void AddResetCallBack(std::function < CALLBACK_RESET > fnc);

typedef void(__cdecl CALLBACK_INITDIRECTX)(PVOID pDevice);

extern "C" SNAKERENDER_API void AddInitDirectXCallBack(std::function < CALLBACK_INITDIRECTX > fnc);