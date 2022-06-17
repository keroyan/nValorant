#include "menu.h"

void Menu::Render() 
{
	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("nValorant", nullptr, flags);
	{
		// This part will be done after the api is finished!
	}
	ImGui::End();
}