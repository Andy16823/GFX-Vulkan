# GFX‑Vulkan

GFX‑Vulkan is a hobby game engine built on top of Vulkan. It's a focused, actively developed personal engine intended for making small to mid-sized games and prototypes — not a commercial AAA engine, but a practical and capable foundation that I plan to use for real game projects. The codebase demonstrates engine architecture, rendering pipelines, model loading, text rendering, asset handling, and shader management — suitable for contributors, hobbyists, and indie developers who want a compact, modern Vulkan-based engine.

Status
- Maturity: Hobby engine — feature-complete for many common tasks and actively developed
- Language: C++ (MSVC Visual Studio project included)
- Vendored: `stb_image.h` (single-header image loader)
- Platforms: Primary development on Windows (Visual Studio). Linux/macOS support is possible with some build work.

What it is and what it isn’t
- This is a serious hobby engine designed to be used for actual games and prototypes. It’s not trying to replace Unity/Unreal, but the goal is to be robust and practical enough to compete with other small engines and to be used as the basis for finished hobby games.
- The project is pragmatic: clear code structure, reusable systems (rendering, materials, asset loading), and a small surface area that makes extension and maintenance easy.

Key features
- Vulkan renderer with modular pipeline and resource management
- Model loading via Assimp (external)
- Windowing and input via GLFW (external)
- Font rendering with FreeType (external)
- Math via glm (external)
- Asset management, shader hot-copy (post-build copies), and basic PBR/unlit material systems
- Post-build script copies shaders and public engine headers into the build output for convenient testing

Repository structure
- `VulkanApp.sln` — Visual Studio solution
- `VulkanApp/` — main project with source and headers (Graphics, Core, Assets, Math, etc.)
- `shaders/` — shader sources (copied into output by Post-Build)
- `stb_image.h` — vendored single-header image loader
- `x64/` — build output (should be ignored by git; do not commit build artifacts)
- `LICENSE` — project license (engine code)

Why use or follow this engine
- Focused and compact: small surface area for understanding and extending.
- Practical: intended to be used for playable hobby games and prototypes.
- Educational value: the codebase is organized so you can learn modern Vulkan patterns while working on actual projects.
- Extensible: designed to accept new rendering features, tools, or platform ports.

Prerequisites (developer machine)
- Windows 10/11 (recommended) with Visual Studio 2022 (C++ workload) or equivalent MSVC toolset
- LunarG Vulkan SDK (for development headers & libs). Example tested version: 1.4.328.0
- GLFW, Assimp, FreeType, glm — install via vcpkg or system package manager
- Optional: vcpkg to simplify dependency management

Quick start — Windows (Visual Studio)
1. Install Visual Studio 2022 (Desktop development with C++).
2. Install the LunarG Vulkan SDK (https://vulkan.lunarg.com/) — the installer usually sets `VULKAN_SDK`.
3. (Recommended) Use vcpkg:
   - git clone https://github.com/microsoft/vcpkg
   - run `bootstrap-vcpkg.bat`
   - `vcpkg integrate install`
   - `vcpkg install glfw3 assimp freetype glm`
4. Open `VulkanApp.sln` in Visual Studio.
5. Select configuration `Release | x64` (or `Debug | x64`) and build.
6. The Post-Build step copies `shaders/` and engine headers to the output folder. Run the executable from the build output folder.

Notes for Linux / macOS
- The repo currently includes a Visual Studio project. To target Linux/macOS, add or adapt a CMake build and consider MoltenVK for macOS (Vulkan-on-Metal).
- Platform ports are part of the roadmap.

Project configuration tips
- Avoid hard-coded absolute include/lib paths — use environment variables or package managers:
  - Example: use `$(VULKAN_SDK)\Include` and `$(VULKAN_SDK)\Lib` in Visual Studio instead of `C:\VulkanSDK\...`.
- Ensure `x64/` and other build output directories are in `.gitignore`. If `x64/` is tracked, remove it from the index with:
  - `git rm -r --cached x64`
  - commit & push

Dependencies summary
- Vendored:
  - `stb_image.h` (included — keep its license header)
- External (user must install):
  - Vulkan SDK (LunarG) or platform Vulkan runtime & GPU drivers
  - GLFW
  - Assimp
  - FreeType
  - glm
See `DEPENDENCIES.md` for installation instructions and tested versions.

Packaging & releases
- When publishing binaries:
  - Create a ZIP per platform including: executable, allowed redistributable DLLs, `shaders/`, `legal/` (vendored license copies), and `README_RUN.txt`.
  - Include SHA256 checksums.
  - In the release notes, list required external dependencies (Vulkan runtime, GPU drivers, SDK version if relevant).

Licensing & legal
- Engine code license: see `LICENSE` in the repo root.
- Vendored license:
  - `stb_image.h` contains its license header — a copy should be placed in `legal/STB_IMAGE_LICENSE.txt`.
- If you later distribute third‑party binaries, include their original license texts in `legal/`.

Contributing
- Contributions welcome: small, focused PRs are preferred.
- If you vendor third-party code, include the original license text in `legal/`.
- Follow the existing code style and add a short description of changes to your PR.

Roadmap / next goals
- Harden engine APIs and polish material/scene APIs
- Add more demo scenes and gameplay examples
- Add cross-platform CMake build and GitHub Actions CI (Windows + Linux)
- Publish prebuilt demo releases with included `legal/` for easy testing

Contact / credits
- Repo: https://github.com/Andy16823/GFX-Vulkan
- Author: Andy16823
- Third-party libraries used (not bundled): glm, GLFW, Assimp, FreeType, LunarG Vulkan SDK, stb_image