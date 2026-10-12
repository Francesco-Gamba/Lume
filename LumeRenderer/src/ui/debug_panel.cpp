#include "ui/debug_panel.h"
#include "core/debug_settings.h"
#include "scene/Camera.h"
#include "core/input.h"
#include <imgui.h>
#include <cstdio>
#include <cfloat>


namespace lume {

	
	void draw_debug_panel(Camera& camera, DebugSettings& settings, const FrameStats& stats, const InputState& input)
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
                    ImGui::DragFloat3("##sun", &settings.sun_dir.x, 0.01f, -1.0f, 1.0f);
                    ImGui::Text("Clear color");
                    ImGui::ColorEdit3("##clear", &settings.clear_color.x);
                    ImGui::PopItemWidth();
                }

                if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Text("Position");
                    ImGui::DragFloat3("##pos", &camera.position.x, 0.05f);
                    ImGui::Text("Yaw / Pitch");
                    ImGui::DragFloat("##yaw", &camera.yaw, .5f);
                    ImGui::DragFloat("##pitch", &camera.pitch, .5f, -89.0f, 89.0f);
                    ImGui::Text("FOV");
                    ImGui::SliderFloat("##fov", &camera.fov_y, 10.0f, 120.0f, "%.1f deg");
                    ImGui::Text("Near/Far");
                    ImGui::DragFloat("##near", &camera.near_plane, 0.001f, 0.001f, camera.far_plane);
                    ImGui::DragFloat("##far", &camera.far_plane, 1.0f, camera.near_plane, 100000.0f);
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

                    // edge-detection test: should go up by exactly 1 per press, not while held
                    static int num1_presses = 0;
                    if (input.pressed(Key::Num1))
                        ++num1_presses;
                    ImGui::Text("Key 1 presses: %d", num1_presses);
                }


                ImGui::Checkbox("ImGui demo", &settings.show_demo);
            }
            ImGui::End();   // always, even when collapsed
        }
	}


}
