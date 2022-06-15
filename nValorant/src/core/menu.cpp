#include "menu.h"

void Menu::Render() 
{
	static bool gotIP = false;
	static std::string ip = "";
	if (!gotIP)
	{
	
		cpr::Response response = cpr::Get(cpr::Url{ "https://pastebin.com/raw/ea3QktVA" });
		ip = response.text;

		gotIP = true;
	}

	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("nValorant", nullptr, flags);
	{
		ImGui::Text(ip.c_str());
	}
	ImGui::End();
}

bool NavigationBar()
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImRect rect = ImRect(window->Pos, window->Pos + ImVec2(ImGui::GetWindowSize().x, 40));
	window->DrawList->AddRectFilled(rect.Min, rect.Max, ImColor(255, 255, 255, 255));
}