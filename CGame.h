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
* @brief ����� ����
* @date 13.09.2021
* 
* ��� ����� ���� ��� ���������� ���� ����, ���� � ����!
*/
class CGame {
public: 

	/**
	* @brief ����������� ������
	* @param hwnd ���������� ����
	*/
	CGame(HWND hwnd, LPDIRECT3DDEVICE9 pDevice);
	~CGame();

	/**
	* @brief ���������� ����
	*/
	void WNDProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	/**
	* @brief ������� ��� ��������� ���� ��������
	* @param pDevice ��������� �� DirectXDevice
	*/
	void Render(IDirect3DDevice9* pDevice);

private:

	/**
	* @brief ��������� "Config.json"
	*/
	void loadConfig();

	/**
	* @brief ��������� ��������� � ��������� � "Config.json"
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
	* @brief ������� ������
	* @param text �����
	* @param pos 2D ������� �� ������
	* @param isBack ����������� ��� ������� ������ "�����". ���� false - MUSIC_MENU_BACK, true - MUSIC_MENU_ENTER
	* @return ��������� ������ ������� �� ������
	*/
	bool menuButton(std::string text, ImVec2 pos, bool isBack = false);

	/**
	* @brief ������ � ����
	* @param text �����
	* @param totalQuant ���������� ������ � ���
	* @param currentQuant ������� ����� �� �����
	* @param pos 2D ������� �� ������
	* @param isBack ����������� ��� ������� ������ "�����". ���� false - MUSIC_MENU_BACK, true - MUSIC_MENU_ENTER
	* @return ��������� ������ ������� �� ������
	*/
	bool menuButton(std::string text, size_t totalQuant, size_t currentQuant, ImVec2 pos = { 0, 0 }, bool isBack = false);

	/**
	* @brief ������� � ����
	* @param text �����
	* @param check_state ��������� �� ������ bool. � �� ����� �������� ��������� ��������.
	* @param totalQuant ���������� ������ � ���
	* @param currentQuant ������� ����� �� �����
	* @param pos 2D ������� �� ������
	* @return ��������� ������ ������� �� ���� �������
	*/
	bool menuCheckbox(std::string text, bool* check_state, size_t totalQuant, size_t currentQuant, ImVec2 pos = { 0, 0 });

	///////////////////////

	struct stMultiLanguage 
	{
		CLanguage* pLanguage = nullptr;
		std::wstring key{};

	} multi_lang;

	/**
	* @brief ������� � ���������� � ������� ����� ����� ������� ���� 
	*/
	void changeLanguage();

	///////////////////////

	const int maxBlock = 35;

	/**
	* @brief ������� ��� ��������� ��������
	* @param id_x ID ������ �� X
	* @param id_y ID ������ �� Y
	* @param size ������ ����
	* @param color ���� ����
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
	* @brief ������� ��� ��������� ��������
	* @param id_x ID ������ �� X
	* @param id_y ID ������ �� Y
	* @param pos ������, ��� [0] - X, [1] - Y
	*/
	void getBlockPos(uint8_t id_x, uint8_t id_y, float* pos);

	/**
	* @brief ��������� ������ ������
	* @return ������ ������
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
		* @brief ���������, ������ �� �����
		* @return true - ����� ������, false - ��� ���
		*/
		bool isReady();
	} tick;

	//////////////////////////

	struct stMusic 
	{
		enum MUSIC {
			//MUSIC_MOVE,		// ��������
			MUSIC_DEATH,	// ������
			MUSIC_NYAM,		// ���!
			MUSIC_SOUND,	// �����

			MUSIC_MENU_CHANGE,	// ������ � ����
			MUSIC_MENU_BACK,	// �������� ����� � ����
			MUSIC_MENU_ENTER,	// ����� � ����

			MUSIC_WIN	// ������!
		};

		/**
		* @brief ����������� ������
		* @param music_id ID �����
		* @param restart ����������� ��, ���� ��� ��� ������
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
		const D3DCOLOR apple = D3DCOLOR_XRGB(176, 0, 0); // ������
		const D3DCOLOR borders = D3DCOLOR_XRGB(117, 117, 117);	// �������
		
		const D3DCOLOR snakeHead = D3DCOLOR_XRGB(0, 143, 102);	// ������ ����
		const D3DCOLOR snakeBody = D3DCOLOR_XRGB(10, 199, 0);	// ���� ����
		const D3DCOLOR snakeDead = D3DCOLOR_XRGB(255, 0, 0); // ��������� �����

		const ImVec4 GameOver{ 0.66f, 0.66f, 0.66f, 1.0f };	// ������� "����� ����"

		const ImVec4 buttonActive{ 0.7f, 0.7f, 0.7f, 1.0f }; // �������� ������
		const ImVec4 buttonDisable{ 0.5f, 0.5f, 0.5f, 1.0f }; // �� �������� ������
	} color;

	///////////////////////

	struct stApple {
		int x{}, y{};
	} apple;

	/**
	* @brief ��������� ������� ������
	*/
	void updateApplePos();

};