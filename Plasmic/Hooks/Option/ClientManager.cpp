#include "ClientManager.h"
#include "../Other/Module.h"

std::vector<Hook*> ClientManager::Hooks;
std::vector<Module*> ClientManager::Modules;
std::vector<std::string> ClientManager::Categories;

#include "Hooks/ClientInstance.h"
#include "Hooks/RenderContext.h"
#include "Hooks/GameMode.h"
#include "Hooks/KeyItem.h"

ClientInstance* Minecraft::CachedInstance = nullptr; //Resolve error on compile
GameMode* Minecraft::CachedGameMode = nullptr; //Resolve error on compile

void ClientManager::InitHooks() {

	if (MH_Initialize() == MH_OK) {
		Hooks.push_back(new ClientInstance_Hook());
		Hooks.push_back(new RenderContext());
		Hooks.push_back(new GameMode_Hook());
		Hooks.push_back(new KeyItem());
	}

	Utils::DebugLogOutput("Initializing Hooks...");

	for (int I = 0; I < Hooks.size(); I++) { //Initialize the hooks
		Hooks.at(I)->Install();
	}
}

/* Combat */

/* Movement */

/* Player */

/* Visuals */
#include "Modules/TabGUI.h"
#include "Modules/Keystroke.h"
#include "Modules/fpsCounter.h"
#include "Modules/clockModule.h"
/* World */

/* Other */
#include "Modules/Uninject.h"

void ClientManager::InitModules() {
	/* Combat */

	/* Movement */

	/* Visuals */
	Modules.push_back(new TabGUI());
	Modules.push_back(new Keystroke());
	Modules.push_back(new fpsCounter());
	Modules.push_back(new clockModule());
	/* World */

	/* Other */
	Modules.push_back(new Uninject());

	for (int I = 0; I < Modules.size(); I++) { //Initialize Categories
		bool exists = false;
		for (auto currCategory : Categories) {
			if (currCategory == Modules.at(I)->category) exists = true;
		}
		if (!exists) {
			Categories.push_back(Modules.at(I)->category);
		}
	}

	for (;;) {
		for (int I = 0; I < Modules.size(); I++) { //Loop Modules
			Modules.at(I)->onBaseTick();
		}
	}
}

std::vector<class Module*> ClientManager::GetModulesFromCategory(std::string Category) {
	bool exists = false;
	for (auto Curr : Categories) {
		if (Curr == Category) exists = true;
	}
	if (exists) {
		std::vector<Module*> ModulesArr;
		for (auto Module : Modules) {
			if (Module->category == Category) ModulesArr.push_back(Module);
		}
		return ModulesArr;
	}
	else {
		return std::vector<Module*>();
	}
}