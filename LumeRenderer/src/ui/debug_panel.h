#pragma once

namespace lume {
	struct FrameStats;
	struct DebugSettings;
    class Camera;
    struct InputState;
    class CameraController;
	
	void draw_debug_panel(Camera& camera, DebugSettings& settings, const FrameStats& stats, const InputState& input, CameraController& controller);
}