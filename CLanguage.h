/************************************************
*                                               *
*             Project: Snake game               *
*   Coder: Tim4ukys | My url: vk.com/tim4ukys   *
*                                               *
************************************************/

/**
* @brief Класс для удобного перевода игры
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
	* @brief Возращает объект класса stWords c переведёнными словами
	* @details Строки в классе будут типом std::wstring
	* @param vKey Ключик
	* @return объект класса stWords c переведёнными словами
	*/
	stWords<std::wstring>	getTranslateWordsW(std::wstring vKey);

	/**
	* @brief Возращает объект класса stWords c переведёнными словами
	* @details Строки в классе будут типом std::string
	* @param vKey Ключик
	* @return объект класса stWords c переведёнными словами
	*/
	stWords<std::string>	getTranslateWordsA(std::wstring vKey);

private:

	/**
	* @brief Парсит json конфиги
	* @return Статус проделанной работы
	*/
	bool reloadFiles();

	/**
	* @brief В случае неудачной загрузки/парсинга конфигов загружается словарь прям "из кода"
	*/
	void loadSafeWords();

	std::map<std::wstring, stWords<std::string>> mapWordsA{};
	std::map<std::wstring, stWords<std::wstring>> mapWordsW{};
};

