#pragma once

#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>

#include "driver/driver.h"
#include "globals.h"
#include <xorstr/xorstr.hpp>

struct Vector3 {
	float x;
	float y;
	float z;
};

class CUtils {
public:
	std::string GetName(uintptr_t address);
	std::vector<uintptr_t> GetChildren(uintptr_t address);
	uintptr_t FindFirstChild(uintptr_t address, std::string name);

	uintptr_t GetDataModel();
	uintptr_t GetWorkspace();
	uintptr_t GetCharacter();
	uintptr_t GetPlayers();
	uintptr_t GetLocalPlayer();
	uintptr_t GetHumanoid();
	int SetWalkSpeed();
	int SetJumpPower();
	int SetHealth();
	int SetGravity();
	float GetMaxHealth();

	Vector3 GetHeadPos(uintptr_t playerInstance);
};

inline auto Utils = std::make_unique<CUtils>();