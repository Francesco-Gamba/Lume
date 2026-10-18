#pragma once

namespace lume {
	struct FrameStats;
	struct DebugSettings;
    class Camera;
    struct InputState;
    class CameraController;
    class Scene;
	
	void drawDebugPanel(Camera& camera, DebugSettings& settings, const FrameStats& stats, const InputState& input, CameraController& controller, Scene& scene);
}
