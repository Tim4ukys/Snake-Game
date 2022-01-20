#include "pch.h"
#include "SnakeRender.h"

IDXGISwapChain* g_pSwapChain = nullptr;
ID3D10RenderTargetView* g_pRenderTargetView = nullptr;

D3DPRESENT_PARAMETERS* g_pDevicePTR;
LPDIRECT3D9 g_Direct3D9 = nullptr;

directxDevice g_pDevice{};

DIRECTX_VERSION g_Version{};

////////////////////////////
/////      EVENTS      /////
////////////////////////////

std::vector<std::function < CALLBACK_ADDRENDER > > g_fncCallBackRender{};
void AddRenderCallBack(std::function < CALLBACK_ADDRENDER > fnc)
{
	g_fncCallBackRender.push_back(fnc);
}

std::vector<std::function < CALLBACK_LOSTRESET>> g_fncCallBackLostReset{};
void AddLostResetCallBack(std::function < CALLBACK_LOSTRESET> fnc)
{
	g_fncCallBackLostReset.push_back(fnc);
}

std::vector<std::function < CALLBACK_RESET >> g_fncCallBackReset{};
void AddResetCallBack(std::function < CALLBACK_RESET > fnc)
{
	g_fncCallBackReset.push_back(fnc);
}

std::vector<std::function < CALLBACK_INITDIRECTX >> g_fncCallBackInitDirectX{};
void AddInitDirectXCallBack(std::function < CALLBACK_INITDIRECTX > fnc)
{
	g_fncCallBackInitDirectX.push_back(fnc);
}

///////////////////////////////
/////      FUNCTIONS      /////
///////////////////////////////

void lostResetDevice()
{
	for (auto& clfnc : g_fncCallBackLostReset)
	{
		clfnc();
	}
}

void resetDevice()
{
	for (auto& clfnc : g_fncCallBackReset)
	{
		clfnc(g_pDevice.UNKOWN);
	}
}

void ResetObjects()
{
	lostResetDevice();

	auto pDevice = g_pDevice.pD3D9Device;
	if (auto hr = pDevice->Reset(g_pDevicePTR); hr == S_OK)
	{
		resetDevice();
	}
}

void DirectXDraw()
{
	if (g_pDevice.UNKOWN)
	{
		switch (g_Version)
		{
		case DIRECTX_VERSION::DIRECTX9:
		{
			auto pDevice = g_pDevice.pD3D9Device;

			pDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0/*black*/, 0.0f, 0);

			if (SUCCEEDED(pDevice->BeginScene()))
			{
				pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
				pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
				pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

				for (auto& clfnc : g_fncCallBackRender)
				{
					clfnc(g_pDevice.pD3D9Device);
				}

				pDevice->EndScene();
			}

			// Выводим содержимое вторичного буфера
			auto result = pDevice->Present(NULL, NULL, NULL, NULL);

			if (result == D3DERR_DEVICELOST && pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
				ResetObjects();
		}
		break;
		case DIRECTX_VERSION::DIRECTX10:
		{
			auto pDevice = g_pDevice.pD3D10Device;

			const FLOAT ClearColor[4]{ 0, 0, 0, 1.0f };
			pDevice->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

			auto result = g_pSwapChain->Present(0, 0);
			if (result == D3DERR_DEVICELOST)
				ResetObjects();
		}
		break;
		}
	}
}

void CleanupRenderTarget()
{
	if (g_pRenderTargetView) { g_pRenderTargetView->Release(); g_pRenderTargetView = nullptr; }
}

void CreateRenderTarget()
{
	ID3D10Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pDevice.pD3D10Device->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
}

WNDPROC g_pWindowProc = nullptr;
LRESULT __stdcall WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
		if (g_pDevice.UNKOWN != nullptr && wParam != SIZE_MINIMIZED)
		{
			lostResetDevice();
			CleanupRenderTarget();

			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_R8G8B8A8_UNORM, 0);

			CreateRenderTarget();
			resetDevice();
		}
	}
	return ::CallWindowProcA(g_pWindowProc, hWnd, msg, wParam, lParam);
}

HRESULT InitDirectX(DIRECTX_VERSION vers, HWND hWnd)
{
	HRESULT hr = S_OK;

	RECT rect;
	GetClientRect(hWnd, &rect);

	switch (vers)
	{
	case DIRECTX_VERSION::DIRECTX9:
		{
			g_Direct3D9 = Direct3DCreate9(D3D_SDK_VERSION);
			if (!g_Direct3D9)
				return S_FALSE;

			g_pDevicePTR = reinterpret_cast<D3DPRESENT_PARAMETERS*>(malloc(sizeof(D3DPRESENT_PARAMETERS)));
			ZeroMemory(g_pDevicePTR, sizeof(D3DPRESENT_PARAMETERS));
			g_pDevicePTR->Windowed = TRUE;
			g_pDevicePTR->SwapEffect = D3DSWAPEFFECT_DISCARD;
			g_pDevicePTR->BackBufferFormat = D3DFMT_UNKNOWN;

			hr = g_Direct3D9->CreateDevice(
				D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_NOWINDOWCHANGES,
				g_pDevicePTR, &g_pDevice.pD3D9Device);
			if (FAILED(hr))
			{
				SAFE_RELEASE(g_Direct3D9);
				return false;
			}
			resetDevice();
		}
		break;
	case DIRECTX_VERSION::DIRECTX10:
		{
			g_pWindowProc = (WNDPROC)SetWindowLongW(hWnd, GWL_WNDPROC, (LONG)WndProcHandler);

			DXGI_SWAP_CHAIN_DESC* sd;

			sd = reinterpret_cast<DXGI_SWAP_CHAIN_DESC*>(malloc(sizeof(DXGI_SWAP_CHAIN_DESC)));

			const UINT width = rect.right - rect.left;
			const UINT height = rect.bottom - rect.top;

			sd->BufferCount = 1;
			sd->BufferDesc.Width = width;
			sd->BufferDesc.Height = height;
			sd->BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sd->BufferDesc.RefreshRate.Numerator = 60;
			sd->BufferDesc.RefreshRate.Denominator = 1;
			sd->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sd->OutputWindow = hWnd;
			sd->SampleDesc.Count = 1;
			sd->SampleDesc.Quality = 0;
			sd->Windowed = TRUE;

			auto pDevice = &g_pDevice.pD3D10Device;

			hr = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, sd, &g_pSwapChain, pDevice);

			ID3D10Texture2D* pBackBuffer;
			hr = g_pSwapChain->GetBuffer(NULL, __uuidof(ID3D10Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));
			if (FAILED(hr))
				return hr;

			hr = (*pDevice)->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
			pBackBuffer->Release();
			if (FAILED(hr))
				return hr;

			(*pDevice)->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);

			D3D10_VIEWPORT vp;
			vp.Width = width;
			vp.Height = height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			(*pDevice)->RSSetViewports(1, &vp);
		}
		break;
	default:
		return S_FALSE;
		break;
	}

	for (auto& clfnc : g_fncCallBackInitDirectX)
	{
		clfnc(g_pDevice.UNKOWN);
	}

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	return S_OK;
}