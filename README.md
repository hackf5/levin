# Levin

A Vulkan-based graphics engine built with C++23.

## Prerequisites

### MSYS2 Environment

This project is built using the **UCRT64** environment in MSYS2. Make sure to use the UCRT64 terminal (not MINGW64 or the base MSYS2 environment).

### Required Packages

Install the required packages in MSYS2 UCRT64:

```bash
pacman -S --needed \
    mingw-w64-ucrt-x86_64-toolchain \
    mingw-w64-ucrt-x86_64-cmake \
    mingw-w64-ucrt-x86_64-ninja \
    mingw-w64-ucrt-x86_64-pkgconf \
    mingw-w64-ucrt-x86_64-vulkan-headers \
    mingw-w64-ucrt-x86_64-vulkan-loader \
    mingw-w64-ucrt-x86_64-vulkan-validation-layers \
    mingw-w64-ucrt-x86_64-glfw \
    mingw-w64-ucrt-x86_64-spdlog \
    mingw-w64-ucrt-x86_64-glslang
```

### Git Submodules

The project uses git submodules for dependencies like ImGui. Initialize them before building:

```bash
git submodule update --init --recursive
```

## Building

### Debug Build

```bash
cmake --preset Debug
cmake --build build/debug
```

### Release Build

```bash
cmake --preset Release
cmake --build build/release
```

## Running

After building, the executable will be in the `bin` directory:

```bash
# Debug
cd ./bin/debug && ./levin.exe

# Release
cd ./bin/release && ./levin.exe
```

## Project Structure

- `src/` - Main source code
  - `vulkan/` - Vulkan wrapper classes
  - `scenes/` - Render scenes
  - `gui/` - ImGui integration
  - `model/` - 3D model structures
- `shaders/` - GLSL shader sources (compiled to SPIR-V)
- `assets/` - Textures and other assets
- `submodules/` - Git submodules (ImGui)
- `libs/` - Third-party libraries (stb_image)

## Dependencies

- **Vulkan** - Graphics API
- **GLFW** - Window and input management
- **GLM** - Mathematics library (fetched via CMake)
- **ImGui** - Immediate mode GUI (submodule)
- **spdlog** - Logging library
- **vk-bootstrap** - Vulkan initialization helper (fetched via CMake)
- **stb_image** - Image loading library

## License

See [LICENSE](LICENSE) file for details.
