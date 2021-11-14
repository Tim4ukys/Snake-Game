/************************************************
*                                               *
*             Project: Snake game               *
*   Coder: Tim4ukys | My url: vk.com/tim4ukys   *
*                                               *
************************************************/

/**
* @brief ����� ��� �������� �������� ����
* @date 12.09.2021
*/
class CLanguage {
public:

	/*enum class CODE_TRANSLETE : int {
		CODE_PLAY,
		CODE_EXIT,
		CODE_CONTINUE,
		CODE_SETTING
	};*/

	/*struct stWordsW {
		std::wstring play{}, exit{}, continue_game{},
			back{}, gameover{};

		struct stSettings {
			std::wstring name{}, change_language{},
				sound{}, off{}, on{};
		} settings;
	};*/
	template<typename T>
	struct stWords {
		T play{}, exit{}, continue_game{},
			back{}, gameover{}, win{};

		struct stSettings {
			T name{}, change_language{},
				sound{}, off{}, on{};
		} settings;
	};

	std::vector<std::wstring> vKey{};

	CLanguage();
	~CLanguage();

	/**
	* @brief ��������� ������ ������ stWords c ������������ �������
	* @details ������ � ������ ����� ����� std::wstring
	* @param vKey ������
	* @return ������ ������ stWords c ������������ �������
	*/
	stWords<std::wstring>	getTranslateWordsW(std::wstring vKey);

	/**
	* @brief ��������� ������ ������ stWords c ������������ �������
	* @details ������ � ������ ����� ����� std::string
	* @param vKey ������
	* @return ������ ������ stWords c ������������ �������
	*/
	stWords<std::string>	getTranslateWordsA(std::wstring vKey);

private:

	/**
	* @brief ������ json �������
	* @return ������ ����������� ������
	*/
	bool reloadFiles();

	/**
	* @brief � ������ ��������� ��������/�������� �������� ����������� ������� ���� "�� ����"
	*/
	void loadSafeWords();

	std::map<std::wstring, stWords<std::string>> mapWordsA{};
	std::map<std::wstring, stWords<std::wstring>> mapWordsW{};
};

