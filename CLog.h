/************************************************
*                                               *
*             Project: Snake game               *
*   Coder: Tim4ukys | My url: vk.com/tim4ukys   *
*                                               *
************************************************/

/**
* @brief Класс для отладки игры
* @date 17.09.2021
*/
class CLog {
public:

	std::string lastFuncName{};

	CLog(std::string fileLogName);
	~CLog();

	void log(const char* fmt, ...);
	void regLastFnc(std::string fncName);

private:

	std::string fileName{};

};

