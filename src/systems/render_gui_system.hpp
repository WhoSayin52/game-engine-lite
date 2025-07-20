#ifndef RENDER_GUI_SYSTEM_HPP
#define RENDER_GUI_SYSTEM_HPP

#include "../ecs/ecs.hpp"

#include "../components/animation_component.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/health_component.hpp"
#include "../components/projectile_component.hpp"
#include "../components/projectile_emitter_component.hpp"
#include "../components/rigidbody_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/transform_component.hpp"

#include <SDL2/SDL.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>

class RenderGuiSystem : public System {
public:
	RenderGuiSystem() = default;

	void update(SDL_Renderer* renderer, Registry& registry, SDL_Rect& camera) {
		//Dear ImGui setup
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		//Logic
		if (ImGui::Begin("Spawn Enemies", nullptr)) {
			ImGui::Text("Here is where we spawn enemies");

			static int pos_x{ 0 };
			static int pos_y{ 0 };
			static int scale_x{ 1 };
			static int scale_y{ 1 };
			static int vel_x{ 0 };
			static int vel_y{ 0 };
			static int health{ 100 };
			static float rotation{ 0.0f };
			static float proj_angle{ 0.0f };
			static float proj_speed{ 100.0f };
			static float proj_delay{ 2.0f };
			static float proj_duration{ 5.0f };
			static int proj_dmg{ 10 };
			const char* sprites[]{
				"tank_panther_right",
				"truck_ford_right"
			};
			static int selected_sprite_index{ 0 };


			if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Combo("texture id", &selected_sprite_index, sprites, IM_ARRAYSIZE(sprites));
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::InputInt("pos x", &pos_x);
				ImGui::InputInt("pos y", &pos_y);
				ImGui::SliderInt("scale x", &scale_x, 1, 10);
				ImGui::SliderInt("scale y", &scale_y, 1, 10);
				ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Rigidbody", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::InputInt("vel x", &vel_x);
				ImGui::InputInt("vel y", &vel_y);
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Projectile", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::SliderAngle("angle (deg)", &proj_angle, 0, 360);
				ImGui::SliderFloat("speed (px/sec)", &proj_speed, 10, 500);
				ImGui::InputFloat("delay (sec)", &proj_delay);
				ImGui::InputFloat("duration (sec)", &proj_duration);
			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::InputInt("health", &health);
			}
			ImGui::Spacing();

			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::Button("Create new enemy")) {
				Entity enemy{ registry.create_entity() };
				enemy.add_group("enemies");
				enemy.add_component<TransformComponent>(
					glm::dvec2(pos_x, pos_y),
					glm::dvec2(scale_x, scale_y),
					glm::degrees(rotation)
				);
				enemy.add_component<RigidbodyComponent>(glm::dvec2(vel_x, vel_y));
				enemy.add_component<SpriteComponent>(sprites[selected_sprite_index], 1);
				enemy.add_component<BoxColliderComponent>(25, 20, glm::dvec2(5.0, 5.0));

				double proj_vel_x{ cos(proj_angle) * proj_speed };
				double proj_vel_y{ sin(proj_angle) * proj_speed };

				enemy.add_component<ProjectileEmitterComponent>(
					glm::dvec2{ proj_vel_x, proj_vel_y },
					proj_dmg,
					static_cast<double>(proj_delay),
					static_cast<double>(proj_duration)
				);
				enemy.add_component<HealthComponent>(health);

				pos_x = pos_y = 0;
				scale_x = scale_y = 1;
				rotation = proj_angle = 0.0f;
				proj_dmg = 10;
				proj_delay = 2.0f;
				proj_duration = 5.0f;
				proj_speed = 100;
				health = 100;
			}
		}
		ImGui::End();

		//Display mouse coordinates
		ImGuiWindowFlags window_flags{
			ImGuiWindowFlags_NoDecoration |
			ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoNav
		};

		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowBgAlpha(0.9f);

		if (ImGui::Begin("Map coordinates", nullptr, window_flags)) {

			ImGuiIO& io{ ImGui::GetIO() };
			ImGui::Text(
				"Map coordinates (x=%.1f, y=%.1f)",
				io.MousePos.x + static_cast<float>(camera.x),
				io.MousePos.y + static_cast<float>(camera.y)
			);
		}
		ImGui::End();
		//Logic_End

		//Dear ImGui Render
		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
	}
};

#endif //RENDER_GUI_SYSTEM_HPP
