/************************************************
*                                               *
*             Project: Snake game               *
*   Coder: Tim4ukys | My url: vk.com/tim4ukys   *
*                                               *
************************************************/
#include "main.h"

CLanguage::stWords<std::string> CLanguage::getTranslateWordsA(std::wstring vKey)
{
	pLog->regLastFnc("CLanguage::getTranslateWordsA()");
	return mapWordsA.at(vKey);
}

CLanguage::stWords<std::wstring> CLanguage::getTranslateWordsW(std::wstring vKey)
{
	pLog->regLastFnc("CLanguage::getTranslateWordsW()");
	return mapWordsW.at(vKey);
}

void CLanguage::loadSafeWords()
{
	pLog->regLastFnc("CLanguage::loadSafeWords()");
	auto key = std::wstring(L"English");
	vKey.push_back(key);
	/////////////////
	mapWordsW[key].play = L"Play";
	mapWordsW[key].exit = L"Exit";
	mapWordsW[key].continue_game = L"Continue";
	mapWordsW[key].back = L"Back";
	mapWordsW[key].settings = {
		L"Settings",
		L"Change the language",
		L"Sound",
		L"Off",
		L"On"
	};
	mapWordsW[key].gameover = L"GAME OVER!";
	mapWordsW[key].win = L"YOU WIN!";

	mapWordsA[key].play = string_from_wstring(mapWordsW[key].play);
	mapWordsA[key].exit = string_from_wstring(mapWordsW[key].exit);
	mapWordsA[key].continue_game = string_from_wstring(mapWordsW[key].continue_game);
	mapWordsA[key].back = string_from_wstring(mapWordsW[key].back);
	mapWordsA[key].gameover = string_from_wstring(mapWordsW[key].gameover);
	mapWordsA[key].win = string_from_wstring(mapWordsW[key].win);
	mapWordsA[key].settings.change_language = string_from_wstring(mapWordsW[key].settings.change_language);
	mapWordsA[key].settings.name = string_from_wstring(mapWordsW[key].settings.name);
	mapWordsA[key].settings.off = string_from_wstring(mapWordsW[key].settings.off);
	mapWordsA[key].settings.on = string_from_wstring(mapWordsW[key].settings.on);
	mapWordsA[key].settings.sound = string_from_wstring(mapWordsW[key].settings.sound);
}

bool CLanguage::reloadFiles()
{
	pLog->regLastFnc("CLanguage::reloadFiles()");
	WIN32_FIND_DATAW files_folders_list{};
	HANDLE hFind = FindFirstFileW((getCurrentPath() + L"\\language\\*.json").c_str(), &files_folders_list);
	if (hFind == INVALID_HANDLE_VALUE) {
		loadSafeWords();
		return false;
	}
	do {
		if (FILE_ATTRIBUTE_DIRECTORY == files_folders_list.dwFileAttributes) continue;

		auto path_file = getCurrentPath() + L"\\language\\" + files_folders_list.cFileName;
		std::ifstream i(path_file);
		json j{};
		i >> j;

		auto key = std::wstring(files_folders_list.cFileName, wcslen(files_folders_list.cFileName) - 5U);
		vKey.push_back(key);
		mapWordsA[key].play = j["PLAY"].get<std::string>();
		mapWordsA[key].exit = j["EXIT"].get<std::string>();
		mapWordsA[key].continue_game = j["CONTINUE"].get<std::string>();
		mapWordsA[key].back = j["BACK"].get<std::string>();
		mapWordsA[key].settings = {
			j["SETTING"]["SETTING_NAME"].get<std::string>(),
			j["SETTING"]["SETTING_CHANGE_LANGUAGE"].get<std::string>(),
			j["SETTING"]["SETTING_SOUND"].get<std::string>(),
			j["SETTING"]["SETTING_OFF"].get<std::string>(),
			j["SETTING"]["SETTING_ON"].get<std::string>()
		};
		mapWordsA[key].gameover = j["GAMEOVER"].get<std::string>();
		mapWordsA[key].win = j["WIN"].get<std::string>();

		/////////////

		mapWordsW[key].play = wstring_from_string(mapWordsA[key].play);
		mapWordsW[key].exit = wstring_from_string(mapWordsA[key].exit);
		mapWordsW[key].continue_game = wstring_from_string(mapWordsA[key].continue_game);
		mapWordsW[key].back = wstring_from_string(mapWordsA[key].back);
		mapWordsW[key].gameover = wstring_from_string(mapWordsA[key].gameover);
		mapWordsW[key].win = wstring_from_string(mapWordsA[key].win);
		mapWordsW[key].settings.change_language = wstring_from_string(mapWordsA[key].settings.change_language);
		mapWordsW[key].settings.name = wstring_from_string(mapWordsA[key].settings.name);
		mapWordsW[key].settings.off = wstring_from_string(mapWordsA[key].settings.off);
		mapWordsW[key].settings.on = wstring_from_string(mapWordsA[key].settings.on);
		mapWordsW[key].settings.sound = wstring_from_string(mapWordsA[key].settings.sound);

	} while (FindNextFileW(hFind, &files_folders_list));
	return true;
}

CLanguage::CLanguage()
{
	pLog->regLastFnc("CLanguage::CLanguage()");
	reloadFiles();
}
CLanguage::~CLanguage()
{

}