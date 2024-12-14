#include "LevelUp.h"
#include "MessageBoxes.h"

namespace LevelUpMenu
{
	//reimplemantation of RE::CreateMessage
	template <typename... T>
	void CreateMessage(const char* a_message, void (*a_callback)(uint8_t), std::uint32_t a_arg3, std::uint32_t a_arg4,
		std::uint32_t a_arg5, T... argz)
	{
		using func_t = decltype(&CreateMessage<T...>);
		REL::Relocation<func_t> func{ RELOCATION_ID(51420, 52269) };
		return func(a_message, a_callback, a_arg3, a_arg4, a_arg5, argz...);
	}

	void ShowLevelUpOptions(const std::vector<std::string>& options)
	{
		std::vector<const char*> args;
		for (const auto& option : options) 
		{
			args.push_back(option.c_str());
		}

		std::string levelUpMessage =
			"Вы достигли следующего уровня, выберете какую характеристику персонажа вы хотите повысить.\n Максимальное значение любой характеристики — 12, при достижении этого значения вы больше не сможете её повышать";

		switch (args.size())
		{
		case 1:
			CreateMessage(levelUpMessage.c_str(),
				LevelUpCallbak,
				0, 25, 10, args[0], nullptr);
			break;
		case 2:
			CreateMessage(levelUpMessage.c_str(),
				LevelUpCallbak,
				0, 25, 10, args[0], args[1], nullptr);
			break;
		case 3:
			CreateMessage(levelUpMessage.c_str(),
				LevelUpCallbak,
				0, 25, 10, args[0], args[1], args[2], nullptr);
			break;
		case 4:
			CreateMessage(levelUpMessage.c_str(),
				LevelUpCallbak,
				0, 25, 10, args[0], args[1], args[2], args[3], nullptr);
			break;
		case 5:
			CreateMessage(levelUpMessage.c_str(),
				LevelUpCallbak,
				0, 25, 10, args[0], args[1], args[2], args[3], args[4], nullptr);
			break;
		case 6:
			CreateMessage(levelUpMessage.c_str(),
				LevelUpCallbak,
				0, 25, 10, args[0], args[1], args[2], args[3], args[4], args[5], nullptr);
			break;
		default:
			CreateMessage("Все характеристики достигли максимума. Теперь вы будете получать только перки при повышении уровня.",
				LevelUpCallbak, 0, 25, 10, "Ок");
			break;
		}
	}

	std::vector<std::pair<std::string, RE::TESGlobal*>> PrepareLevelUpOptions() 
	{
		auto strength = RE::TESForm::LookupByEditorID<RE::TESGlobal>("GACHI_Stat_strength");
		auto agility = RE::TESForm::LookupByEditorID<RE::TESGlobal>("GACHI_Stat_agility");
		auto endurance = RE::TESForm::LookupByEditorID<RE::TESGlobal>("GACHI_Stat_endurance");
		auto intelligence = RE::TESForm::LookupByEditorID<RE::TESGlobal>("GACHI_Stat_intelligence");
		auto wisdom = RE::TESForm::LookupByEditorID<RE::TESGlobal>("GACHI_Stat_wisdom");
		auto resistance = RE::TESForm::LookupByEditorID<RE::TESGlobal>("GACHI_Stat_Resistance");

		std::vector<std::pair<std::string, RE::TESGlobal*>> options = 
		{ 
			{ fmt::format("Сила [{}]", strength->value), strength },
			{ fmt::format("Ловкость [{}]", agility->value), agility },
			{ fmt::format("Выносливость [{}]", endurance->value), endurance },
			{ fmt::format("Интеллект [{}]", intelligence->value), intelligence },
			{ fmt::format("Мудрость [{}]", wisdom->value), wisdom },
			{ fmt::format("Сопротивление [{}]", resistance->value), resistance } 
		};

		options.erase(
			std::remove_if(options.begin(), options.end(), [](const auto& option) { return option.second->value >= 12; }),
			options.end());

		return options;
	}

	void UpdateLevelUpMessageBoxesInQueue() 
	{
		std::vector<std::pair<std::string, RE::TESGlobal*>> options = PrepareLevelUpOptions();

		const auto& msgs = *(REL::Relocation<RE::BSTArray<RE::MessageBoxData*>*>(REL::ID(519819)));

		auto menu = RE::UI::GetSingleton()->GetMenu<RE::MessageBoxMenu>().get();

		bool allStatsMaxed =
			std::all_of(options.begin(), options.end(), [](const auto& option) { return option.second->value >= 12; });

		if (allStatsMaxed) 
		{
			for (auto msg : msgs) 
			{
				msg->bodyText =
					"Все характеристики достигли максимума. Теперь вы будете получать только перки при повышении уровня.";
				msg->buttonText.clear();
				msg->buttonText.push_back("Ок");
			}
			RE::FxResponseArgs<2> textArgs;
			textArgs.Add("Все характеристики достигли максимума. Теперь вы будете получать только перки при повышении уровня.");
			textArgs.Add(false);
			RE::FxDelegate::Invoke(menu->uiMovie.get(), "setMessageText", textArgs);

			RE::FxResponseArgs<2> btnArgs;
			btnArgs.Add(" ");
			btnArgs.Add("Ок");
			RE::FxDelegate::Invoke(menu->uiMovie.get(), "setButtons", btnArgs);

			return;
		}

		std::vector<std::string> filteredOptions;

		for (const auto& option : options) 
		{
			filteredOptions.push_back(option.first);
		}

		for (auto msg : msgs) {
			msg->buttonText.clear();
			for (const auto& text : filteredOptions) 
			{
				msg->buttonText.push_back(text.c_str());
			}
		}

		RE::FxResponseArgs<7> btnArgs;
		btnArgs.Add(" ");
		for (const auto& buttonText : filteredOptions) 
		{
			btnArgs.Add(buttonText.c_str());
		}
		RE::FxDelegate::Invoke(menu->uiMovie.get(), "setButtons", btnArgs);
	}

	void LevelUpCallbak(uint8_t button)
	{
		std::vector<std::pair<std::string, RE::TESGlobal*>> options = PrepareLevelUpOptions();

		bool allStatsMaxed =
			std::all_of(options.begin(), options.end(), [](const auto& options) { return options.second->value >= 12; });

		if (allStatsMaxed) 
		{
			return;
		}

		switch (button) 
		{
		case 0:
			options[0].second->value += 1;
			break;
		case 1:
			options[1].second->value += 1;
			break;
		case 2:
			options[2].second->value += 1;
			break;
		case 3:
			options[3].second->value += 1;
			break;
		case 4:
			options[4].second->value += 1;
			break;
		case 5:
			options[5].second->value += 1;
			break;
		}

		UpdateLevelUpMessageBoxesInQueue();
	}

	void LevelUpMenu::ForceLevelUp()
	{
		std::vector<std::pair<std::string, RE::TESGlobal*>> options = PrepareLevelUpOptions();

		std::vector<std::string> filteredOptions;
		for (const auto& option : options) 
		{
			filteredOptions.push_back(option.first);
		}

		ShowLevelUpOptions(filteredOptions);
		RE::PlayerCharacter::GetSingleton()->skills->AdvanceLevel(false);  // don't remove this!
	}
	void LevelUpMenu::LevelUpMenuHook()
	{
		_noVanillaLevelUpMenu = SKSE::GetTrampoline().write_call<5>(REL::ID(51638).address() + 0xf8e,
			&LevelUpMenu::ForceLevelUp);  // SkyrimSE.exe+8C02EE
	}
}
