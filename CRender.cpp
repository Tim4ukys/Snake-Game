/************************************************
*                                               *
*             Project: Snake game               *
*   Coder: Tim4ukys | My url: vk.com/tim4ukys   *
*                                               *
************************************************/
#include "main.h"

#include "CRender.h"
#include "CGame.h"

inline D3DCOLOR D3DCOLOR_XRGB_ARRAY(const int* a)
{
    return D3DCOLOR_XRGB(a[0], a[1], a[2]);
}

bool CRender::initD3D(HWND hWnd)
{
    pLog->regLastFnc("CRender::initD3D()");
    if (initDirectX) return true;

    if (pD3D = Direct3DCreate9(D3D_SDK_VERSION); !pD3D)
        return false;

    pDevicePTR.Windowed = TRUE;
    pDevicePTR.SwapEffect = D3DSWAPEFFECT_DISCARD;
    pDevicePTR.BackBufferFormat = D3DFMT_UNKNOWN;
    
    if (HRESULT hr = pD3D->CreateDevice(
        D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_NOWINDOWCHANGES,
        &pDevicePTR, &pDevice); FAILED(hr))
    {
        SAFE_RELEASE(pD3D);
        return false;
    }

    // init other lib
    {
        pGame = new CGame(hWnd, pDevice);
        Reset(pDevice);
    }

    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);

    initDirectX = true;
    return true;
}
void CRender::CleanupD3D()
{
    pLog->regLastFnc("CRender::CleanupD3D()");
    SAFE_RELEASE(pDevice);
    SAFE_RELEASE(pD3D);
}

void CRender::Reset(IDirect3DDevice9* pDevice)
{
    pLog->regLastFnc("CRender::postReset()");
    font.Initialize(pDevice);
    render.Initialize(pDevice);

    // Reset
    ImGui_ImplDX9_CreateDeviceObjects();
}
void CRender::lostReset()
{
    pLog->regLastFnc("CRender::lostReset()");
    font.Invalidate();
    render.Invalidate();

    ImGui_ImplDX9_InvalidateDeviceObjects();
}
void CRender::ResetObj()
{
    pLog->regLastFnc("CRender::ResetDevice()");
    lostReset();

    if (auto hr = pDevice->Reset(&pDevicePTR); hr == D3D_OK)
        Reset(pDevice);
}

void CRender::Draw()
{
    pLog->regLastFnc("CRender::Draw()");
    if (pDevice)
    {
        /*
            Очищаем сцену от всего всякого..
        */
        pDevice->Clear(0, NULL,
            D3DCLEAR_TARGET,
            D3DCOLOR_XRGB_ARRAY(CLEAR_COLOR), 0.0f, 0);

        if (SUCCEEDED(pDevice->BeginScene()))
        {
            pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
            pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
            pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
            pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
            //pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
            //pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
            //pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

            pGame->Render(pDevice);
            
            pDevice->EndScene();
        }

        // Выводим содержимое вторичного буфера
        auto result = pDevice->Present(NULL, NULL, NULL, NULL);

        if (result == D3DERR_DEVICELOST && pDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetObj();
    }
}

CRender::CRender()
{
    pLog->regLastFnc("CRender::CRender()");
}
CRender::~CRender()
{
    SAFE_RELEASE(pDevice);
    SAFE_RELEASE(pD3D);
}