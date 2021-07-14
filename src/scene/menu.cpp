#include <iostream>
#include <algorithm>
#include "menu.h"
#include "../utility.h"

const std::string PATH_BACKGROUND = "assets/image/title.png";

SceneMenu::SceneMenu() {
	this->app = NULL;
	this->quit = false;
	this->nextState = STATE_EXIT;
}

SceneMenu::~SceneMenu() {
	this->destroy();
}

void SceneMenu::loadMedia() {
	this->background = ResourceManager::Instance()->GetImage(PATH_BACKGROUND);
	if (this->background == NULL)
		this->quit = true;
}

void SceneMenu::init(olc::PixelGameEngine* app) {
	this->app = app;
	this->quit = false;
	this->background = NULL;
	this->loadMedia();
	this->nextState = STATE_EXIT;
}

int SceneMenu::getNextState() {
	return this->nextState;
}

void SceneMenu::destroy() {
	// do nothing
}

void SceneMenu::update(double ms, olc::imgui::PGE_ImGUI* gui) {

	/*
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	*/
	//ImGui_ImplSDL2_ProcessEvent(&e);
	//User requests quit
#ifndef __EMSCRIPTEN__
	if (app->GetKey(olc::Key::ESCAPE).bReleased)
	{
		this->nextState = STATE_EXIT;
		this->quit = true;
	}
#endif
	/*
	int32_t wheel = app->GetMouseWheel();

	int mouseX, mouseY;
	olc::vi2d mousePos = this->app->GetWindowMouse();
	olc::HWButton btnLeft =  this->app->GetMouse(olc::Mouse::LEFT);
	olc::HWButton btnRight = this->app->GetMouse(olc::Mouse::RIGHT);

	// Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)

	io.DeltaTime = std::max<float>(FRAME_MIN, (float) ms);
	io.MousePos = ImVec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	io.MouseDown[0] = btnLeft.bPressed;
	io.MouseDown[1] = btnRight.bPressed;
	io.MouseWheel = static_cast<float>(wheel);
	*/
	int w, h, winW, winH;
	w = this->background->width;
	h = this->background->height;
	olc::vi2d windowSize = app->GetWindowSize();
	winW = windowSize.x;
	winH = windowSize.y;

	// ? io.DisplaySize = ImVec2((float)w, (float)h);
	
	//ImGui::NewFrame();
	gui->ImGui_ImplPGE_NewFrame();
	ImGui::SetNextWindowPos(ImVec2((float)(winW / 4.0), (float)(winH / 4.0)), ImGuiCond_::ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2((float)(winW / 2.0), (float)(winH / 2.0)), ImGuiCond_::ImGuiCond_Once);
	ImGui::Begin("The Great Machine Armada");
	// ImGui::Text("OLC Game Jam 2020");
	if (ImGui::Button("Play")) {
		this->nextState = STATE_PLAY;
		this->quit = true;
	}
#ifndef __EMSCRIPTEN__
	if (ImGui::Button("Exit")) {
		this->nextState = STATE_EXIT;
		this->quit = true;
	}
#endif
	ImGui::End();
	//ImGui::ShowDemoWindow();
	ImGui::EndFrame();
	ImGui::Render();
	//gui->ImGui_ImplPGE_Render();

	/*
	SDL_Rect fillR;
	fillR.x = 0;
	fillR.y = 0;
	fillR.w = winW;
	fillR.h = winH;


	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	*/
	app->DrawSprite(0, 0, this->background);

	// Rendering		
	//ImGuiSDL::Render(ImGui::GetDrawData());
}

bool SceneMenu::running() {
	return !this->quit;
}
