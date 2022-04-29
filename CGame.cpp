/************************************************
*                                               *
*             Project: Snake game               *
*   Coder: Tim4ukys | My url: vk.com/tim4ukys   *
*                                               *
************************************************/
#include "main.h" 

#include "CGame.h"
#include "CLanguage.h"
#include "CRender.h"

#include "SDKwavefile.h"

/**
* @brief Возращает путь до шрифта по его "Face name"
* @param faceName "Face name" шрифта
* @return Полный путь до шрифта
*/
inline std::string GetSystemFontFile(const std::string& faceName) 
{
	pLog->regLastFnc("GetSystemFontFile()");
	static const LPWSTR fontRegistryPath = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
	HKEY hKey;
	LONG result;
	std::wstring wsFaceName(faceName.begin(), faceName.end());

	// Open Windows font registry key
	result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, fontRegistryPath, 0, KEY_READ, &hKey);
	if (result != ERROR_SUCCESS) {
		return "";
	}

	DWORD maxValueNameSize, maxValueDataSize;
	result = RegQueryInfoKey(hKey, 0, 0, 0, 0, 0, 0, 0, &maxValueNameSize, &maxValueDataSize, 0, 0);
	if (result != ERROR_SUCCESS) {
		return "";
	}

	DWORD valueIndex = 0;
	LPWSTR valueName = new WCHAR[maxValueNameSize];
	LPBYTE valueData = new BYTE[maxValueDataSize];
	DWORD valueNameSize, valueDataSize, valueType;
	std::wstring wsFontFile;

	// Look for a matching font name
	do {

		wsFontFile.clear();
		valueDataSize = maxValueDataSize;
		valueNameSize = maxValueNameSize;

		result = RegEnumValue(hKey, valueIndex, valueName, &valueNameSize, 0, &valueType, valueData, &valueDataSize);

		valueIndex++;

		if (result != ERROR_SUCCESS || valueType != REG_SZ) {
			continue;
		}

		std::wstring wsValueName(valueName, valueNameSize);

		// Found a match
		if (_wcsnicmp(wsFaceName.c_str(), wsValueName.c_str(), wsFaceName.length()) == 0) {

			wsFontFile.assign((LPWSTR)valueData, valueDataSize);
			break;
		}
	} while (result != ERROR_NO_MORE_ITEMS);

	delete[] valueName;
	delete[] valueData;

	RegCloseKey(hKey);

	if (wsFontFile.empty()) {
		return "";
	}

	// Build full font file path
	wchar_t winDir[MAX_PATH];
	GetWindowsDirectoryW(winDir, MAX_PATH);

	std::wstringstream ss{};
	ss << winDir << "\\Fonts\\" << wsFontFile;
	wsFontFile = ss.str();

	return std::string(wsFontFile.begin(), wsFontFile.end());
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void CGame::WNDProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	pLog->regLastFnc("CGame::WNDProc()");
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) return;

	switch (uMsg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE && game_state == GAME_STATE::GAME_STATE_PLAY) 
			game_state = GAME_STATE::GAME_STATE_PAUSE;
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		int	iKey = int(wParam);
		uint32_t ScanCode = LOBYTE(HIWORD(lParam));


		if (game_state == GAME_STATE::GAME_STATE_PLAY)
		{
			switch (iKey)
			{
			case VK_UP:
			{
				auto size = vDirect.size();
				if (size < 5U) {
					if (size > 0U) {
						if (vDirect.at(size - 1U) != DIRECT::DIRECT_DOWN)
							vDirect.push_back(DIRECT::DIRECT_UP);
					}
					else if (directCurrent != DIRECT::DIRECT_DOWN) vDirect.push_back(DIRECT::DIRECT_UP);
				}
			}
			break;
			case VK_DOWN:
			{
				auto size = vDirect.size();
				if (size < 5U) {
					if (size > 0U) {
						if (vDirect.at(size - 1U) != DIRECT::DIRECT_UP)
							vDirect.push_back(DIRECT::DIRECT_DOWN);
					}
					else if (directCurrent != DIRECT::DIRECT_UP) vDirect.push_back(DIRECT::DIRECT_DOWN);
				}
			}
			break;
			case VK_LEFT:
			{
				auto size = vDirect.size();
				if (size < 5U) {
					if (size > 0U) {
						if (vDirect.at(size - 1U) != DIRECT::DIRECT_RIGHT)
							vDirect.push_back(DIRECT::DIRECT_LEFT);
					}
					else if (directCurrent != DIRECT::DIRECT_RIGHT) vDirect.push_back(DIRECT::DIRECT_LEFT);
				}
			}
			break;
			case VK_RIGHT:
			{
				auto size = vDirect.size();
				if (size < 5U) {
					if (size > 0U) {
						if (vDirect.at(size - 1U) != DIRECT::DIRECT_LEFT)
							vDirect.push_back(DIRECT::DIRECT_RIGHT);
					}
					else if (directCurrent != DIRECT::DIRECT_LEFT) vDirect.push_back(DIRECT::DIRECT_RIGHT);
				}
			}
			break;
			case VK_ESCAPE:
			{
				game_state = GAME_STATE::GAME_STATE_PAUSE;
			}
			break;
			}
		}
		else if (game_state == GAME_STATE::GAME_STATE_PAUSE)
		{
			switch (iKey)
			{
			case VK_ESCAPE:
				if (menu_id == MENU_ID::MENU_ID_SETTING) {
					music.play(stMusic::MUSIC::MUSIC_MENU_BACK, TRUE);
					menu_id = MENU_ID::MENU_ID_MAIN;
				}
				else if (menu_id == MENU_ID::MENU_ID_MAIN) game_state = GAME_STATE::GAME_STATE_PLAY;
				else if (menu_id == MENU_ID::MENU_ID_SETTING_CHANGELANGUAFE) {
					music.play(stMusic::MUSIC::MUSIC_MENU_BACK, TRUE);
					menu_id = MENU_ID::MENU_ID_SETTING;
				}
				break;
			}
		}
		else if (game_state == GAME_STATE::GAME_STATE_PAUSE_NOSTARTGAME)
		{
			switch (iKey)
			{
			case VK_ESCAPE:
				if (menu_id == MENU_ID::MENU_ID_SETTING) {
					music.play(stMusic::MUSIC::MUSIC_MENU_BACK, TRUE);
					menu_id = MENU_ID::MENU_ID_MAIN;
				}
				else if (menu_id == MENU_ID::MENU_ID_SETTING_CHANGELANGUAFE) {
					music.play(stMusic::MUSIC::MUSIC_MENU_BACK, TRUE);
					menu_id = MENU_ID::MENU_ID_SETTING;
				}
				break;
			}
		}
	}
	break;
	}
}

bool CGame::stTick::isReady()
{
	pLog->regLastFnc("CGame::stTick::isReady()");
	TICK currentTime = GetTickCount64();
	if (!m_startTime) m_startTime = currentTime;
	
	if (float t = 2 * ((currentTime - m_startTime) % DELAY) / static_cast<float>(DELAY); t > 1.0f)
	{
		m_startTime = currentTime;
		return true;
	}

	return false;
}

float CGame::getSizeBlock()
{
	pLog->regLastFnc("CGame::getSizeBlock()");
	return static_cast<float>(pRender->pDevicePTR.BackBufferWidth - 20) / maxBlock;
}
void CGame::getBlockPos(uint8_t id_x, uint8_t id_y, float* pos)
{
	pLog->regLastFnc("CGame::getBlockPos()");
	//pos[0] = trunc(pRender->pDevicePTR.BackBufferWidth / (id_x * 20))  + 10;
	//pos[1] = trunc(pRender->pDevicePTR.BackBufferHeight / (id_x * 20)) + 10;
	pos[0] = getSizeBlock() * id_x + 10;
	pos[1] = getSizeBlock() * id_y + 10;
}

void CGame::changeLanguage()
{
	pLog->regLastFnc("CGame::changeLanguage()");
	const int sizePage{ 5 };
	const int spacePage{ 30 };
	const int startOffsetX{ 10 };

	static uint8_t currentPage{ '\x0' };
	static size_t lenPage = multi_lang.pLanguage->vKey.size() / sizePage + (multi_lang.pLanguage->vKey.size() % 5 ? 1 : 0 );

	if (menuButton(u8"\u25B2", { static_cast<float>(pRender->pDevicePTR.BackBufferWidth) / 2 - ImGui::CalcTextSize(u8"\u25B2").x / 2, 7 }) && currentPage > '\x0')
		currentPage--;

	if (menuButton(u8"\u25BC", { static_cast<float>(pRender->pDevicePTR.BackBufferWidth) / 2 - ImGui::CalcTextSize(u8"\u25BC").x / 2, 475 }) && currentPage < lenPage - 1)
		currentPage++;

	auto *key_name = &multi_lang.pLanguage->vKey;
	for (int i = 0; i < sizePage; i++)
	{
		size_t offset = currentPage * sizePage;
		auto currentNumbPage = offset + i;
		if (currentNumbPage >= key_name->size()) break;

		auto text = string_from_wstring(key_name->at(currentNumbPage));
		ImVec2 pos{ pRender->pDevicePTR.BackBufferWidth / 2 - ImGui::CalcTextSize(text.c_str()).x / 2,
			float(pRender->pDevicePTR.BackBufferHeight / 2) / sizePage * (i + 1) + startOffsetX + (i * spacePage) };

		if (menuButton(text, pos)) {
			multi_lang.key = key_name->at(currentNumbPage);
			menu_id = MENU_ID::MENU_ID_SETTING;
		}
	}
}

bool CGame::menuCheckbox(std::string text, bool* check_state, size_t totalQuant, size_t currentQuant, ImVec2 pos)
{
	pLog->regLastFnc("CGame::menuCheckbox()");
	text += ": ";

	struct stCheckboxData {
		bool isPlayMusic{}, state{};
	};
	static std::map<std::string, stCheckboxData> mapCheckboxData{};

	auto words = multi_lang.pLanguage->getTranslateWordsA(multi_lang.key);

	ImVec2 size_off = ImGui::CalcTextSize(words.settings.off.c_str());
	ImVec2 size = ImGui::CalcTextSize(text.c_str());
	ImVec2 new_pos = { pos.x - (size.x + size_off.x) / 2, (pos.y / totalQuant /*- size.y*/) * currentQuant};

	ImGui::SetCursorPos(new_pos);
	bool result = ImGui::InvisibleButton(text.c_str(), { size.x + size_off.x, size.y });
	auto isHovered = ImGui::IsItemHovered();

	if (result && !mapCheckboxData[text].state) {
		music.play(stMusic::MUSIC::MUSIC_MENU_ENTER, TRUE);

		*check_state ^= true;
		mapCheckboxData[text].state = true;
	}
	else if (!result) {
		mapCheckboxData[text].state = false;
	}

	if (!mapCheckboxData[text].isPlayMusic && ImGui::IsItemHovered() && !result) {
		mapCheckboxData[text].isPlayMusic = true;
		music.play(stMusic::MUSIC::MUSIC_MENU_CHANGE, TRUE);
	}
	else if (!isHovered) {
		mapCheckboxData[text].isPlayMusic = false;
	}

	ImGui::SetCursorPos(new_pos);
	ImGui::TextColored((isHovered ? color.buttonActive : color.buttonDisable), text.c_str());
	ImGui::SetCursorPos({ new_pos.x + size.x, new_pos.y });
	ImGui::TextColored((isHovered ? color.buttonActive : color.buttonDisable), (*check_state ? words.settings.on : words.settings.off).c_str());

	return result;
}
bool CGame::menuButton(std::string text, size_t totalQuant, size_t currentQuant, ImVec2 pos, bool isBack)
{
	pLog->regLastFnc("CGame::menuButton()");
	ImVec2 size = ImGui::CalcTextSize(text.c_str());
	ImVec2 new_pos = { pos.x - size.x / 2, (pos.y / totalQuant /*- size.y*/) * currentQuant };

	ImGui::SetCursorPos(new_pos);
	bool result = ImGui::InvisibleButton(text.c_str(), size);

	ImGui::SetCursorPos(new_pos);
	ImGui::TextColored((ImGui::IsItemHovered() ? color.buttonActive : color.buttonDisable), text.c_str());

	static std::map<std::string, bool> mapButtonData{};
	if (!mapButtonData[text] && ImGui::IsItemHovered() && !result) {
		mapButtonData[text] = true;
		music.play(stMusic::MUSIC::MUSIC_MENU_CHANGE, TRUE);
	}
	else if (result) {
		music.play((!isBack ? stMusic::MUSIC::MUSIC_MENU_ENTER : stMusic::MUSIC::MUSIC_MENU_BACK), TRUE);
	}
	else if (!ImGui::IsItemHovered()) {
		mapButtonData[text] = false;
	}

	return result;
}
bool CGame::menuButton(std::string text, ImVec2 pos, bool isBack)
{
	pLog->regLastFnc("CGame::menuButton()");
	ImVec2 size = ImGui::CalcTextSize(text.c_str());

	ImGui::SetCursorPos(pos);
	bool result = ImGui::InvisibleButton(text.c_str(), size);

	ImGui::SetCursorPos(pos);
	ImGui::TextColored((ImGui::IsItemHovered() ? color.buttonActive : color.buttonDisable), text.c_str());

	static std::map<std::string, bool> mapButtonData{};
	if (!mapButtonData[text] && ImGui::IsItemHovered() && !result) {
		mapButtonData[text] = true;
		music.play(stMusic::MUSIC::MUSIC_MENU_CHANGE, TRUE);
	}
	else if (result) {
		music.play((!isBack ? stMusic::MUSIC::MUSIC_MENU_ENTER : stMusic::MUSIC::MUSIC_MENU_BACK), TRUE);
	}
	else if (!ImGui::IsItemHovered()) {
		mapButtonData[text] = false;
	}

	return result;
}

void CGame::renderBox(int id_x, int id_y, float size, D3DCOLOR color)
{
	pLog->regLastFnc("CGame::renderBox()");
	float pos[2]{};
	getBlockPos(id_x, id_y, pos);
	pRender->render.D3DBox(pos[0], pos[1], size, size, color);
}
void CGame::Render(IDirect3DDevice9* pDevice)
{
	pLog->regLastFnc("CGame::Render()");

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::PushFont(game_over.pFont);
	if (game_state != GAME_STATE::GAME_STATE_PAUSE_NOSTARTGAME)
	{

		// Рисуем границы 
		{
			auto maxHeight = pRender->pDevicePTR.BackBufferHeight, // Высота. n = window size - 39
				maxWidth = pRender->pDevicePTR.BackBufferWidth; // Ширина. n = window size - 16

			pRender->render.D3DBox(0, 0, 10, static_cast<float>(maxHeight), color.borders); // Левая стенка
			pRender->render.D3DBox(10, static_cast<float>(maxHeight - 10), static_cast<float>(maxWidth - 10), 10, color.borders); // Нижния стенка
			pRender->render.D3DBox(static_cast<float>(maxWidth - 10), 0, 10, static_cast<float>(maxHeight - 10), color.borders); // Правая стенка
			pRender->render.D3DBox(10, 0, static_cast<float>(maxWidth - 20), 10, color.borders); // Верхнея стенка
		}
		auto size = getSizeBlock();

		if (tick.isReady() && !game_over.isActive && game_state == GAME_STATE::GAME_STATE_PLAY && !win.isWin)
		{
			if (vDirect.size() > 0U) {
				directCurrent = vDirect.at(0);
				vDirect.erase(vDirect.begin(), vDirect.begin() + 1U);
			}
			switch (directCurrent)
			{
			case CGame::DIRECT::DIRECT_UP:
				if (snake.vSnake.at(0).y > 0) snake.move(blocksEx(snake.vSnake.at(0).x, snake.vSnake.at(0).y - '\x1'));
				else game_over.isActive = true;
				break;
			case CGame::DIRECT::DIRECT_DOWN:
				if (snake.vSnake.at(0).y < maxBlock - 1) snake.move(blocksEx(snake.vSnake.at(0).x, snake.vSnake.at(0).y + '\x1'));
				else game_over.isActive = true;
				break;
			case CGame::DIRECT::DIRECT_LEFT:
				if (snake.vSnake.at(0).x > 0) snake.move(blocksEx(snake.vSnake.at(0).x - '\x1', snake.vSnake.at(0).y));
				else game_over.isActive = true;
				break;
			case CGame::DIRECT::DIRECT_RIGHT:
				if (snake.vSnake.at(0).x < maxBlock - 1) snake.move(blocksEx(snake.vSnake.at(0).x + '\x1', snake.vSnake.at(0).y));
				else game_over.isActive = true;
				break;
			}

			for (size_t f = 1; f < snake.vSnake.size(); f++)
			{
				if (snake.vSnake.at(0).x == snake.vSnake.at(f).x && snake.vSnake.at(0).y == snake.vSnake.at(f).y) game_over.isActive = true;
			}

			if (game_over.isActive) {
				music.play(stMusic::MUSIC::MUSIC_DEATH, FALSE);
			}
		}

		if (snake.vSnake.at(0).x == apple.x && snake.vSnake.at(0).y == apple.y && !win.isWin) {
			snake.add();
			static const size_t winSize = (maxBlock - 1) * (maxBlock - 1);
			if (snake.vSnake.size() == winSize)
			{
				win.isWin = true;
				music.play(stMusic::MUSIC::MUSIC_WIN);
			}
			else
			{
				updateApplePos();
				if (tick.DELAY > 350) tick.DELAY -= 75;
				music.play(stMusic::MUSIC::MUSIC_NYAM, TRUE);
			}
		}

		renderBox(apple.x, apple.y, size, color.apple);

		//getBlockPos(posID[0], posID[1], pos);
		//pRender->render.D3DBox(pos[0], pos[1], size, size, D3DCOLOR_XRGB(214, 0, 0));
		for (size_t i = 0; i < snake.vSnake.size(); i++)
		{
			//float pos[2]{};
			//getBlockPos(snake.vSnake.at(i).x, snake.vSnake.at(i).y, pos);
			//pRender->render.D3DBox(pos[0], pos[1], size, size, (i ? color.snakeBody : color.snakeHead) );
			renderBox(snake.vSnake.at(i).x, snake.vSnake.at(i).y, size, (i ? color.snakeBody : color.snakeHead));
		}

		if (win.isWin) {
			auto win_str = multi_lang.pLanguage->getTranslateWordsA(multi_lang.key).win;
			ImVec2 pos{ pRender->pDevicePTR.BackBufferWidth / 2 - ImGui::CalcTextSize(win_str.c_str()).x / 2, static_cast<float>(pRender->pDevicePTR.BackBufferHeight / 2) };

			pRender->render.D3DBox(0, 0, pRender->pDevicePTR.BackBufferWidth, pRender->pDevicePTR.BackBufferHeight,
				ImGui::GetColorU32({ 0, 0, 0, win.alpha_background }));
			auto bckgrnd = ImGui::GetBackgroundDrawList();
			bckgrnd->AddText({ pos.x, pos.y + win.y }, ImGui::GetColorU32({ color.GameOver.x, color.GameOver.y, color.GameOver.z, win.alpha }),
				win_str.c_str());

			if (win.y > 0.0f) win.y -= 0.625f;

			if (win.alpha_background < 0.75f) win.alpha_background += 0.005f;
			else if (win.alpha < 1.0f) win.alpha += 0.01f;
		}

		if (game_over.isActive && !win.isWin)
		{
			renderBox(snake.vSnake.at(0).x, snake.vSnake.at(0).y, size, color.snakeDead);

			auto bckgrnd = ImGui::GetBackgroundDrawList();
			auto gmvr_str = multi_lang.pLanguage->getTranslateWordsA(multi_lang.key).gameover;
			static ImVec2 gmvr_pos{ pRender->pDevicePTR.BackBufferWidth / 2 - ImGui::CalcTextSize(gmvr_str.c_str()).x / 2,
				pRender->pDevicePTR.BackBufferHeight / 2 - ImGui::CalcTextSize(gmvr_str.c_str()).y / 2 };

			bckgrnd->AddText(gmvr_pos, ImGui::GetColorU32(color.GameOver), gmvr_str.c_str());
		}
	}
	if (game_state != GAME_STATE::GAME_STATE_PLAY)
	{
		static bool use_work_area = true;
		static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings 
			| ImGuiWindowFlags_NoTitleBar;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
		ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

		ImGui::Begin("menu", NULL, flags);

		auto translWords = multi_lang.pLanguage->getTranslateWordsA(multi_lang.key);

		switch (menu_id)
		{
		case CGame::MENU_ID::MENU_ID_MAIN:
			if (menuButton((game_state == GAME_STATE::GAME_STATE_PAUSE_NOSTARTGAME ? translWords.play : translWords.continue_game), 3, 1, 
				ImVec2(pRender->pDevicePTR.BackBufferWidth / 2, pRender->pDevicePTR.BackBufferHeight / 2)))
				game_state = GAME_STATE::GAME_STATE_PLAY;
			if (menuButton(translWords.settings.name, 3, 2, ImVec2(pRender->pDevicePTR.BackBufferWidth / 2, pRender->pDevicePTR.BackBufferHeight / 2)))
				menu_id = MENU_ID::MENU_ID_SETTING;
			if (menuButton(translWords.exit, 3, 3, ImVec2(pRender->pDevicePTR.BackBufferWidth / 2, pRender->pDevicePTR.BackBufferHeight / 2)))
				PostQuitMessage(0);
			break;
		case CGame::MENU_ID::MENU_ID_SETTING:
			if (menuButton(translWords.settings.change_language, 3, 1, ImVec2(pRender->pDevicePTR.BackBufferWidth / 2, pRender->pDevicePTR.BackBufferHeight / 2)))
				menu_id = MENU_ID::MENU_ID_SETTING_CHANGELANGUAFE;
			if (menuButton(translWords.back, 3, 3, ImVec2(pRender->pDevicePTR.BackBufferWidth / 2, pRender->pDevicePTR.BackBufferHeight / 2), true))
			{
				menu_id = MENU_ID::MENU_ID_MAIN;
				saveConfig();
			}

			menuCheckbox(translWords.settings.sound, &music.state, 3, 2, ImVec2(pRender->pDevicePTR.BackBufferWidth / 2, pRender->pDevicePTR.BackBufferHeight / 2));
			break;
		case CGame::MENU_ID::MENU_ID_SETTING_CHANGELANGUAFE:
			changeLanguage();
			break;
		}

		ImGui::End();
	}

	ImGui::PopFont();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

blocksEx::blocksEx(uint8_t xnew, uint8_t ynew)
{
	pLog->regLastFnc("blocksEx::blocksEx()");
	x = xnew; y = ynew;
}

CSnake::CSnake(uint8_t len)
{
	pLog->regLastFnc("CSnake::CSnake()");
	for (uint8_t i = 0; i < len; i++)
		vSnake.push_back(blocksEx(START_COORD[0] - i, START_COORD[1]));
}

void CSnake::move(blocksEx new_blocks)
{
	pLog->regLastFnc("CSnake::move()");
	auto temp = vSnake;
	vSnake.at(0) = new_blocks;

	auto safeSize = vSnake.size();
	for (size_t i = 1; i < safeSize; i++) {
		vSnake.at(i) = temp.at(i - 1);

		if (isAdd && i == safeSize - 1U) {
			vSnake.push_back(temp.at(i));
			isAdd = false;
		}
	}
}

void CSnake::add()
{
	pLog->regLastFnc("CSnake::add()");
	isAdd = true;
}

std::vector<IXAudio2SourceVoice*> g_pDestMusic{};
HANDLE g_hThreadCleanMusicThash;
void cleanMusicTrash(PVOID param)
{
	while (!*reinterpret_cast<PBOOL>(param))
	{
		Sleep(1000);
		for (size_t i{}; i < g_pDestMusic.size(); i++)
		{
			auto& pMusic = g_pDestMusic.at(i);
			XAUDIO2_VOICE_STATE state;
			pMusic->GetState(&state);
			if (!state.BuffersQueued) {
				pMusic->DestroyVoice();
				g_pDestMusic.erase(g_pDestMusic.begin() + (int)i);
			}
		}
	}
}

void CGame::stMusic::play(MUSIC music_id, BOOL restart)
{
	pLog->regLastFnc("CGame::stMusic::play()");
	//if (state) BASS_ChannelPlay(arrMusic.at(music_id), restart);
	if (state) {
		auto& m = arrMusic.at(music_id);

		IXAudio2SourceVoice* pSourceVoice;
		WAVEFORMATEX* pwfx = m.first.m_pWAV->GetFormat();
		pXAudio2->CreateSourceVoice(&pSourceVoice, pwfx);

		g_pDestMusic.push_back(pSourceVoice);

		XAUDIO2_BUFFER buffer = { 0 };
		buffer.pAudioData = m.first.m_pbWaveData;
		buffer.Flags = XAUDIO2_END_OF_STREAM;  // tell the source voice not to expect any data after this buffer
		buffer.AudioBytes = m.first.m_cbWaveSize;
		pSourceVoice->SubmitSourceBuffer(&buffer);

		pSourceVoice->Start();
	}
}

void CGame::loadConfig()
{
	pLog->regLastFnc("CGame::loadConfig()");
	std::ifstream inFileStream{ getCurrentPath() + L"\\config.json" };
	if (inFileStream.fail()) {
		multi_lang.key = L"English";

		std::ofstream outFileStream{ getCurrentPath() + L"\\config.json" };
		if (!outFileStream.fail()) {

			json j{};
			j["langKey"] = string_from_wstring(multi_lang.key);
			j["soundState"] = false;

			outFileStream << j.dump(4);
			outFileStream.close();
		}
	}
	else {
		json j{};
		inFileStream >> j;

		multi_lang.key = wstring_from_string(j["langKey"].get<std::string>());
		music.state = j["soundState"].get<bool>();

		inFileStream.close();
	}
}

void CGame::saveConfig()
{
	pLog->regLastFnc("CGame::saveConfig()");
	std::ofstream outFileStream{ getCurrentPath() + L"\\config.json" };
	if (!outFileStream.fail()) {
		json j{};

		j["langKey"] = string_from_wstring(multi_lang.key);
		j["soundState"] = music.state;

		outFileStream << j.dump(4);
		outFileStream.close();
	}
}

CGame::CGame(HWND hwnd, LPDIRECT3DDEVICE9 pDevice)
{
	pLog->regLastFnc("CGame::CGame()");

	auto fnSoundInit = [this]()->bool {
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		
		if (FAILED(XAudio2Create(&music.pXAudio2)))
		{
			CoUninitialize();
			return false;
		}

		if (UINT32 count; !FAILED(music.pXAudio2->GetDeviceCount(&count)))
		{
			for (size_t i{}; i < count; i++)
			{
				XAUDIO2_DEVICE_DETAILS dev;

				if (!FAILED(music.pXAudio2->GetDeviceDetails(i, &dev)))
					pLog->log("dev[%d].DisplayName: %s", i, string_from_wstring(dev.DisplayName).c_str());
			}
		}

		if (FAILED(music.pXAudio2->CreateMasteringVoice(&music.pMasteringVoice)))
		{
			SAFE_RELEASE(music.pXAudio2);
			CoUninitialize();
			return false;
		}

		g_hThreadCleanMusicThash = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&cleanMusicTrash, &music.m_nIsStopClearThread, NULL, NULL);

		return true;
	};

	if (!fnSoundInit()/*BASS_Init(-1, 44100, 0, 0, NULL)*/)
		PostQuitMessage(-1);

	for (size_t i = 0; i < music.arrMusic.size(); i++)
	{
		std::wstring path = getCurrentPath() + L"\\music\\" + std::to_wstring(i + 1) + L".wav";

		CWaveFile* pWav = music.arrMusic.at(i).first.m_pWAV = new CWaveFile();
		if (FAILED(pWav->Open((LPWSTR)path.c_str(), NULL, WAVEFILE_READ)))
		{
			continue;
		}

		DWORD cbWaveSize = music.arrMusic.at(i).first.m_cbWaveSize = pWav->GetSize();

		// Read the sample data into memory
		BYTE* pbWaveData = music.arrMusic.at(i).first.m_pbWaveData = new BYTE[cbWaveSize];

		if (FAILED(pWav->Read(pbWaveData, cbWaveSize, &cbWaveSize)))
		{
			SAFE_DELETE_ARRAY(music.arrMusic.at(i).first.m_pbWaveData);
			continue;
		}
	}

	multi_lang.pLanguage = new CLanguage();
	loadConfig();


	//pRender->getDevice()->ShowCursor(FALSE);

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(hwnd);

	ImGui::GetIO().MouseDoubleClickTime = 0.8f;
	ImGui::GetIO().Fonts->AddFontFromFileTTF(GetSystemFontFile("Segoe UI").c_str(), 14.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	{
		auto cpath = getCurrentPath() + L"\\font\\vga-8x16.ttf";
		static const ImWchar icons_ranges[] = { 0x0020, 0xfffd, 0 };
		game_over.pFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(string_from_wstring(cpath).c_str(), 67.0f, NULL, icons_ranges/*67.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic()*/);
	}
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImGui_ImplDX9_Init(pDevice);

	updateApplePos();

	//ImGui_ImplDX9_CreateDeviceObjects();
}
CGame::~CGame()
{
	if (multi_lang.pLanguage) delete multi_lang.pLanguage;

	//TerminateThread(g_hThreadCleanMusicThash, -1);
	music.m_nIsStopClearThread = TRUE;
	WaitForSingleObject(g_hThreadCleanMusicThash, INFINITE);
	CloseHandle(g_hThreadCleanMusicThash);

	for (auto& pMusic : g_pDestMusic)
	{
		pMusic->DestroyVoice();
	}
	g_pDestMusic.clear();

	for (size_t i = 0; i < music.arrMusic.size(); i++)
	{
		/*HSTREAM ch = music.arrMusic.at(i);
		BASS_ChannelStop(ch);
		BASS_ChannelFree(ch);*/

		//music.arrMusic.at(i).second->DestroyVoice();
		SAFE_DELETE(music.arrMusic.at(i).first.m_pWAV);
		SAFE_DELETE_ARRAY(music.arrMusic.at(i).first.m_pbWaveData);
	}

	saveConfig();

	//BASS_Free();
	music.pMasteringVoice->DestroyVoice();

	SAFE_RELEASE(music.pXAudio2);
	CoUninitialize();
}

void CGame::updateApplePos()
{
	pLog->regLastFnc("CGame::updateApplePos()");
	int xn{}, yn{};
	bool rel{};
	do {
		xn = random<int>(0, maxBlock - 1), yn = random<int>(0, maxBlock - 1);
		for (size_t i = 0; i < snake.vSnake.size(); i++) {
			if (snake.vSnake.at(i).x == xn && snake.vSnake.at(i).y == yn) break;

			if (snake.vSnake.size() - 1U == i) rel = true;
		}
	} while (!rel);

	apple = { xn, yn };
}