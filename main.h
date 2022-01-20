/************************************************
*                                               *
*             Project: Snake game               *
*   Coder: Tim4ukys | My url: vk.com/tim4ukys   *
*                                               *
************************************************/
#ifndef _MAIN_H_
#define _MAIN_H_

#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#pragma warning(disable : 4005)
#pragma warning(disable : 4838)

// STL
#include <string>
#include <vector>
#include <array>
#include <thread>
#include <functional>
#include <filesystem>
#include <random>
#include <sstream>
#include <fstream>
#include <map>
#include <codecvt>
// Old STL
#include <cstdint>
// WIN-API
#include <Windows.h>
#include <xaudio2.h>
#include <strsafe.h>
#include <shellapi.h>
#include <mmsystem.h>
#include <conio.h>
// DirectX Library
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "DXUT.lib")
#pragma comment(lib, "DXUTOpt.lib")

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "comctl32.lib")
// Other Library
#include "d3drender.h"
#include "nlohmann/json.hpp"
//#include "CSprite.hpp"
#include "ocornut/imgui/imgui.h"
#include "ocornut/imgui/imgui_impl_dx9.h"
#include "ocornut/imgui/imgui_impl_win32.h"
//#include "un4seen/bass dll/bass.h"

using json = nlohmann::json;

#include "CLog.h"
//#include "CLanguage.h"
//#include "CRender.h"
//#include "CGame.h"

extern CLog* pLog;
extern class CRender* pRender;
extern class CGame* pGame;

#define SAFE_DELETE(p) if (p){ delete p; p=nullptr; }
#define SAFE_DELETE_ARRAY(arr) if (arr){ delete[] arr; arr=nullptr; }

/**
* @brief Возращает случайное значение
* @param min Минимальное значение
* @param max Максимальное значение
* @return Случайное значение
*/
template <typename T>
inline T random(T min, T max)
{
	pLog->regLastFnc("random()");
	std::random_device rd;
	std::default_random_engine e1(rd());
	std::uniform_int_distribution<T> uniform_dist(min, max);
	return uniform_dist(e1);
}

/**
* @brief Возращает wstring
* @param str string
* @return wstring
*/
inline std::wstring wstring_from_string(std::string str)
{
	pLog->regLastFnc("wstring_from_string()");
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

/**
* @brief Возращает string
* @param wstr wstring
* @return string
*/
inline std::string string_from_wstring(std::wstring wstr)
{
	pLog->regLastFnc("string_from_wstring()");
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

/**
* @brief Возращает путь до папки где лежит исполняемый файл
* @return Полный путь до исполняемого файла
*/
inline std::wstring getCurrentPath() {
	pLog->regLastFnc("getCurrentPath()");
	static std::wstring rel{ L"" };
	if (rel == L"") {
		wchar_t tempBuff[1024]{};
		GetModuleFileNameW(NULL, tempBuff, sizeof(tempBuff) / sizeof(wchar_t));

		for (size_t i = wcslen(tempBuff); i > 0; i--)
		{
			if (tempBuff[i] == L'\\') {
				tempBuff[i] = L'\0';
				break;
			}
			else tempBuff[i] = L'\0';
		}
		rel = std::wstring(tempBuff, wcslen(tempBuff));
	}
	return rel;
}

#endif