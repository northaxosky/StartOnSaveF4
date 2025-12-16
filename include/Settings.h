#pragma once

class Settings
{
public:
	[[nodiscard]] static Settings* GetSingleton()
	{
		static Settings singleton;
		return std::addressof(singleton);
	}

	bool CheckKeyPress();

	[[nodiscard]] bool GetValidSave(const RE::BSFixedString& a_name, std::int32_t a_offset = 0) const;

	void LoadSettings();

	void TryAutoLoadGame();

	// members
	std::string specificSave{};
	bool useSpecificSave{ false };
	std::string charName{};
	bool useCharName{};
	std::int32_t type{ 0 };
	bool disableWarning{ false };
	int KEY{ 16 };
	bool skipLoading{ false };
	bool startNewGame{ true };
};
