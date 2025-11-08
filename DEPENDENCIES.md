# Dependencies — GFX‑Vulkan

This document lists the external libraries and tools the project uses, how they are provided, and how to install them. Keep versions you tested here so other contributors can reproduce your setup.

## Vendored in this repo
- stb_image.h
  - Single-file header included in the repository.
  - Action required: keep the license/copyright block inside `stb_image.h` unchanged and add a copy to `legal/STB_IMAGE_LICENSE.txt`.

## Not vendored (must be installed by user)
These dependencies are expected to be installed on the developer / user machine or available via package manager. They are not distributed in the repo.

- Vulkan
  - What: Vulkan loader/runtime (provided by GPU drivers) and the LunarG Vulkan SDK for development.
  - Example tested version: 1.4.328.0
  - Official: https://vulkan.lunarg.com/ & https://github.com/KhronosGroup/Vulkan-Loader
  - Note: Do NOT include SDK binaries in the repo. If you ever bundle SDK parts in a release, include the SDK's license files in `legal/`.

- GLFW
  - Purpose: Windowing and input
  - Install: vcpkg (`vcpkg install glfw3`), system package manager, or build from source.
  - Official: https://www.glfw.org/

- Assimp
  - Purpose: Model import
  - Install: vcpkg (`vcpkg install assimp`) or platform packages
  - Official: https://github.com/assimp/assimp

- FreeType
  - Purpose: Font/text rendering
  - Install: vcpkg (`vcpkg install freetype`) or platform packages
  - Official: https://www.freetype.org/

- glm
  - Purpose: Header-only math library
  - Install: vcpkg (`vcpkg install glm`), distro package, or copy headers
  - Official: https://github.com/g-truc/glm

## Recommended install methods

### Windows (recommended workflow)
- Install Visual Studio 2022 (C++ workload).
- Install LunarG Vulkan SDK (installer typically sets `VULKAN_SDK`).
- Install vcpkg and required packages:
  - git clone https://github.com/microsoft/vcpkg
  - bootstrap and integrate with VS: `bootstrap-vcpkg.bat` then `vcpkg integrate install`
  - Install libs: `vcpkg install glfw3 assimp freetype glm`
- In Visual Studio, ensure `VULKAN_SDK` environment variable is set (installer usually does it).
- Open `VulkanApp.sln`, set configuration `Release|x64`, build.

Example quick commands (PowerShell):
- Set VULKAN_SDK temporarily: `$env:VULKAN_SDK="C:\VulkanSDK\1.4.328.0"`
- Build via CMake (if you add CMake support): `cmake -S . -B build -A x64 -DCMAKE_TOOLCHAIN_FILE=path\to\vcpkg.cmake && cmake --build build --config Release`

### Linux (Ubuntu example)
- Install Vulkan runtime/tools: `sudo apt install libvulkan1 vulkan-utils` or use LunarG SDK.
- Install dependencies: `sudo apt install libglfw3-dev libassimp-dev libfreetype6-dev libglm-dev`
- Build with CMake: `cmake -S . -B build && cmake --build build -j`

### macOS
- Consider MoltenVK if you target macOS (Vulkan-on-Metal). Install with Homebrew or via LunarG SDK bundle.
- Install libs: `brew install glfw assimp freetype glm`
- Note: macOS support may need additional adjustments and licensing checks for MoltenVK.

## Project / build notes
- Replace absolute paths in the Visual Studio project with variables:
  - Use `$(VULKAN_SDK)\Include` and `$(VULKAN_SDK)\Lib` instead of hard-coded `C:\VulkanSDK\...`.
- Post-build steps copy `shaders` and headers into the output directory. Those copies are local build artifacts and should not be committed.
- The repo must not include binary outputs. Ensure `x64/` and other build output directories are in `.gitignore`. If build outputs were committed previously, remove them from git history or untrack with `git rm -r --cached x64`.

## Licensing and legal
- Your project license (e.g., MIT) stays in `LICENSE`.
- For vendored files (stb_image.h) include the original license text in `legal/STB_IMAGE_LICENSE.txt`.
- For dependencies you do not distribute, include a reference in this file with tested version and URL. If you later distribute any third-party binaries, add their LICENSE files to `legal/` and include them in release archives.

## Release packaging checklist
- Do NOT include full SDKs or system-provided runtimes in repo.
- When creating release ZIPs that include built binaries:
  - Include: EXE, required redistributable DLLs (only those you are allowed to redistribute), `shaders/`, `legal/`, `README_RUN.txt`.
  - Add checksums (SHA256) for artifacts.
  - Clearly state which external dependencies users must install (Vulkan SDK, GPU drivers).

## Troubleshooting pointers
- If build fails with missing Vulkan headers: ensure `VULKAN_SDK` is set and points to a valid SDK install.
- If linker complains about missing `vulkan-1.lib` or similar: confirm the SDK's Lib path is added (or use vcpkg-provided libs).
- If runtime fails to create a Vulkan instance: ensure GPU drivers are up to date and support the Vulkan version you target.