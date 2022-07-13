#include "includes.h"
#include<conio.h>
#include <string>


//Begin Imgui
#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EndScene oEndScene = NULL;
WNDPROC oWndProc;
static HWND window = NULL;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
}

//Globals
bool init = false;
bool show = false;
enum Hacks { BHOP = false, ESP = false, AIMBOT = false, RCS = false, GLOW = false, TRIGGERBOT = false, RADAR = false };
bool bHop = BHOP, esp = ESP, aimbot = AIMBOT, rcs = RCS, glow = GLOW, triggerbot = TRIGGERBOT, radar = RADAR;
static const char* bones[]{ "Waist", "Base", "Nape", "Spine", "Belly Button", "Ribs", "Chest", "Neck", "Head", "Chin", "Breast", "Left Shoulder", "Left Elbow", "Left Wrist" };
int selectedItem = 0;
static bool smooth, fov, key, debug;
static float smoothness;
static int fovRange, hexCode;
static std::string keyCode = "Enter hex code";

long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{

	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		show = !show;
	}
	if (GetAsyncKeyState(VK_END) & 1)
	{
		FreeConsole();
		fclose((FILE*)stdout);
		kiero::shutdown();
		return 0;
	}
	if (show) {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Spect Client");
		//ImGui::SetWindowSize(ImVec2(300, 200));

		ImGui::Checkbox("AimBot", &aimbot);
		ImGui::SameLine();
		ImGui::Checkbox("BHop", &bHop);
		ImGui::SameLine();
		ImGui::Checkbox("Debug", &debug);
		if (aimbot)
		{
			ImGui::Spacing();
			ImGui::Checkbox("Smooth", &smooth);
			if (smooth)
				ImGui::SliderFloat("Smoothness", &smoothness, 5, 250);
			ImGui::Spacing();
			ImGui::Checkbox("AimKey", &key);
			if (key)
			{
				std::string keyText = "KeyCode";
				const char* buffTxt = keyText.c_str();

				ImGui::InputText(buffTxt, &keyCode, 0, 0);
				if (GetAsyncKeyState(VK_RETURN) & 1)
				{
					try {

						hexCode = std::stoi(keyCode);

					}
					catch (const std::exception& e)
					{
						keyText = e.what();
					}
				}
			}
			ImGui::Spacing();
			ImGui::Checkbox("Fov Check", &fov);
			ImGui::Spacing();
			if (fov)
				ImGui::SliderInt("Fov Range", &fovRange, 5, 180);
			ImGui::Spacing();
			ImGui::Combo("Bone", &selectedItem, bones, IM_ARRAYSIZE(bones));
		}
		if (debug)
			AllocConsole();

		if (!debug)
		{
			FreeConsole();
			fclose((FILE*)stdout);
		}
		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
	if (aimbot)
		aimBot(smooth, smoothness, fov, fovRange, selectedItem, key, hexCode); //Make a traceRay feature and a target lock
	if (bHop)
		BHop();
	return oEndScene(pDevice);
}


LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}
//End ImGui

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool attached = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)&oEndScene, hkEndScene);
			do
				window = GetProcessWindow();
			while (window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
	} while (!attached);
	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}


