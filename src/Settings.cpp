#include "Settings.h"

struct Game
{
	static void DoBeforeNewOrLoad()
	{
		using func_t = decltype(&DoBeforeNewOrLoad);
		static REL::Relocation<func_t> func{ REL::ID{ 17, 2228951 } };
		return func();
	}

	static void StartNewGame(RE::MainMenu* a_menu)
	{
		using func_t = decltype(&StartNewGame);
		static REL::Relocation<func_t> func{ REL::ID{ 719179, 2249317 } };
		return func(a_menu);
	}
};

bool Settings::CheckKeyPress()
{
	if (skipLoading) {
		return true;
	}

	if (GetAsyncKeyState(KEY) & 0x8000) {
		skipLoading = true;
	}

	return skipLoading;
}

bool Settings::GetValidSave(const RE::BSFixedString& a_name, std::int32_t a_offset) const
{
	return type == (0 + a_offset) ||
	       type == (1 + a_offset) && string::icontains(a_name, "Quicksave") ||
	       type == (2 + a_offset) && string::icontains(a_name, "Autosave") ||
	       type == (3 + a_offset) && string::icontains(a_name, "Save") ||
	       type == (4 + a_offset) && string::icontains(a_name, "ExitSave");
}

void Settings::LoadSettings()
{
	constexpr auto path = L"Data/F4SE/Plugins/po3_StartOnSave.ini";

	CSimpleIniA ini;
	ini.SetUnicode();

	ini.LoadFile(path);

	ini::get_value(ini, specificSave, "Settings", "Save File", ";Auto load specific save. If blank, the last save will be loaded");
	useSpecificSave = !specificSave.empty();

	ini::get_value(ini, charName, "Settings", "Character Name", ";Auto load saves belonging to this character only. If blank, all saves will be considered.");
	useCharName = !charName.empty();

	ini::get_value(ini, type, "Settings", "Save Type", ";Type of save to auto load\n;0 - Last save (any), 1 - Last quicksave, 2 - Last autosave, 3 - Last manual save, 4 - Last exit save.\n;5 - First save (any), 6 - First quicksave, 7 - First autosave, 8 - First manual save, 9 - First exit save.");
	ini::get_value(ini, KEY, "Settings", "Skip AutoLoad Hotkey", ";Skip autoload by pressing this key (default: SHIFT) before the main menu loads.\n;List of keycodes - https://www.indigorose.com/webhelp/ams/Program_Reference/Misc/Virtual_Key_Codes.htm");
	ini::get_value(ini, startNewGame, "Settings", "Start New Game", ";Automatically start a new game if there are no saves.");
	ini::get_value(ini, disableWarning, "Settings", "Disable Missing Content Warning", ";Disable warning messagebox when loading saves with missing mods.");

	(void)ini.SaveFile(path);
}

void Settings::TryAutoLoadGame()
{
	if (CheckKeyPress()) {
		return;
	}

	if (const auto manager = RE::BGSSaveLoadManager::GetSingleton(); manager) {
		if (!manager->isSaveListBuilt) {
			manager->BuildSaveGameList(manager->displayPlayerID);
		}

		const auto& list = manager->saveGameList;
		if (list.empty()) {
			if (startNewGame) {
				if (auto mainMenu = RE::UI::GetSingleton()->GetMenu<RE::MainMenu>()) {
					Game::StartNewGame(mainMenu.get());
				}
			}
			return;
		}

		RE::BGSSaveLoadFileEntry* lastGame = nullptr;

		if (useSpecificSave) {
			if (const auto result = std::ranges::find_if(list, [&](const auto& save) {
					return specificSave == save->fileName;
				});
				result != list.end()) {
				lastGame = *result;
			}
		}

		if (!lastGame) {
			const auto get_valid_save = [&](RE::BGSSaveLoadFileEntry* a_save, std::int32_t a_offset = 0) {
				if (a_save && GetValidSave(a_save->fileName, a_offset)) {
					if (!useCharName || string::iequals(a_save->playerName, charName)) {
						lastGame = a_save;
						return true;
					}
				}
				return false;
			};

			if (type < 5) {
				for (auto& save : list | std::views::reverse) {
					if (get_valid_save(save)) {
						break;
					}
				}
			} else {
				for (auto& save : list) {
					if (get_valid_save(save, 5)) {
						break;
					}
				}
			}
		}

		if (lastGame) {
			F4SE::GetTaskInterface()->AddTask([lastGame, manager, this]() {
				Game::DoBeforeNewOrLoad();
				static REL::Relocation<bool*> gameSystemsShouldUpdate{ REL::ID{ 779552, 2698031 } };
				*gameSystemsShouldUpdate = true;
				manager->queuedEntryToLoad = lastGame;
				if (disableWarning) {
					manager->QueueSaveLoadTask(RE::BGSSaveLoadManager::QUEUED_TASK::kMissingContentLoad);
				} else {
					manager->QueueSaveLoadTask(RE::BGSSaveLoadManager::QUEUED_TASK::kLoadGame);
				}
			});
		}
	}
}
