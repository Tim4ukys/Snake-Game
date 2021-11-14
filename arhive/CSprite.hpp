#ifndef _CSPRITE_HPP_
#define _CSPRITE_HPP_
#include "DDSTextureLoader9.h"

class CSprite {
private:
    LPD3DXSPRITE pSprite = nullptr; // sprite то с помощю чего мы будем рисовать
    LPDIRECT3DTEXTURE9 pTexture = nullptr; // сама текстура
    D3DXIMAGE_INFO tImageInfo{}; // некоторая информация о картинке
    UINT WW{}, WH{}; // реальные размеры окна
public:

    inline CSprite(IDirect3DDevice9* pD3DDevice, const wchar_t* FullPath, UINT WindowWidth, UINT WindowHeight, bool isDDS) {
        WW = WindowWidth;
        WH = WindowHeight;
        D3DXCreateSprite(pD3DDevice, &pSprite); // создаём pSprite
        if (!isDDS)
        {
            D3DXGetImageInfoFromFileW(FullPath, &tImageInfo); // загружаем инфу о картинке из файла
            D3DXCreateTextureFromFileExW(pD3DDevice, FullPath, tImageInfo.Width, tImageInfo.Height, tImageInfo.MipLevels, NULL, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, 
                D3DX_DEFAULT, D3DCOLOR_XRGB(255, 0, 255), // пиксели с этим цветом (розовый) отображаться не будут
                NULL, NULL, &pTexture); // грузим картинку из файла
        }
        else
        {
            DirectX::CreateDDSTextureFromFile(pD3DDevice, FullPath, &pTexture, false);
        }
    }

    inline ~CSprite(void) {
        SAFE_RELEASE(pSprite);
        SAFE_RELEASE(pTexture);
    }

    inline void Render(float X, float Y, float Width, float Height, D3DCOLOR Color) {
        D3DXMATRIX matXY, matWH, matAll;
        D3DXMatrixTranslation(&matXY, ((float)WW * X) / 100, ((float)WH * Y) / 100, 0.0f);
        D3DXMatrixScaling(&matWH, (((WW * Width) / 100) / tImageInfo.Width), (((WH * Height) / 100) / tImageInfo.Height), 0.0f);
        D3DXMatrixMultiply(&matAll, &matWH, &matXY);
        pSprite->Begin(D3DXSPRITE_ALPHABLEND);
        pSprite->SetTransform(&matAll);
        auto hr = pSprite->Draw(pTexture, NULL, NULL, NULL, Color);
        pSprite->End();
    }
};

#endif