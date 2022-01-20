/************************************************
*                                               *
*             Project: Snake game               *
*   Coder: Tim4ukys | My url: vk.com/tim4ukys   *
*                                               *
************************************************/

class blocksEx {
public:
	blocksEx(uint8_t xnew, uint8_t ynew);

	uint8_t x{}, y{};
};

class CSnake {

	const uint8_t START_COORD[2]{ 5, 15 };
	bool isAdd{};

public:

	CSnake(uint8_t len);


	std::vector < blocksEx > vSnake{};


	void add();
	void move(blocksEx new_blocks);

};

class CLanguage;

/**
* @brief Класс игры
* @date 13.09.2021
* 
* Это класс игры где происходит весь треш, мясо и угар!
*/
class CGame {
public: 

	/**
	* @brief Конструктор класса
	* @param hwnd Обработчик окна
	*/
	CGame(HWND hwnd, LPDIRECT3DDEVICE9 pDevice);
	~CGame();

	/**
	* @brief Обработчик окна
	*/
	void WNDProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	* @brief Функция для отрисовки всех объектов
	* @param pDevice Указатель на DirectXDevice
	*/
	void Render(IDirect3DDevice9* pDevice);

private:

	/**
	* @brief Загружает "Config.json"
	*/
	void loadConfig();

	/**
	* @brief Выгружает настройки и сохраняет в "Config.json"
	*/
	void saveConfig();

	enum class GAME_STATE : int {
		GAME_STATE_PAUSE,
		GAME_STATE_PLAY,
		GAME_STATE_PAUSE_NOSTARTGAME
	} game_state = GAME_STATE::GAME_STATE_PAUSE_NOSTARTGAME;

	///////////////////////

	struct stWin 
	{
		bool isWin{};

		float alpha{}, alpha_background{};
		float y{ 150 };
	} win;

	///////////////////////

	enum class MENU_ID : int {
		MENU_ID_MAIN,
		MENU_ID_SETTING,
		MENU_ID_SETTING_CHANGELANGUAFE
	} menu_id = MENU_ID::MENU_ID_MAIN;


	/**
	* @brief Обычная кнопка
	* @param text Текст
	* @param pos 2D Позиция на экране
	* @param isBack Проигрывать при нажатии музыку "Назад". Если false - MUSIC_MENU_BACK, true - MUSIC_MENU_ENTER
	* @return Возращает статус нажатия на кнопку
	*/
	bool menuButton(std::string text, ImVec2 pos, bool isBack = false);

	/**
	* @brief Кнопка в ряду
	* @param text Текст
	* @param totalQuant Количество кнопок в ряд
	* @param currentQuant Текущий номер по счёту
	* @param pos 2D Позиция на экране
	* @param isBack Проигрывать при нажатии музыку "Назад". Если false - MUSIC_MENU_BACK, true - MUSIC_MENU_ENTER
	* @return Возращает статус нажатия на кнопку
	*/
	bool menuButton(std::string text, size_t totalQuant, size_t currentQuant, ImVec2 pos = { 0, 0 }, bool isBack = false);

	/**
	* @brief ЧекБокс в ряду
	* @param text Текст
	* @param check_state Указатель на объект bool. В нём будет хранится состояние ЧекБокса.
	* @param totalQuant Количество кнопок в ряд
	* @param currentQuant Текущий номер по счёту
	* @param pos 2D Позиция на экране
	* @return Возращает статус нажатия на этот элемент
	*/
	bool menuCheckbox(std::string text, bool* check_state, size_t totalQuant, size_t currentQuant, ImVec2 pos = { 0, 0 });

	///////////////////////

	struct stMultiLanguage 
	{
		CLanguage* pLanguage = nullptr;
		std::wstring key{};

	} multi_lang;

	/**
	* @brief Подменю в настройках в котором нужно будет выбрать язык 
	*/
	void changeLanguage();

	///////////////////////

	const int maxBlock = 35;

	/**
	* @brief Функция для отрисовки квадрата
	* @param id_x ID кубика по X
	* @param id_y ID кубика по Y
	* @param size Размер куба
	* @param color Цвет куба
	*/
	void renderBox(int id_x, int id_y, float size, D3DCOLOR color);

	///////////////////////

	enum class DIRECT : int {
		DIRECT_UP,
		DIRECT_DOWN,
		DIRECT_LEFT,
		DIRECT_RIGHT
	} directCurrent = DIRECT::DIRECT_RIGHT;
	std::vector<DIRECT> vDirect{};

	/**
	* @brief Функция для отрисовки квадрата
	* @param id_x ID кубика по X
	* @param id_y ID кубика по Y
	* @param pos Массив, где [0] - X, [1] - Y
	*/
	void getBlockPos(uint8_t id_x, uint8_t id_y, float* pos);

	/**
	* @brief Возращает размер кубика
	* @return размер кубика
	*/
	float getSizeBlock();

	CSnake snake{ 3 };

	//////////////////////////

	typedef uint64_t TICK;
	struct stTick
	{
		int DELAY{ 1000 };

		TICK m_startTime{};

		/**
		* @brief Проверяет, прошло ли время
		* @return true - время прошло, false - ещё нет
		*/
		bool isReady();
	} tick;

	//////////////////////////

	struct stMusic 
	{
		enum MUSIC {
			//MUSIC_MOVE,		// Движение
			MUSIC_DEATH,	// Смерть
			MUSIC_NYAM,		// Ням!
			MUSIC_SOUND,	// Песня

			MUSIC_MENU_CHANGE,	// Выбрал в меню
			MUSIC_MENU_BACK,	// Вернулся назад в меню
			MUSIC_MENU_ENTER,	// Нажал в меню

			MUSIC_WIN	// Победа!
		};

		/**
		* @brief Проигрывает музыку
		* @param music_id ID звука
		* @param restart Проигрывать ли, если она уже играет
		*/
		void play(MUSIC music_id, BOOL restart = FALSE);

		IXAudio2* pXAudio2 = nullptr;
		IXAudio2MasteringVoice* pMasteringVoice = nullptr;

		bool state{};

		struct musData {
			class CWaveFile* m_pWAV;
			LPBYTE m_pbWaveData;
			DWORD m_cbWaveSize;
		};
		std::array<std::pair<musData, IXAudio2SourceVoice*>/*HSTREAM*/, 7U> arrMusic{};
	} music;

	struct stGameOver {
		
		bool isActive{};

		ImFont* pFont = nullptr;

		//const ImVec2 pos{ 144, 243 };

	} game_over;

	////////////////////////

	struct stColor {
		const D3DCOLOR apple = D3DCOLOR_XRGB(176, 0, 0); // Яблоко
		const D3DCOLOR borders = D3DCOLOR_XRGB(117, 117, 117);	// Граница
		
		const D3DCOLOR snakeHead = D3DCOLOR_XRGB(0, 143, 102);	// Голова змеи
		const D3DCOLOR snakeBody = D3DCOLOR_XRGB(10, 199, 0);	// Тело змеи
		const D3DCOLOR snakeDead = D3DCOLOR_XRGB(255, 0, 0); // Стукнутая часть

		const ImVec4 GameOver{ 0.66f, 0.66f, 0.66f, 1.0f };	// Надпись "Конец игры"

		const ImVec4 buttonActive{ 0.7f, 0.7f, 0.7f, 1.0f }; // Активная кнопка
		const ImVec4 buttonDisable{ 0.5f, 0.5f, 0.5f, 1.0f }; // Не активная кнопка
	} color;

	///////////////////////

	struct stApple {
		int x{}, y{};
	} apple;

	/**
	* @brief Обновляет позицию Яблока
	*/
	void updateApplePos();

};