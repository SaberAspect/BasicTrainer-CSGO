#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_stdlib.h"
#include <iostream>
#include <string>
#include "Headers/Offsets.h"
#include "Menu/Menu.h"
#include "Hacks/Headers/Aimbot.h"
#include "Hacks/Headers/Bunnyhop.h"
#include "Hacks/Headers/Rcs.h"
#include "Hacks/Headers/Glow.h"
#include "Hacks/Headers/Triggerbot.h"
#include "Hacks/Headers/Radar.h"
#define WINDOW_NAME "Dear ImGui DirectX9 Example"
typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);