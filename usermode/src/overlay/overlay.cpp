#include "overlay.hpp"

bool COverlay::CreateD3DDevice(HWND handle)
{
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	RtlZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));

	SwapChainDesc.BufferCount = 2;
	SwapChainDesc.BufferDesc.Width = 0;
	SwapChainDesc.BufferDesc.Height = 0;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = handle;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const UINT CreateDeviceFlags = 0;
	D3D_FEATURE_LEVEL D3DFeatureLevel;
	const D3D_FEATURE_LEVEL FeatureLevelArr[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	HRESULT Result = D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE, nullptr, CreateDeviceFlags, FeatureLevelArr,
		2, D3D11_SDK_VERSION, &SwapChainDesc, &this->DxgiSwapChain, &this->D3D11Device,
		&D3DFeatureLevel, &this->D3D11DeviceContext);

	if (Result == DXGI_ERROR_UNSUPPORTED)
	{
		Result = D3D11CreateDeviceAndSwapChain(nullptr,
			D3D_DRIVER_TYPE_WARP, nullptr, CreateDeviceFlags, FeatureLevelArr,
			2, D3D11_SDK_VERSION, &SwapChainDesc, &this->DxgiSwapChain, &this->D3D11Device,
			&D3DFeatureLevel, &this->D3D11DeviceContext);
	}

	if (Result != S_OK)
		return false;

	this->CreateRenderTarget();
	return true;
}

void COverlay::CleanupD3DDevice()
{
	this->CleanupRenderTarget();

	if (this->DxgiSwapChain)
	{
		this->DxgiSwapChain->Release();
		this->DxgiSwapChain = nullptr;
	}

	if (this->D3D11DeviceContext)
	{
		this->D3D11DeviceContext->Release();
		this->D3D11DeviceContext = nullptr;
	}

	if (this->D3D11Device)
	{
		this->D3D11Device->Release();
		this->D3D11Device = nullptr;
	}
}

void COverlay::CreateRenderTarget()
{
	ID3D11Texture2D* D3D11BackBuffer;
	this->DxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&D3D11BackBuffer));
	if (D3D11BackBuffer != nullptr)
	{
		this->D3D11Device->CreateRenderTargetView(D3D11BackBuffer, nullptr, &this->D3D11RenderTargetView);
		D3D11BackBuffer->Release();
	}
}

void COverlay::CleanupRenderTarget()
{
	if (this->D3D11RenderTargetView)
	{
		this->D3D11RenderTargetView->Release();
		this->D3D11RenderTargetView = nullptr;
	}
}

__forceinline LRESULT __stdcall WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (Overlay->D3D11Device != nullptr && wParam != SIZE_MINIMIZED)
		{
			Overlay->CleanupRenderTarget();
			Overlay->DxgiSwapChain->ResizeBuffers(2, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			Overlay->CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;
	}
	return DefWindowProcA(hWnd, msg, wParam, lParam);
}

void COverlay::InitializeTheme() {
	// windark style - DestroyerDarkNess - ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 8.399999618530273f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 3.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 3.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 3.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 5.599999904632568f;
	style.ScrollbarRounding = 18.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 3.0f;
	style.TabRounding = 3.0f;
	style.TabBorderSize = 0.0f;
	//style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.125490203499794f, 0.125490203499794f, 0.125490203499794f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3490196168422699f, 0.3490196168422699f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.3294117748737335f, 0.6000000238418579f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.3019607961177826f, 0.3019607961177826f, 0.3019607961177826f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.250980406999588f, 0.250980406999588f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.168627455830574f, 0.168627455830574f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2156862765550613f, 0.2156862765550613f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.0f, 0.3294117748737335f, 0.6000000238418579f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.0f, 0.3294117748737335f, 0.6000000238418579f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0f, 0.4705882370471954f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}

bool COverlay::WorldToScreen(const Vector3& worldPos, ImVec2& screenPos) {
	// Load the 3D point into an XMVECTOR
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(worldPos.x, worldPos.y, worldPos.z, 1.0f);

	// Transform by view and projection matrices
	DirectX::XMVECTOR viewPos = DirectX::XMVector4Transform(pos, viewMatrix);
	DirectX::XMVECTOR projPos = DirectX::XMVector4Transform(viewPos, projectionMatrix);

	// Get the W component to cull behind‑camera points
	float w = DirectX::XMVectorGetW(projPos);
	if (w < 0.1f)
		return false;

	// Perform the perspective divide
	DirectX::XMVECTOR ndc = DirectX::XMVectorScale(projPos, 1.0f / w);

	// Extract normalized device coords
	float ndcX = DirectX::XMVectorGetX(ndc);
	float ndcY = DirectX::XMVectorGetY(ndc);

	// Map to screen space
	screenPos.x = (screenWidth * 0.5f) * (ndcX + 1.0f);
	screenPos.y = (screenHeight * 0.5f) * (1.0f - ndcY);  // flip Y for top‑down

	return true;
}

void COverlay::DrawFOV() {
	// thx to @c4rguy. for helping here

	POINT cursorPos;
	if (!GetCursorPos(&cursorPos))
		return;

	if (!ScreenToClient(this->hWnd, &cursorPos))
		return;

	ImVec2 screenPos = ImVec2(static_cast<float>(cursorPos.x), static_cast<float>(cursorPos.y));

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	ImColor fovColor(0xC2, 0x52, 0xFF, 255);

	drawList->AddCircle(screenPos, 100.f, fovColor, 100, 1.5f);
}

void COverlay::DrawESP() {
	uintptr_t PlayersService = Utils->GetPlayers();

	auto Players = Utils->GetChildren(PlayersService);

	for (uintptr_t player : Players) {
		if (player == Utils->GetLocalPlayer())
			continue;

		Vector3 headPos = Utils->GetHeadPos(player);
		std::cout << "[+] DrawESP::headPos -> " << headPos.x << headPos.y << headPos.z << "\n";
		DrawBoxESP(headPos, 5.0f, IM_COL32(0, 255, 0, 255));
	}
}

void COverlay::DrawBoxESP(
	const Vector3& headPos,
	float height,
	ImU32 color
) {
	ImVec2 top, bottom;

	if (!WorldToScreen(headPos, top))
		return;

	Vector3 footPos = headPos;
	footPos.y -= height;

	if (!WorldToScreen(footPos, bottom))
		return;

	float boxHeight = bottom.y - top.y;
	float boxWidth = boxHeight / 2.0f;

	ImVec2 topLeft = ImVec2(top.x - boxWidth / 2, top.y);
	ImVec2 bottomRight = ImVec2(top.x + boxWidth / 2, bottom.y);

	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	drawList->AddRect(topLeft, bottomRight, color, 2.0f);
}

bool COverlay::Initialize()
{
	ImGui_ImplWin32_EnableDpiAwareness();

	WNDCLASSEX WindowClass;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.cbWndExtra = 0;
	WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(CreateSolidBrush(RGB(0, 0, 0)));
	WindowClass.hCursor = LoadCursorA(nullptr, IDC_ARROW);
	WindowClass.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
	WindowClass.hIconSm = LoadIconA(nullptr, IDI_APPLICATION);
	WindowClass.hInstance = GetModuleHandleA(nullptr);
	WindowClass.lpfnWndProc = WindowProc;
	WindowClass.lpszClassName = "gubgub";
	WindowClass.lpszMenuName = nullptr;
	WindowClass.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassExA(&WindowClass);
	const auto Window = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE, WindowClass.lpszClassName, "Shitty External",
		WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, WindowClass.hInstance, nullptr);

	hWnd = Window;

	SetLayeredWindowAttributes(Window, 0, 255, LWA_ALPHA);

	const MARGINS Margin = { -1 };
	DwmExtendFrameIntoClientArea(Window, &Margin);

	if (!this->CreateD3DDevice(Window))
	{
		this->CleanupD3DDevice();
		UnregisterClassA(WindowClass.lpszClassName, WindowClass.hInstance);
		return false;
	}

	ShowWindow(Window, SW_SHOW);
	UpdateWindow(Window);

	ImGui::CreateContext();
	ImGui::GetIO().IniFilename = nullptr;

	InitializeTheme();

	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX11_Init(this->D3D11Device, this->D3D11DeviceContext);

	const ImVec4 ClearColor = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);

	bool Init = true, Show = true, Done = false;
	while (!Done)
	{
		MSG Message;
		while (PeekMessageA(&Message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessageA(&Message);
			if (Message.message == WM_QUIT)
				Done = true;
		}

		if (Done)
			break;

		const auto TargetWindow = FindWindow(nullptr, xorstr_("Roblox"));
		const auto ForegroundWindow = GetForegroundWindow();

		if (TargetWindow != ForegroundWindow && Window != ForegroundWindow)
			MoveWindow(Window, 0, 0, 0, 0, true);
		else
		{
			RECT Rect;
			GetWindowRect(TargetWindow, &Rect);

			auto RSizeX = Rect.right - Rect.left;
			auto RSizeY = Rect.bottom - Rect.top;

			auto Status = false;
			MONITORINFO MonitorInfo = { sizeof(MONITORINFO) };
			if (GetMonitorInfoA(MonitorFromWindow(TargetWindow, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
			{
				RECT WindowRect;
				if (GetWindowRect(TargetWindow, &WindowRect))
				{
					Status = WindowRect.left == MonitorInfo.rcMonitor.left
						&& WindowRect.right == MonitorInfo.rcMonitor.right
						&& WindowRect.top == MonitorInfo.rcMonitor.top
						&& WindowRect.bottom == MonitorInfo.rcMonitor.bottom;
				}
			}

			if (Status)
			{
				RSizeX += 16;
				RSizeY -= 24;
			}
			else
			{
				RSizeY -= 63;
				Rect.left += 8;
				Rect.top += 31;
			}
			MoveWindow(Window, Rect.left, Rect.top, RSizeX, RSizeY, 1);
		}

		if ((GetAsyncKeyState(VK_RSHIFT) & 1))
			Show = !Show;

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			{
				// here stuff like esp, fov etc
				
				// old flags:
				// ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground;
				
				ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
					ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground |
					ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;

				ImGuiIO& io = ImGui::GetIO();
				ImVec2 windSize = io.DisplaySize;
				ImGui::SetNextWindowPos(ImVec2(0, 0));
				ImGui::SetNextWindowSize(windSize);

				ImGui::Begin("##Overlay", nullptr, flags);
				DrawFOV();
				DrawESP();
				ImGui::End();
			}

			if (GetForegroundWindow() == FindWindowA(nullptr, xorstr_("Roblox")) || GetForegroundWindow() == Window) {

				if (Show) {
					ImGui::SetNextWindowSize(ImVec2(600, 355), ImGuiCond_FirstUseEver);
					ImGui::Begin("Shitty External - Skid Leaks", nullptr);
					{
						if (ImGui::BeginTabBar(xorstr_("#Tabs"), ImGuiTabBarFlags_None))
						{
							if (ImGui::BeginTabItem(xorstr_("Home"))) {
								ImGui::Text("Welcome to Shitty External!");
								ImGui::EndTabItem();
							}


							if (ImGui::BeginTabItem(xorstr_("Player"))) {
								ImGui::Text(xorstr_("WalkSpeed"));
								ImGui::SameLine();
								ImGui::InputFloat(xorstr_("##WalkSpeed"), &Globals->WalkSpeed, 1, 2, 0);
								ImGui::SameLine();
								if (ImGui::Button(xorstr_("Set##ws"))) {
									Utils->SetWalkSpeed();
								}

								ImGui::Text(xorstr_("JumpPower"));
								ImGui::SameLine();
								ImGui::InputFloat(xorstr_("##JumpPower"), &Globals->JumpPower, 1, 2, 0);
								ImGui::SameLine();
								if (ImGui::Button(xorstr_("Set##jp"))) {
									Utils->SetJumpPower();
								}

								ImGui::Separator();

								if (ImGui::Button(xorstr_("Reset Character"))) {
									float old_health = Globals->Health;
									Globals->Health = 0;
									Utils->SetHealth();
									Globals->Health = old_health;
								}

								ImGui::EndTabItem();
							}

							if (ImGui::BeginTabItem(xorstr_("World"))) {
								ImGui::Text(xorstr_("Gravity"));
								ImGui::SameLine();
								ImGui::InputFloat(xorstr_("##Gravity"), &Globals->Gravity, 1, 2, 0);
								ImGui::SameLine();
								if (ImGui::Button(xorstr_("Set##g"))) {
									Utils->SetGravity();
								}

								ImGui::EndTabItem();
							}

							if (ImGui::BeginTabItem(xorstr_("Settings"))) {
								if (ImGui::Button(xorstr_("Exit")))
									exit(0);

								ImGui::EndTabItem();
							}

							ImGui::EndTabBar();
						}
					}
					ImGui::End();
				}
			}

			if (Show) {
				SetWindowLongA(Window, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
			}
			else {
				SetWindowLongA(Window, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW);
			}

			ImGui::EndFrame();
			ImGui::Render();

			const float ClearColorWithAlpha[4] = { ClearColor.x * ClearColor.w, ClearColor.y * ClearColor.w, ClearColor.z * ClearColor.w, ClearColor.w };
			this->D3D11DeviceContext->OMSetRenderTargets(1, &this->D3D11RenderTargetView, nullptr);
			this->D3D11DeviceContext->ClearRenderTargetView(this->D3D11RenderTargetView, ClearColorWithAlpha);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			this->DxgiSwapChain->Present(1, 0);

			const float targetFrameTime = 1.0f / 60.0f; // 60 FPS
			static DWORD lastTime = timeGetTime();
			DWORD currentTime = timeGetTime();
			float deltaTime = (currentTime - lastTime) / 1000.0f;

			if (deltaTime < targetFrameTime) {
				std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((targetFrameTime - deltaTime) * 1000)));
			}

			lastTime = timeGetTime();
		}
	}

	Init = false;

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	this->CleanupD3DDevice();
	DestroyWindow(Window);
	UnregisterClassA(WindowClass.lpszClassName, WindowClass.hInstance);
}