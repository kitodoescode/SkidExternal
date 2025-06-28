#pragma once

#include <Windows.h>
#include <iostream>

class CGlobals {
public:
	// cheat values (used by cheat)
	float WalkSpeed;
	float JumpPower;
	float Health;
	float Gravity;
	
	// game values
	uintptr_t BaseAddr;
	uintptr_t DataModel;
	uintptr_t Workspace;
	uintptr_t Players;
	uintptr_t LocalPlayer;
	uintptr_t Character;
	uintptr_t Humanoid;
	float MaxHealth;
};

inline auto Globals = std::make_unique<CGlobals>();