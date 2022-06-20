#include "menu.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::map<const char*, void*> textures;
void AddImage(const char* unique_id, const stbi_uc* buffer, size_t buffSize, ImVec2 size, ImVec2 pos)
{
	if (auto savedTexture = textures.find(unique_id); savedTexture != textures.end())
	{
		ImGui::GetCurrentWindow()->DrawList->AddImage(savedTexture->second, ImGui::GetWindowPos() + pos, ImGui::GetWindowPos() + pos + size);
		return;
	}

	void* pTexture = nullptr;
	std::int32_t width, height;
	stbi_set_flip_vertically_on_load_thread(false);
	const auto data = stbi_load_from_memory(buffer, buffSize, &width, &height, nullptr, STBI_rgb_alpha);
	if (data)
	{
		pTexture = ImGui_CreateTextureRGBA(width, height, data);
		stbi_image_free(data);
		ImGui::GetCurrentWindow()->DrawList->AddImage(pTexture, ImGui::GetWindowPos() + pos, ImGui::GetWindowPos() + pos + size);
		textures.insert({ unique_id, pTexture });
	}
}

void AddImage(const char* unique_id, const char* fileName, ImVec2 size)
{
	if (auto savedTexture = textures.find(unique_id); savedTexture != textures.end())
	{
		ImGui::Image(savedTexture->second, size);
		return;
	}

	void* pTexture = nullptr;
	std::int32_t width, height;
	stbi_set_flip_vertically_on_load_thread(false);
	const auto data = stbi_load(fileName, &width, &height, nullptr, STBI_rgb_alpha);
	if (data)
	{
		pTexture = ImGui_CreateTextureRGBA(width, height, data);
		stbi_image_free(data);

		ImGui::Image(pTexture, size);
		textures.insert({ unique_id, pTexture });
	}
}

bool Button(const char* name, ImVec2 size, float alpha = 1.f)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiID id = window->GetID(name);
	const ImVec2 label_size = ImGui::CalcTextSize(name);
	const ImVec2 pos = window->DC.CursorPos;
	const ImRect bb = ImRect(pos, pos + size);

	ImGui::ItemSize(bb);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

	static std::unordered_map<ImGuiID, float> arrValuesAlpha;
	ImAnim::Animator borderAnimator(&arrValuesAlpha, id, 2.f, 4.0f, 20.f, ImGui::GetIO().DeltaTime);

	if (hovered)
		borderAnimator.Forward();
	else 
		borderAnimator.Reverse();

	ImGui::RenderFrame(bb.Min, bb.Max, ImColor(239.f / 255.f, 73.f / 255.f, 88.f / 255.f, alpha), false);
	window->DrawList->AddText(bb.Min + ((size / 2) - label_size * 0.5f), ImColor(1.f, 1.f, 1.f, alpha), name);
	window->DrawList->AddRect(bb.Min - 
		ImVec2(
			borderAnimator.GetValue(),
			borderAnimator.GetValue()),
		bb.Max + ImVec2(
			borderAnimator.GetValue(), 
			borderAnimator.GetValue()), 
		ImColor(134.f / 255.f, 139.f / 255.f, 149.f / 255.f, alpha));

	return pressed;
}

void Nav()
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGui::SetCursorPos(ImVec2(0, 0));
	ImGui::BeginChild("##navbar", ImVec2(window->Size.x, 30));
	window->DrawList->AddLine(window->Pos + ImVec2(0, 30), window->Pos + ImVec2(window->Size.x, 30), ImColor(143, 145, 159, 255));

	ImGui::EndChild();
}

void Footer()
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	const int height = 95;
	ImGui::SetCursorPos(ImVec2(0, window->Size.y - height));
	ImGui::BeginChild("##footer", ImVec2(window->Size.x, height));
	{
		window->DrawList->AddLine(window->Pos + ImVec2(0, window->Size.y - height), window->Pos + ImVec2(window->Size.x, window->Size.y - height), ImColor(143, 145, 159, 255));
	
		// Profile Card
		ImGui::SetCursorPos(ImVec2(10, (height / 2) - 28));
		AddImage(Loadout::Identity.PlayerCardID.c_str(), fmt::format("{0}\\resources\\{1}.png", std::filesystem::current_path().u8string(), Loadout::Identity.PlayerCardID).c_str(), ImVec2(60, 60));

		// Username
		window->DrawList->AddText(window->Pos + ImVec2(80, (window->Size.y - height + 28)), ImColor(255, 255, 255, 255), Player::username.c_str());
		window->DrawList->AddText(window->Pos + ImVec2(80, (window->Size.y - height + 53)), ImColor(255, 255, 255, 255), fmt::format("Level {}", Progression::progress.Level).c_str());

		ImGui::SetCursorPos(ImVec2(window->Size.x - 160, (height / 2) - 25));
		Button("LAUNCH", ImVec2(140, 50));
	}
	ImGui::EndChild();
}

static std::int32_t step = 1;
bool Menu::Step(const char* title, const char* description, const char* buttonName)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiID id = window->GetID(title);

	// Animator
	static std::unordered_map<ImGuiID, float> arrTitleAnimator;
	ImAnim::Animator titleAnimator(&arrTitleAnimator, id, 0.f, 1.f, 0.8f, ImGui::GetIO().DeltaTime);
	titleAnimator.Forward();

	static std::unordered_map<ImGuiID, float> arrDescAnimator;
	ImAnim::Animator descAnimator(&arrDescAnimator, id, 0.f, 1.f, 0.8f, ImGui::GetIO().DeltaTime);

	window->DrawList->AddText(pHeaderFont, 60.f, window->Pos + ImVec2(70, 70), ImColor(239.f / 255.f, 73.f / 255.f, 88.f / 255.f, titleAnimator.GetValue()), title);

	if (titleAnimator.GetValue() == 1.f)
	{
		descAnimator.Forward();
		window->DrawList->AddText(window->Pos + ImVec2(70, 140), ImColor(92.f / 255.f, 156.f / 255.f, 184.f / 255.f, descAnimator.GetValue()), description);
	}

	ImGui::SetCursorPos(ImGui::GetWindowSize() - ImVec2(155, 60));
	return Button(buttonName, ImVec2(140, 45), descAnimator.GetValue());
}

static bool firstTime = true;

void Menu::Render() 
{
	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("nValorant", nullptr, flags);
	{
		// The users first time!
		if (firstTime) 
		{
			
			switch (step) 
			{

			// Welcome screen!
			case 1:
				if (Step("Welcome!", "It seems like it is your first time using this application.\nLet's start by connecting to your account...", "Next"))
					step++;
				break;

			// Connection part!
			case 2:
				if (Step("Connection!", "Before we connect make sure you have Valorant launched as we do automatic authentication\nand for that we need Valorant launched!\n\nWe do support offline mode but you need to run ONCE to calibrate all the settings correctly", "Connect"))
				{
					Globals::Api.Connect();
					if (Globals::Api.isSuccessful)
					{
						Player::GetData(&Globals::Api);
						Session::GetData(&Globals::Api);
						Loadout::GetData(&Globals::Api);
						Progression::GetData(&Globals::Api);
						if (!std::filesystem::exists(fmt::format("resources\\{}.png", Loadout::Identity.PlayerCardID)))
							Globals::Cards.Download(Loadout::Identity.PlayerCardID.c_str());

						firstTime = false;
					}
					else { step = 10; }
				}
				break;

			// Error: if connection wasn't established between the app and the valorant api
			case 10:
				if (Step("Error!", "Oops, seems like we couldn't connect\nMake sure Valorant is open!", "Try again!"))
				{
					Globals::Api.Connect();
					if (Globals::Api.isSuccessful)
					{
						Player::GetData(&Globals::Api);
						Session::GetData(&Globals::Api);
						Loadout::GetData(&Globals::Api);
						Progression::GetData(&Globals::Api);

						firstTime = false;
					}
				}
				break;
			}
			//	AddImage("##logo", logo_compressed_size, IM_ARRAYSIZE(logo_compressed_size), ImVec2(50, 50), ImVec2(300, 300));
		}
		else 
		{
			Nav();
			{
				ImGuiWindow* window = ImGui::GetCurrentWindow();

				int nextRow = 0;
				int nextColumn = 0;
				for (auto& agent : Globals::Agents.Agents)
				{
					bool sameline = false;
					float max = 20 + (nextColumn * 70);
					if (max > window->Size.x)
					{
						nextRow++;
						nextColumn = 0;
					}
					else
						sameline = true;

					ImGui::SetCursorPos(ImVec2(20 + (nextColumn * 70), 60 + (65 * nextRow)));
					AddImage(agent.uuid.c_str(), fmt::format("C:\\Users\\cow\\source\\repos\\nValorant\\x64\\Release\\resources\\{0}.png", agent.uuid.c_str()).c_str(), ImVec2(60, 60));
					
					if (sameline)
						ImGui::SameLine();

					nextColumn++;
				}

			}
			Footer();
		}
	}
	ImGui::End();
}
