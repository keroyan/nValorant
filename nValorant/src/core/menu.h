#pragma once

#include <imgui/imgui.h>
#include <imgui/backend/imgui_impl_dx9.h>
#include <imgui/backend/imgui_impl_win32.h>
#include <imgui/imgui_internal.h>
#include <cpr/cpr.h>
#include "ImAnim.h"

#include <api.h>
#include <player/player.h>
#include <store/wallet.h>

// Fonts
#include "fonts/bold.h"

// Images
#include "images/logo.h"

class Menu 
{
public:
	Menu() 
	{
		ImGuiStyle& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_WindowBg] = ImColor(15, 25, 35, 255);
		style.Colors[ImGuiCol_ChildBg] = ImColor(21, 35, 49, 255);

		style.WindowPadding = ImVec2(0, 0);

		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromMemoryCompressedTTF(PrimaryFont_compressed_data, PrimaryFont_compressed_size, 16.f);
		pHeaderFont = io.Fonts->AddFontFromMemoryCompressedTTF(PrimaryFont_compressed_data, PrimaryFont_compressed_size, 60.f);
	}

	void Render();
private:

	bool Step(const char* title, const char* description, const char* buttonName);
	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar;
	ImFont* pHeaderFont;

	// Api!
	CApi api = CApi();
	CPlayer player = CPlayer(&api);
	CWallet wallet = CWallet(&api);

};