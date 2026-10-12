#pragma once

namespace lume {
	struct FrameStats;
	struct DebugSettings;
    class Camera;
    struct InputState;
	
	void draw_debug_panel(Camera& camera, DebugSettings& settings, const FrameStats& stats, const InputState& input);
}