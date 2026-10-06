# Lume

A custom real-time glTF renderer, built from scratch on OpenGL 4.6 core and C++20.
Learning project: start at mesh display, add one technique per effect, keep the scope contained.

Full plan, milestones and schedule: [`docs/PLAN.md`](docs/PLAN.md).

## Build

Windows, MSVC, Ninja, vcpkg in manifest mode.

```sh
cmake --preset x64-relwithdebinfo
cmake --build --preset x64-relwithdebinfo
```

Use **RelWithDebInfo** for daily work — a Debug build of a renderer is very slow.
Reach for Debug only when you need the MSVC iterator checks or a clean call stack.

> The build is not wired up yet: the root `CMakeLists.txt` is still the Visual Studio
> wizard output and there is no `vcpkg.json`. See *Setup still to do* below.

## Layout

```
docs/          plan, per-milestone notes, reference comparison captures
external/      third-party sources committed into the repo (glad2 loader, mikktspace)
src/
  core/        app loop, window, input, time, logging, path resolution, jobs
  math/        aabb, frustum, octahedral, SH, sampling — no GL, no deps, unit-tested
  gfx/         GL RAII wrappers, shader system (#include preprocessor, hot-reload), debug output, GPU timers
  scene/       glTF import, nodes, transforms, materials, lights, cameras, GI proxies
  renderer/    passes/ ibl/ ao/ gi/ reflections/
  ui/          ImGui panels, gizmos
shaders/       common/ is the include root; passes/ gi/ ao/ post/ by subsystem
tools/
  blender/     export helpers (probe & reflection volume empties -> glTF extras)
  python/      shader validation, reference asset fetch
tests/         unit tests, mainly for math/
assets/        scenes/ env/ textures/ — committed via Git LFS
```

Not in the repo, and deliberately so:

- **`assets/reference/`** — Khronos glTF-Sample-Assets. Gitignored; fetched by
  `tools/python/fetch_reference.py`. Hundreds of MB that don't belong in your LFS quota.
- **`D:\FRANCESCO\Lume\art\`** — Blender working files, a sibling folder outside Git.
  `.blend` files don't delta-compress, so every save would be a full new LFS blob.
  Export `.glb` from there into `assets/scenes/`.
- **`out/`, `config/`, `cache/`** — build output, saved settings, the Embree bake cache.
  All generated, all gitignored.

## Conventions

- **Filenames are lowercase**: `shader.h`, `probe_volume.cpp`. Decided early so there's
  nothing to rename at 200 files.
- **`src/` is the only include root**: `#include "gfx/shader.h"`, never a relative `../../`.
  Headers sit next to their sources — there's no public API to publish, so no `include/` tree.
- **`shaders/` is the only shader include root**: `#include "common/brdf.glsl"`.
- **Shader extensions carry meaning**: `.vert` / `.frag` / `.comp` are entry points,
  `.glsl` is include-only. The hot-reload watcher and shader validation rely on this.
- **One technique per effect.** One GI method, one AO method, one reflection method.
- **`math/` stays dependency-free** so it can be unit-tested without a GL context. The
  octahedral and SH round-trips in particular — a subtly wrong seam reads as a GI leak.

## Setup still to do

1. `git init`, then `git lfs install` **before** adding anything under `assets/`.
2. Delete the wizard leftovers: `LumeRenderer.h`, and move `LumeRenderer.cpp` to `src/main.cpp`.
3. Rewrite the root `CMakeLists.txt`:
   - `cmake_minimum_required(VERSION 3.25)` (3.10 blocks features you'll want)
   - a `lume_core` static library + a thin `lume` executable, so `tests/` can link the library
   - `target_include_directories(lume_core PUBLIC src)`
   - one `CMakeLists.txt` per `src/` subdirectory using `target_sources`, no `file(GLOB)`
   - a generated `lume_paths.h` carrying `CMAKE_SOURCE_DIR`, so shaders hot-reload from
     the source tree instead of a copy in the build directory
4. Fix `CMakePresets.json`: add the missing **x64-relwithdebinfo** preset, drop both x86
   configs, drop the `SegmentHeap.cmake` injection.
5. Add `vcpkg.json` with the M0 dependencies: glfw3, glm, imgui (docking + glfw/opengl3
   bindings), fastgltf, stb, meshoptimizer, tracy. Embree 4 lands at M7.
6. Generate the glad2 loader (GL 4.6 core + `ARB_bindless_texture`) into `external/glad/`.
