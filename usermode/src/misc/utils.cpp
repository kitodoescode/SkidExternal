#include "utils.h"

std::string CUtils::GetName(uintptr_t address) {
	/*
	inline constexpr uintptr_t Name = 0x78;
	*/
	uintptr_t namePtr = read<uintptr_t>(address + 0x78);
	std::string name;

	if (read<int>(namePtr + 0x10) >= 16) {
		uintptr_t namePtr2 = read<uintptr_t>(namePtr);
		while (read<char>(namePtr2) != '\0') {
			name += read<char>(namePtr2);
			namePtr2++;
		}
	}
	else {
		name = read<std::string>(namePtr);
	}

	return name;
}

std::vector<uintptr_t> CUtils::GetChildren(uintptr_t address) {
	/*
	inline constexpr uintptr_t Children = 0x80;
    inline constexpr uintptr_t ChildrenEnd = 0x8;
	*/
	std::vector<uintptr_t> children;

	uintptr_t childrenPtr = read<uintptr_t>(address + 0x80);
	uintptr_t childrenEnd = read<uintptr_t>(childrenPtr + 0x8);

	for (auto i = read<uintptr_t>(childrenPtr); i < childrenEnd; i += 0x10) {
		children.push_back(read<uintptr_t>(i));
	}

	return children;
}

uintptr_t CUtils::FindFirstChild(uintptr_t address, std::string name) {
	for (auto child : GetChildren(address)) {
		if (GetName(child) == name) {
			return child;
		}
	}

	return 0;
}



uintptr_t CUtils::GetDataModel() {
	// fakedm + fakedmtodm = datamodel

	/*
	inline constexpr uintptr_t FakeDataModelPointer = 0x67633D8; // only offset that needs update since most others dont update
	inline constexpr uintptr_t FakeDataModelToDataModel = 0x1B8;
	*/

	uintptr_t baseaddr = Globals->BaseAddr;
	uintptr_t FakeDataModel = read<uintptr_t>(baseaddr + 0x67633D8);
	uintptr_t DataModel = read<uintptr_t>(FakeDataModel + 0x1B8);
	Globals->DataModel = DataModel;
	return DataModel;
}

uintptr_t CUtils::GetWorkspace() {
	// datamodel -> workspace

	uintptr_t datamodel = Globals->DataModel;
	uintptr_t Workspace = FindFirstChild(datamodel, xorstr_("Workspace"));
	Globals->Workspace = Workspace;
	return Workspace;
}

uintptr_t CUtils::GetPlayers() {
	// datamodel -> players

	uintptr_t datamodel = Globals->DataModel;
	uintptr_t Players = FindFirstChild(datamodel, xorstr_("Players"));
	Globals->Players = Players;
	return Players;
}

uintptr_t CUtils::GetLocalPlayer() {
	// datamodel -> players -> localplayer

	/*
	inline constexpr uintptr_t LocalPlayer = 0x128;
	*/

	uintptr_t players = Globals->Players;
	uintptr_t LocalPlayer = read<uintptr_t>(players + 0x128);
	Globals->LocalPlayer = LocalPlayer;
	return LocalPlayer;
}

uintptr_t CUtils::GetCharacter() {
	// method 1: datamodel -> workspace -> localplayer's name (character)
	// method 2: datamodel -> players -> localplayer -> character

	/*
	* method 1
	uintptr_t localplayer = Globals->LocalPlayer;
	uintptr_t workspace = Globals->Workspace;
	std::string playername = GetName(localplayer);
	uintptr_t Character = FindFirstChild(workspace, playername);
	*/
	
	// method 2
	uintptr_t players = Globals->Players;
	uintptr_t localplayer = Globals->LocalPlayer;
	/*
	inline constexpr uintptr_t ModelInstance = 0x330;
	*/
	uintptr_t character = read<uintptr_t>(localplayer + 0x330);
	Globals->Character = character;
	return character;
}

uintptr_t CUtils::GetHumanoid() {
	// method 1: datamodel -> workspace -> localplayer's name (character) -> humanoid
	// method 2: datamodel -> players -> localplayer -> character -> humanoid
	// depends on which character is stored
	uintptr_t character = Globals->Character;
	uintptr_t Humanoid = FindFirstChild(character, xorstr_("Humanoid"));
	Globals->Humanoid = Humanoid;
	return Humanoid;
}

int CUtils::SetWalkSpeed() {
	/*
	inline constexpr uintptr_t WalkSpeed = 0x1D8;
	inline constexpr uintptr_t WalkSpeedCheck = 0x3B0;
	*/
	uintptr_t humanoid = Globals->Humanoid;
	uintptr_t walkspeedptr = humanoid + 0x1D8;
	uintptr_t walkspeedptr2 = humanoid + 0x3B0; // this is the checking ptr, needed to avoid being kicked
	write<float>(walkspeedptr, Globals->WalkSpeed);
	write<float>(walkspeedptr2, Globals->WalkSpeed);
	return 1;
}

int CUtils::SetJumpPower() {
	/*
	* inline constexpr uintptr_t JumpPower = 0x1B8;
	*/
	uintptr_t humanoid = Globals->Humanoid;
	uintptr_t jumpspeedptr = humanoid + 0x1B8;
	write<float>(jumpspeedptr, Globals->JumpPower);
	return 1;
}

int CUtils::SetHealth() {
	/*
	* inline constexpr uintptr_t Health = 0x19C;
	*/
	uintptr_t humanoid = Globals->Humanoid;
	uintptr_t healthptr = humanoid + 0x19C;
	write<float>(healthptr, Globals->Health);
	return 1;
}

int CUtils::SetGravity() {
	/*
	* inline constexpr uintptr_t Gravity = 0x950;
	*/
	uintptr_t workspace = Globals->Workspace;
	uintptr_t workspaceptr = workspace + 0x950;
	if (!workspaceptr)
		return 0;

	write<float>(workspaceptr, Globals->Gravity);
	return 1;
}

float CUtils::GetMaxHealth() {
	/*
	* inline constexpr uintptr_t MaxHealth = 0x1BC;
	*/
	uintptr_t humanoid = Globals->Humanoid;
	uintptr_t maxhealthptr = humanoid + 0x1BC;
	float maxhealth = read<float>(maxhealthptr);
	return maxhealth;
}



Vector3 CUtils::GetHeadPos(uintptr_t playerInstance) {
	/*
	* inline constexpr uintptr_t ModelInstance = 0x330;
	* inline constexpr uintptr_t Primitive = 0x178;
	* inline constexpr uintptr_t Position = 0x140;
	*/
	uintptr_t character = read<uintptr_t>(playerInstance + 0x330);
	uintptr_t head = FindFirstChild(character, xorstr_("Head"));
	uintptr_t primitive = read<uintptr_t>(head + 0x178);
	Vector3 headpos = read<Vector3>(primitive + 0x140);

	return headpos;
}