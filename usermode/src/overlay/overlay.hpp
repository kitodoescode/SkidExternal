#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include <Windows.h>
#include <iostream>
#include <thread>
#include <dwmapi.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "winmm.lib")

#include "misc/globals.h"
#include "misc/utils.h"
#include "xorstr/xorstr.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct Screen
{
	int X = 0, Y = 0;
	int Width = 0, Height = 0;

	bool operator==(const Screen& Screen)
	{
		return X == Screen.X && Y == Screen.Y && Width == Screen.Width && Height == Screen.Height;
	}

	bool operator!=(const Screen& Screen)
	{
		return !(*this == Screen);
	}
} CScreen;

class COverlay
{
private:
	void DrawFOV();
	void DrawESP();

	void DrawBoxESP(const Vector3& headPos, float height, ImU32 color);

	// World-to-screen projection helper
	bool WorldToScreen(const Vector3& worldPos, ImVec2& screenPos);

	// Camera & screen data, updated each frame
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX projectionMatrix;

	int screenWidth;
	int screenHeight;

	HWND hWnd;
public:
	ID3D11Device* D3D11Device = nullptr;
	ID3D11DeviceContext* D3D11DeviceContext = nullptr;
	IDXGISwapChain* DxgiSwapChain = nullptr;
	ID3D11RenderTargetView* D3D11RenderTargetView = nullptr;
public:
	bool CreateD3DDevice(HWND handle);
	void CleanupD3DDevice();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	void InitializeTheme();
public:
	bool Initialize();
};

inline auto Overlay = std::make_unique<COverlay>();