/************************************************
*                                               *
*             Project: Snake game               *
*   Coder: Tim4ukys | My url: vk.com/tim4ukys   *
*                                               *
************************************************/
#include "main.h"

#include "CRender.h"
#include "CGame.h"

CRender* pRender    = nullptr;
CGame* pGame        = nullptr;

CLog* pLog = nullptr; // { "logger.log" }

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR szCmdLine, int nCmdShow)
{
    pLog = new CLog("logger.log");

    pRender = new CRender();

    MSG msg{};
    HWND hwnd{};
    WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hInstance = hInstance;
    wc.lpfnWndProc = [](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
        pGame->WNDProc(hWnd, uMsg, wParam, lParam);

        switch (uMsg)
        {
            case WM_DESTROY:
            {
                PostQuitMessage(EXIT_SUCCESS);
                return 0;
            }
            case WM_ERASEBKGND: 
            {
                return 0;
            }
            case WM_PAINT:
            {
                pRender->Draw();
                return 0;
            }
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    };
    wc.lpszClassName = L"MySnakeGame";
    wc.lpszMenuName = nullptr;
    wc.style = CS_VREDRAW | CS_HREDRAW;

    if (!RegisterClassEx(&wc))
        return EXIT_FAILURE;

    SetUnhandledExceptionFilter([](_EXCEPTION_POINTERS* ExceptionInfo)->LONG {
        pLog->log("------------------------------------------");
        pLog->log(" Game has crashed. =( ");
        pLog->log(" Exception at address: 0x%p, Last function processed: %s", ExceptionInfo->ExceptionRecord->ExceptionAddress, pLog->lastFuncName.c_str());

        return EXCEPTION_CONTINUE_SEARCH;
    });

    hwnd = CreateWindowW(wc.lpszClassName, L"Snake game!", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, 0, 0, 566, 589, nullptr, nullptr, wc.hInstance, nullptr);
    if (hwnd == INVALID_HANDLE_VALUE)
        return EXIT_FAILURE;

    pRender->initD3D(hwnd);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    SAFE_DELETE(pRender); 
    SAFE_DELETE(pGame); 
    SAFE_DELETE(pLog);

    return static_cast<int> (msg.wParam);
}