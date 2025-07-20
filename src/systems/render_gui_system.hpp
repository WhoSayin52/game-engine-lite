#ifndef RENDER_GUI_SYSTEM_HPP
#define RENDER_GUI_SYSTEM_HPP

#include "../ecs/ecs.hpp"

#include <SDL2/SDL.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>

class RenderGuiSystem : public System {
public:
	RenderGuiSystem() = default;

	void update(SDL_Renderer* renderer) {
		//Dear ImGui setup
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		//Logic
		ImGuiWindowFlags window_flags{
			//ImGuiWindowFlags_NoDecoration | 
			//ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_AlwaysAutoResize
		};

		if (ImGui::Begin("Spawn Enemies", nullptr, window_flags)) {
			ImGui::Text("Here is where we spawn enemies");
		}
		ImGui::End();
		//Logic_End

		//Dear ImGui Render
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
	}
};

#endif //RENDER_GUI_SYSTEM_HPP
