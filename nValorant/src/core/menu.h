#pragma once

#include <imgui/imgui.h>
#include <imgui/backend/imgui_impl_dx9.h>
#include <imgui/backend/imgui_impl_win32.h>
#include <imgui/imgui_internal.h>

#include <cpr/cpr.h>

class Menu 
{
public:
	Menu() 
	{
		
	}

	void Render();
private:
	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar;
};