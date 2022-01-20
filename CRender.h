/************************************************
*                                               *
*             Project: Snake game               *
*   Coder: Tim4ukys | My url: vk.com/tim4ukys   *
*                                               *
************************************************/

/**
* @brief Класс для рендера в DirectX
* @date 05.09.2021
*/
class CRender {
public: 

	CD3DFont font{ "Segoe UI", 16, FCR_BORDER };
	CD3DRender render{ 128 };

	CRender();
	~CRender();

	/**
	* @brief Инициализация Direct3D
	* @param hWnd Обработчик окна
	*/
	bool initD3D(HWND hWnd);

	/**
	* @brief Валидация Direct3D
	*/
	void CleanupD3D();
	
	/**
	* @brief После-рендеровая очистка буфера рендера
	*/
	void ResetObj();

	/**
	* @brief Параметры DirectX Device
	*/
	D3DPRESENT_PARAMETERS pDevicePTR{};

	/**
	* @brief Сам рендер сцены
	*/
	void Draw();

	/**
	* @brief Указатель на \b Device
	* @return Device DirectX
	*/
	//inline IDirect3DDevice9* getDevice()
	//{
		//return pDevice;
	//}

private:

	const int CLEAR_COLOR[3]{ 0x0, 0x0, 0x0 };

	//IDirect3DDevice9* pDevice = NULL;
	//IDirect3D9* pD3D = nullptr;
	//IDirect3DSurface9* m_pSurface = nullptr;

	bool initDirectX{};

	/**
	* @brief Инициализация объектов
	* @param pDevice Device DirectX
	*/
	void Reset(IDirect3DDevice9* pDevice);

	/**
	* @brief Валидация объектов
	*/
	void lostReset();
};

