# 🕯️ Lume Engine

**Lume** is a lightweight, component system and modular 2D game engine built with **C++20**, **SDL2**, and **OpenGL**. It is designed as a reusable static library, allowing developers to focus on game logic while the engine handles the underlying hardware abstraction and rendering pipeline.

---

## ✨ Features
* **Modern OpenGL Renderer:** High-performance 2D rendering using programmable pipelines (Shaders).
* **Static Library Architecture:** Clean separation between engine code and game projects.
* **Integrated Logging:** High-speed, formatted console and file logging via `spdlog`.
* **Resource Management:** Efficient handling of Textures, Shaders, and Audio assets.
* **Input System:** Abstracted event handling for Keyboard, Mouse, and Controllers.
* **Math Suite:** Optimized linear algebra provided by `GLM`.

---

## 🛠️ Tech Stack
Lume leverages industry-standard libraries (linked as Git Submodules):
* [SDL2](https://www.libsdl.org/) - Window management and low-level input.
* [GLM](https://github.com/g-truc/glm) - OpenGL Mathematics.
* [spdlog](https://github.com/gabime/spdlog) - Fast C++ logging library.
* [Glad](https://glad.dav1d.de/) - OpenGL Multi-Language GL/GLES/WGL/EGL/GLX/OSMesa loader-generator.

---

## 📂 Project Structure
```text
/Lume
  ├── bin/              # Compiled binaries (.lib files)
  ├── build/            # Intermediate build files (.obj)
  ├── include/Lume/     # Public API headers
  ├── src/              # Engine source code (.cpp)
  ├── vendor/           # Third-party libraries (Submodules)
  └── Lume.sln          # Visual Studio 2022 Solution
