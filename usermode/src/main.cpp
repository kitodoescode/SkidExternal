#include <iostream>
#include <thread>
#include <chrono>

#include "driver/driver.h"
#include "misc/utils.h"
#include "overlay/overlay.hpp"

int main_thread() {
	SetConsoleTitleA(xorstr_("Shitty External"));

	if (!mem::find_driver()) {
		system("color 4");
		std::cout << xorstr_("[!] Driver isn't loaded!") << "\n";
		std::cin.get();
		return 0;
	}

	mem::process_id = mem::find_process(xorstr_("RobloxPlayerBeta.exe"));

	if (!mem::process_id || !FindWindow(nullptr, xorstr_("Roblox"))) {
		system("color 4");
		std::cout << "[!] RobloxPlayerBeta.exe not found!\n";
		std::cin.get();
		return 0;
	}

	Globals->BaseAddr = mem::find_image();

	if (!Globals->BaseAddr) {
		system("color 4");
		std::cout << xorstr_("[!] Failed to get base address.") << "\n";
		std::cin.get();
		return 0;
	}

	std::cout << xorstr_("[+] Base Address -> 0x") << std::hex << std::uppercase << Globals->BaseAddr << "\n";

	uintptr_t dataModel = Utils->GetDataModel();

	if (!dataModel) {
		system("color 4");
		std::cout << xorstr_("[!] Failed to get DataModel.") << "\n";
		std::cin.get();
		return 0;
	}

	std::cout << xorstr_("[+] DataModel -> 0x") << std::hex << std::uppercase << dataModel << "\n";

	uintptr_t workspace = Utils->GetWorkspace();

	if (!workspace) {
		system("color 4");
		std::cout << xorstr_("[!] Failed to get Workspace.") << "\n";
		std::cin.get();
		return 0;
	}

	std::cout << xorstr_("[+] Workspace -> 0x") << std::hex << std::uppercase << workspace << "\n";

	uintptr_t players = Utils->GetPlayers();

	if (!players) {
		system("color 4");
		std::cout << xorstr_("[!] Failed to get Players.") << "\n";
		std::cin.get();
		return 0;
	}

	std::cout << xorstr_("[+] Players -> 0x") << std::hex << std::uppercase << players << "\n";

	uintptr_t localPlayer = Utils->GetLocalPlayer();

	if (!localPlayer) {
		system("color 4");
		std::cout << xorstr_("[!] Failed to get LocalPlayer.") << "\n";
		std::cin.get();
		return 0;
	}

	std::cout << xorstr_("[+] LocalPlayer -> 0x") << std::hex << std::uppercase << localPlayer << "\n";

	uintptr_t character = Utils->GetCharacter();

	if (!character) {
		system("color 4");
		std::cout << xorstr_("[!] Failed to get Character.") << "\n";
		std::cin.get();
		return 0;
	}

	std::cout << xorstr_("[+] Character -> 0x") << std::hex << std::uppercase << character << "\n";

	/*auto children = GetChildren(character);
	std::cout << "[Debug] Character children count: " << children.size() << "\n";
	for (auto child : children) {
		std::string name = GetName(child);
		std::cout << "[Debug] Child: " << name << " (0x" << std::hex << child << ")\n";
	}*/

	uintptr_t humanoid = Utils->GetHumanoid();

	if (!humanoid) {
		system("color 4");
		std::cout << xorstr_("[!] Failed to get Humanoid.") << "\n";
		std::cin.get();
		return 0;
	}

	std::cout << xorstr_("[+] Humanoid -> 0x") << std::hex << std::uppercase << humanoid << "\n";

	/*
	
	if (SetWalkSpeed(humanoid, 120.0f) == 1) {
		std::cout << xorstr_("[*] WalkSpeed set to 120.") << "\n";
	}
	else {
		std::cout << xorstr_("[!] Failed to set WalkSpeed.") << "\n";
	}

	if (SetJumpPower(humanoid, 120.0f) == 1) {
		std::cout << xorstr_("[*] JumpPower set to 120.") << "\n";
	}
	else {
		std::cout << xorstr_("[!] Failed to set JumpPower.") << "\n";
	}

	float maxhealth = GetMaxHealth(humanoid);

	if (!maxhealth) {
		system("color 4");
		std::cout << xorstr_("[!] Failed to get Max Health.") << "\n";
		std::cin.get();
		return 0;
	}

	std::cout << xorstr_("[+] Max Health -> ") << maxhealth << "\n";

	if (SetHealth(humanoid, 100.0f) == 1) {
		std::cout << xorstr_("[*] Health set to 100.") << "\n";
	}
	else {
		std::cout << xorstr_("[!] Failed to set Health.") << "\n";
	}
	
	*/

	std::thread(&COverlay::Initialize, Overlay.get()).detach();

	std::cout << xorstr_("[*] External GUI initialized.") << "\n\n";

	std::cout << xorstr_("Press any key to exit . . .");
	std::cin.get();

	return 1;
}

int main() {
	main_thread();
	std::cout << xorstr_("Closing in 5 seconds . . .");
	std::this_thread::sleep_for(std::chrono::seconds(5));
	return 1;
}