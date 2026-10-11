#pragma once
#include <array>
#include <string>

namespace lume{
	
	struct FrameStats
	{
        //timing
        float cpu_ms_avg = 0;
        float cpu_ms_last = 0;
        std::array<float, 120> history{};
        int offset = 0;
        bool seeded = false;

        //scene
        int meshes = 0;
        int triangles = 0;
        int draw_calls = 0;

        //gpu
        std::string gpu_name;

        void update(float ms) { 
            cpu_ms_last = ms;
            
            if (!seeded) {
                cpu_ms_avg = ms;   // seed with the first sample: no warm-up from 0
                seeded = true;
            }
            else {
                cpu_ms_avg = cpu_ms_avg * 0.95f + 0.05f * ms;
            }
            history[offset] = ms;
            offset = (offset + 1) % (int)history.size();
        }

	};


}