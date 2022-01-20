/************************************************
*                                               *
*             Project: Snake game               *
*   Coder: Tim4ukys | My url: vk.com/tim4ukys   *
*                                               *
************************************************/
#include "main.h"

CLog::CLog(std::string fileLogName)
{
	using namespace std;

	fileName = fileLogName;

	ofstream outFileStream{ fileName };

	if (outFileStream.fail()) PostQuitMessage(-1);
	else
	{
		outFileStream << "SNAKE GAME: Started" << endl
			<< "Coder: Tim4ukys. All rights reserved" << endl
			<< "Contact me: vk.com/tim4ukys or tim4ukys.dev@yandex.ru" << endl << endl;

		outFileStream.close();
	}
}

CLog::~CLog()
{

}

void CLog::log(const char* fmt, ...)
{
	std::ofstream outFileStream{ fileName, std::ios_base::app };

	if (!outFileStream.fail())
	{
		char tmp_buf[512]{};

		va_list args = nullptr;
		va_start(args, fmt);
		vsprintf_s(tmp_buf, fmt, args);
		va_end(args);

		SYSTEMTIME time{};
		GetLocalTime(&time);
		char tempBuffTime[15]{};
		sprintf(tempBuffTime, "[%02d:%02d:%02d.%03d]", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		outFileStream << tempBuffTime << ": " << tmp_buf << std::endl;

		outFileStream.close();
	}
}

void CLog::regLastFnc(std::string fncName)
{
#ifdef _DEBUG
	lastFuncName = fncName;
#endif
}