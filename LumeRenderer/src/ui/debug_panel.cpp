#include "ui/debug_panel.h"
#include "core/debug_settings.h"
#include <imgui.h>
#include <cstdio>


namespace lume {

	
	void draw_debug_panel(const FrameStats& stats) {
        ImGui::SetNextWindowPos({10, 10}, ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize({320, 0}, ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Lume")) {
			if (ImGui::CollapsingHeader("Stats", ImGuiTreeNodeFlags_DefaultOpen)) {
				float fps = stats.cpu_ms_avg > 0 ? 1000.0f/ stats.cpu_ms_avg : 0;
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
                                 ImVec2(0, 60));

                ImGui::Separator();
                ImGui::Text("GPU: %s", stats.gpu_name.c_str());
                ImGui::Text(
                    "Meshes %d | Tris %d | Draws %d", stats.meshes, stats.triangles, stats.draw_calls);
                
			}
		}

         ImGui::End();
	}


}
