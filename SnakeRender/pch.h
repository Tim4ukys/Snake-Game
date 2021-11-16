#ifndef PCH_H
#define PCH_H

#pragma warning(disable : 4005)

// stl & old c library
#include <string>
#include <vector>

// win_api
#include <Windows.h>

// DirectX
// 9.0
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
// 10.0
#include <d3d10.h>
#include <D3DX10.h>
#pragma comment(lib, "d3d10.lib")
#pragma comment(lib, "d3dx10.lib")

#define SAFE_RELEASE(p) if (p) { p->Release(); p=nullptr; }

#endif //PCH_H
