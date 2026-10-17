#include "ui/debug_panel.h"
#include "core/debug_settings.h"
#include "scene/Camera.h"
#include "core/camera_controller.h"
#include "core/input.h"
#include <imgui.h>
#include <cstdio>
#include <cfloat>


namespace lume {

	
	void draw_debug_panel(Camera& camera, DebugSettings& settings, const FrameStats& stats, const InputState& input, CameraController& controller)
    {   
        // Independent window: ShowDemoWindow does its own Begin/End
        if (settings.show_demo) {
            ImGui::ShowDemoWindow(&settings.show_demo);
        }

        {
            ImGui::SetNextWindowPos({10, 10}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize({500, 0}, ImGuiCond_FirstUseEver);

            if (ImGui::Begin("Lume")) {
                if (ImGui::CollapsingHeader("Stats", ImGuiTreeNodeFlags_DefaultOpen)) {
                    float fps = stats.cpu_ms_avg > 0 ? 1000.0f / stats.cpu_ms_avg : 0;
                    ImGui::Text("%.2f ms (%.0f FPS)", stats.cpu_ms_avg, fps);

                    char overlay[32];
                    snprintf(overlay, sizeof(overlay), "%.1f ms", stats.cpu_ms_last);
                    ImGui::PlotLines("##frametime",
                                     stats.history.data(),
                                     (int)stats.history.size(),
                                     stats.offset,
                                     overlay,
                                     0.0f,
                                     33.0f,
                                     ImVec2(-FLT_MIN, 100));

                    ImGui::Separator();
                    ImGui::Text("GPU: %s", stats.gpu_name.c_str());
                    ImGui::Text(
                        "Meshes %d | Tris %d | Draws %d", stats.meshes, stats.triangles, stats.draw_calls);
                }

                if (ImGui::CollapsingHeader("Render", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::PushItemWidth(-FLT_MIN);
                    ImGui::Checkbox("VSync", &settings.vsync);
                    ImGui::Checkbox("Wireframe", &settings.wireframe);
                    ImGui::Checkbox("Backface Culling", &settings.cull);

                    const char* modes[] = {"Lit", "Normals", "Flat"};
                    int mode = int(settings.view);

                    ImGui::Text("View");
                    if (ImGui::Combo("##view", &mode, modes, 3)) {
                        settings.view = (ViewMode)mode;
                    }

                    ImGui::Text("Sun");
                    ImGui::InputFloat3("##sun", &settings.sun_dir.x, "%.3f");
                    ImGui::Text("Clear color");
                    ImGui::ColorEdit3("##clear", &settings.clear_color.x);
                    ImGui::PopItemWidth();
                }

                if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
                    // Typed fields: InputFloat has no min/max, so each value is clamped right after its field
                    ImGui::SeparatorText("Transform");
                    ImGui::Text("Position");
                    ImGui::InputFloat3("##cam_pos", &camera.position.x, "%.3f");

                    ImGui::Text("Yaw (deg)");
                    ImGui::InputFloat("##cam_yaw", &camera.yaw, 1.0f, 15.0f, "%.2f");

                    ImGui::Text("Pitch (deg)");
                    ImGui::InputFloat("##cam_pitch", &camera.pitch, 1.0f, 15.0f, "%.2f");
                    camera.clamp_pitch();

                    ImGui::SeparatorText("Lens");
                    ImGui::Text("Vertical FOV (deg)");
                    ImGui::InputFloat("##cam_fov", &camera.fov_y, 1.0f, 5.0f, "%.1f");
                    camera.fov_y = glm::clamp(camera.fov_y, 1.0f, 170.0f);

                    ImGui::Text("Near plane");
                    ImGui::InputFloat("##cam_near", &camera.near_plane, 0.01f, 0.1f, "%.4f");
                    ImGui::Text("Far plane");
                    ImGui::InputFloat("##cam_far", &camera.far_plane, 10.0f, 100.0f, "%.1f");
                    camera.near_plane = glm::max(camera.near_plane, 0.0001f);
                    camera.far_plane  = glm::max(camera.far_plane, camera.near_plane * 1.01f);   // far always beyond near

                    ImGui::SeparatorText("Controller");
                    ImGui::Text("Pivot distance: %.3f", controller.pivot_distance);

                    ImGui::Text("Look sensitivity (deg/px)");
                    ImGui::InputFloat("##ctl_look", &controller.look_sensitivity, 0.01f, 0.1f, "%.3f");
                    controller.look_sensitivity = glm::clamp(controller.look_sensitivity, 0.001f, 5.0f);

                    ImGui::Text("Orbit sensitivity (deg/px)");
                    ImGui::InputFloat("##ctl_orbit", &controller.rotation_sensitivity, 0.01f, 0.1f, "%.3f");
                    controller.rotation_sensitivity = glm::clamp(controller.rotation_sensitivity, 0.001f, 5.0f);

                    ImGui::Text("Fly speed (units/s)");
                    ImGui::InputFloat("##ctl_fly", &controller.fly_speed, 1.0f, 10.0f, "%.2f");
                    controller.fly_speed = glm::clamp(controller.fly_speed, 0.01f, 1000.0f);

                    ImGui::Text("Boost multiplier");
                    ImGui::InputFloat("##ctl_boost", &controller.boost_multiplier, 0.5f, 1.0f, "%.1f");
                    controller.boost_multiplier = glm::clamp(controller.boost_multiplier, 1.0f, 20.0f);

                    ImGui::Text("Zoom step per notch (0-1)");
                    ImGui::InputFloat("##ctl_zoom_step", &controller.zoom_step, 0.01f, 0.05f, "%.2f");
                    controller.zoom_step = glm::clamp(controller.zoom_step, 0.01f, 0.9f);

                    ImGui::Text("Drag zoom sensitivity (per px)");
                    ImGui::InputFloat("##ctl_drag_zoom", &controller.drag_zoom_sensitivity, 0.001f, 0.01f, "%.4f");
                    controller.drag_zoom_sensitivity = glm::clamp(controller.drag_zoom_sensitivity, 0.0001f, 0.1f);

                    ImGui::Text("Min zoom distance");
                    ImGui::InputFloat("##ctl_min_dist", &controller.min_distance, 0.01f, 0.1f, "%.4f");
                    controller.min_distance = glm::max(controller.min_distance, 0.0001f);

                }

                if (ImGui::CollapsingHeader("Input")) {
                    ImGui::Text("Mouse pos   (%.0f, %.0f)", input.mouse_pos.x, input.mouse_pos.y);
                    ImGui::Text("Mouse delta (%.1f, %.1f)", input.mouse_delta.x, input.mouse_delta.y);
                    ImGui::Text("Scroll      %.1f", input.scroll);

                    ImGui::Separator();
                    ImGui::Text("Buttons  L %d  R %d  M %d",
                                (int)input.down(MouseButton::Left),
                                (int)input.down(MouseButton::Right),
                                (int)input.down(MouseButton::Middle));

                    ImGui::Text("WASD %d%d%d%d   QE %d%d",
                                (int)input.down(Key::W),
                                (int)input.down(Key::A),
                                (int)input.down(Key::S),
                                (int)input.down(Key::D),
                                (int)input.down(Key::Q),
                                (int)input.down(Key::E));

                    ImGui::Text("Shift %d  Ctrl %d  Alt %d",
                                (int)input.down(Key::LeftShift),
                                (int)input.down(Key::LeftCtrl),
                                (int)input.down(Key::LeftAlt));
                }




                ImGui::Checkbox("ImGui demo", &settings.show_demo);
            }
            ImGui::End();   // always, even when collapsed
        }
	}


}
