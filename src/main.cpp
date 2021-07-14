#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#define OLC_PGE_APPLICATION
#include "3rd_party/olc/olcPixelGameEngine.h"
#define OLC_PGEX_SOUND
#include "3rd_party/olc/olcPGEX_Sound.h"

#define OLC_PGEX_TTF
#include "3rd_party/olc/olcPGEX_TTF.h"

#include "3rd_party/imgui/imgui.h"
#define OLC_PGEX_DEAR_IMGUI_IMPLEMENTATION
#include "3rd_party/imgui/imgui_impl_pge.h"
#include "main.h"
#include "scene/game.h"
#include "scene/menu.h"
#include "resource_manager.h"
#include "utility.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const std::string DEFAULT_TITLE = "GREAT MACHINE ARMADA";

class GreatMachineArmada : public olc::PixelGameEngine
{
private:
	int GameLayer;
	Scene* currentScene = nullptr;
	olc::imgui::PGE_ImGUI pge_imgui;
	int state = STATE_MENU;
#ifdef __EMSCRIPTEN__
	Uint64 current = 0;
	Uint64 previous = 0;
	double ms = 0.0;
#endif

public:
	GreatMachineArmada() : pge_imgui(false)
	{
		sAppName = DEFAULT_TITLE;
		srand((unsigned int)time(nullptr));
	}

	~GreatMachineArmada()
	{
	}

	bool OnUserCreate() override
	{
		//Initialization flag
		bool success = true;

		//Create a new Layer which will be used for the game
		GameLayer = CreateLayer();
		//The layer is not enabled by default,  so we need to enable it
		EnableLayer(GameLayer, true);

		state = STATE_MENU; // STATE_MENU;
		currentScene = nullptr;
		NextScene();		
		olc::SOUND::InitialiseAudio();		
		
		olc::Font::init();
		olc::Font* font = ResourceManager::Instance()->GetFont(PATH_FONT, FONT_SIZE);
		

		//Set a custom render function on layer 0.  Since DrawUI is a member of
		//our class, we need to use std::bind
		//If the pge_imgui was constructed with _register_handler = true, this line is not needed
		SetLayerCustomRenderFunction(0, std::bind(&GreatMachineArmada::DrawUI, this));

		// Dear ImGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		//(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

#ifdef __EMSCRIPTEN__
		io.IniFilename = nullptr;
#endif

		io.KeyMap[ImGuiKey_Tab] = olc::Key::S;
		io.KeyMap[ImGuiKey_LeftArrow] = olc::Key::LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = olc::Key::RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = olc::Key::UP;
		io.KeyMap[ImGuiKey_DownArrow] = olc::Key::DOWN;
		io.KeyMap[ImGuiKey_PageUp] = olc::Key::PGUP;
		io.KeyMap[ImGuiKey_PageDown] = olc::Key::PGDN;
		io.KeyMap[ImGuiKey_Home] = olc::Key::HOME;
		io.KeyMap[ImGuiKey_End] = olc::Key::END;
		io.KeyMap[ImGuiKey_Insert] = olc::Key::INS;
		io.KeyMap[ImGuiKey_Delete] = olc::Key::DEL;
		io.KeyMap[ImGuiKey_Backspace] = olc::Key::BACK;
		io.KeyMap[ImGuiKey_Space] = olc::Key::SPACE;
		io.KeyMap[ImGuiKey_Enter] = olc::Key::ENTER;
		io.KeyMap[ImGuiKey_Escape] = olc::Key::ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = olc::Key::RETURN;
		io.KeyMap[ImGuiKey_A] = olc::Key::S;                 // for text edit CTRL+A: select all
		io.KeyMap[ImGuiKey_C] = olc::Key::C;                 // for text edit CTRL+C: copy
		io.KeyMap[ImGuiKey_V] = olc::Key::V;                 // for text edit CTRL+V: paste
		io.KeyMap[ImGuiKey_X] = olc::Key::X;                 // for text edit CTRL+X: cut
		io.KeyMap[ImGuiKey_Y] = olc::Key::Y;                 // for text edit CTRL+Y: redo
		io.KeyMap[ImGuiKey_Z] = olc::Key::Z;                 // for text edit CTRL+Z: undo


		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

		//ImGui::StyleColorsClassic();

		return success;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if (currentScene != nullptr)
		{
			//Change the Draw Target to not be Layer 0
			SetDrawTarget((uint8_t)GameLayer);
			//Game Drawing code here
			currentScene->update(fElapsedTime * 1000.0, &this->pge_imgui);
			//DrawUI();
			//Create and react to your UI here, it will be drawn during the layer draw function
			// ImGui::ShowDemoWindow();

			if (!currentScene->running()) {
				state = currentScene->getNextState();
				if (state == STATE_EXIT)
					currentScene = nullptr;
				else
					NextScene();
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool OnUserDestroy() override
	{
		pge_imgui.ImGui_ImplPGE_Shutdown();

		olc::SOUND::StopAll();
		olc::SOUND::DestroyAudio();
		return true;
	}

	void DrawUI(void) {
		//This finishes the Dear ImGui and renders it to the screen
		pge_imgui.ImGui_ImplPGE_Render();
	}


	void close()
	{
		if (currentScene != nullptr)
			currentScene->destroy();

		ResourceManager::Instance()->Clear();


#ifdef __EMSCRIPTEN__
		// std::cout << "Shutting down" << std::endl;
#else
		//Destroy window	
		pge_imgui.ImGui_ImplPGE_Shutdown();
		ImGui::DestroyContext();
#endif

	}

	void NextScene() {
		Scene* nextScene = nullptr;
		if (currentScene != nullptr) {
			currentScene->destroy();
			delete currentScene;
			currentScene = nullptr;
		}

		if (state == STATE_MENU) {
			nextScene = (Scene*) new SceneMenu();
		}
		else if (state == STATE_PLAY) {
			nextScene = (Scene*) new SceneGame();
		}
		if (nextScene != nullptr) {
			nextScene->init(this);
			if (!nextScene->running())
			{
				std::cout << "Failed to load!" << std::endl;
				state = STATE_EXIT;
				delete nextScene;
				currentScene = nullptr;
			}
			else
			{
				currentScene = nextScene;
			}
		}
		else
		{
			currentScene = nextScene;
			state = STATE_EXIT;
		}
	}
};

int main()
{
	GreatMachineArmada game;
	if (game.Construct(SCREEN_WIDTH, SCREEN_HEIGHT, 1, 1))
		game.Start();
	return 0;
}