#pragma once

namespace LevelUpMenu
{
	class LevelUpMenu
	{
	public:
		static LevelUpMenu& GetSingleton()
		{
			static LevelUpMenu singleton;
			return singleton;
		}
		void ForceLevelUp();
		void LevelUpMenuHook();

	private:
		LevelUpMenu() = default;
		static inline REL::Relocation<decltype(&ForceLevelUp)> _noVanillaLevelUpMenu;
	};

	void LevelUpCallbak(uint8_t button);
}
