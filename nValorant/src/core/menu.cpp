#include "menu.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::map<const char*, void*> textures;
void AddImage(const char* unique_id, const stbi_uc* buffer, size_t buffSize, ImVec2 imageSize, ImVec2 imagePos)
{
	if (auto savedTexture = textures.find(unique_id); savedTexture != textures.end())
	{
		ImGui::GetCurrentWindow()->DrawList->AddImage(savedTexture->second, ImGui::GetWindowPos() + imagePos, ImGui::GetWindowPos() + imagePos + imageSize);
		return;
	}

	void* pTexture = nullptr;

	int width, height;
	stbi_set_flip_vertically_on_load_thread(false);

	const auto data = stbi_load_from_memory(buffer, buffSize, &width, &height, nullptr, STBI_rgb_alpha);
	if (data)
	{
		pTexture = ImGui_CreateTextureRGBA(width, height, data);
		stbi_image_free(data);
		ImGui::GetCurrentWindow()->DrawList->AddImage(pTexture, ImGui::GetWindowPos() + imagePos, ImGui::GetWindowPos() + imagePos + imageSize);
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

static bool firstTime = false;

void Menu::Render() 
{
	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);
	ImGui::Begin("nValorant", nullptr, flags);
	{
		// The users first time!
		if (firstTime) 
		{
			if (step == 1)
			{
				if (Step("Welcome!", "It seems like it is your first time using this application.\nLet's start by connecting to your account...", "Next"))
					step++;
			}

			if (step == 2) 
			{
				if (Step("Connection!", "Before we connect make sure you have Valorant launched as we do automatic authentication\nand for that we need Valorant launched!\n\nWe do support offline mode but you need to run ONCE to calibrate all the settings correctly", "Connect"))
				{
					api.Connect();
					api.isSuccessful ? firstTime = false : step = 10;
				}
			}

			if (step == 10) 
			{
				if (Step("Error!", "Oops, seems like we couldn't connect\nMake sure Valorant is open!", "Try again!"))
				{
					api.Connect();
					api.isSuccessful ? firstTime = false : step = 10;
				}
			}

			//	AddImage("##logo", logo_compressed_size, IM_ARRAYSIZE(logo_compressed_size), ImVec2(50, 50), ImVec2(300, 300));
		}
		else 
		{
			Nav();
			{

			}
			Footer();
		}

		//Nav();
		//ImGui::Image();
		
		//	ImGui::Text(fmt::format("Welcome to nValorant, {} \n", player.preferred_username).c_str());
		//	ImGui::Text(fmt::format("Radianite: {}", wallet.radianite).c_str());
		//	ImGui::Text(fmt::format("Points: {}", wallet.points).c_str());
	}
	ImGui::End();
}
