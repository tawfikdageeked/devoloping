```markdown
<div align="center">

# ⚛️ PhysicsGL Engine

### A Stupidly Simple C++ / OpenGL Engine

*Because graphics programming shouldn't require a PhD*

<img src="https://img.shields.io/badge/Language-C++-00599C?style=flat-square&logo=cplusplus&logoColor=white">
<img src="https://img.shields.io/badge/Graphics-OpenGL-5586A4?style=flat-square&logo=opengl&logoColor=white">
<img src="https://img.shields.io/badge/Platform-Linux-FCC624?style=flat-square&logo=linux&logoColor=black">
<img src="https://img.shields.io/badge/License-MIT-green?style=flat-square">

---

**Raw OpenGL:**
```cpp
GLFWwindow* window = glfwCreateWindow(800, 600, "Hello", NULL, NULL);
glfwMakeContextCurrent(window);
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
```

**PhysicsGL:**
```cpp
WindowManager.Start();
WindowManager.CreateWindow("main", 800, 600, "Hello");
WindowManager.UseWindow("main");
```

*Same result. Half the code. Zero jargon.*

</div>

---

## 📦 Installation

### Dependencies

```bash
sudo apt install -y libglfw3-dev libglew-dev build-essential
```

### Get the Engine

```bash
git clone https://github.com/yourusername/physicsgl.git
```

### Compile

```bash
g++ yourfile.cpp -o yourprogram -I/path/to/engine -lglfw -lGL -lGLEW -lm
```

---

## 🚀 Quick Start

```cpp
#include "engine.hpp"

int main()
{
    WindowManager.Start();
    WindowManager.CreateWindow("main", 800, 600, "My First Window");
    WindowManager.UseWindow("main");

    while (WindowManager.IsOpen("main"))
    {
        WindowManager.Update("main");
        WindowManager.Manage();
    }

    WindowManager.StopWindow("main");
    WindowManager.Stop();
    return 0;
}
```

**That's it. 17 lines. A GPU-accelerated window on your screen.**

---

## 📖 API Reference

### Core Functions

| Function | What It Does |
|----------|-------------|
| `WindowManager.Start()` | Initialize engine with sensible defaults |
| `WindowManager.Stop()` | Shut everything down |

### Window Management

| Function | What It Does |
|----------|-------------|
| `WindowManager.CreateWindow(name, w, h, title)` | Create a named window |
| `WindowManager.UseWindow(name)` | Tell OpenGL to draw on this window |
| `WindowManager.IsOpen(name)` | Check if window is still open |
| `WindowManager.Update(name)` | Show the current frame |
| `WindowManager.Manage()` | Handle keyboard/mouse/resize events |
| `WindowManager.StopWindow(name)` | Close a specific window |

### Advanced

| Function | What It Does |
|----------|-------------|
| `WindowManager.TakeHints(major, minor, profile)` | Override default OpenGL version |

---

## 🎯 Examples

### Minimal Window

```cpp
#include "engine.hpp"

int main()
{
    WindowManager.Start();
    WindowManager.CreateWindow("win", 800, 600, "Minimal");
    WindowManager.UseWindow("win");

    while (WindowManager.IsOpen("win"))
    {
        WindowManager.Update("win");
        WindowManager.Manage();
    }

    WindowManager.StopWindow("win");
    WindowManager.Stop();
    return 0;
}
```

### Custom OpenGL Version

```cpp
#include "engine.hpp"

int main()
{
    WindowManager.Start();
    WindowManager.TakeHints(4, 3, WindowMngr::core);
    WindowManager.CreateWindow("win", 1280, 720, "OpenGL 4.3");
    WindowManager.UseWindow("win");

    while (WindowManager.IsOpen("win"))
    {
        WindowManager.Update("win");
        WindowManager.Manage();
    }

    WindowManager.StopWindow("win");
    WindowManager.Stop();
    return 0;
}
```

### Multiple Windows

```cpp
#include "engine.hpp"

int main()
{
    WindowManager.Start();
    WindowManager.CreateWindow("editor", 1280, 720, "Editor");
    WindowManager.CreateWindow("preview", 400, 400, "Preview");
    WindowManager.UseWindow("editor");

    while (WindowManager.IsOpen("editor"))
    {
        WindowManager.Update("editor");
        WindowManager.Update("preview");
        WindowManager.Manage();
    }

    WindowManager.StopWindow("editor");
    WindowManager.StopWindow("preview");
    WindowManager.Stop();
    return 0;
}
```

---

## 🧠 Design Philosophy

```
1. Simple by default, powerful when needed
2. No pointers visible to the user
3. Name your windows, don't manage pointers
4. Sensible defaults, optional overrides
5. If a beginner can't read it, it's too complex
```

---

## 🛣️ Roadmap

- [x] Window management
- [ ] Screen clearing (background color)
- [ ] Shader compilation
- [ ] Mesh creation and rendering
- [ ] Uniform system (send data to GPU)
- [ ] Keyboard/mouse input helpers
- [ ] Camera system
- [ ] Particle physics system
- [ ] Fluid simulation
- [ ] Cloth simulation
- [ ] Compute shaders

---

## 🔧 Technical Details

### What's Under the Hood

```
GLFW     → Window creation and input handling
GLEW     → OpenGL function loading
OpenGL   → GPU communication
C++      → Everything else
```

### Supported Profiles

| Profile | Constant | Description |
|---------|----------|-------------|
| Core | `WindowMngr::core` | Modern OpenGL only |
| Compatibility | `WindowMngr::comp` | Modern + legacy OpenGL |

### Default Settings

```
OpenGL Version:  3.3
Profile:         Core
```

---

## 📐 The Loop Pattern

Every program follows this structure:

```cpp
// ===== SETUP =====
WindowManager.Start();
WindowManager.CreateWindow("main", 800, 600, "App");
WindowManager.UseWindow("main");

// ===== LOOP =====
while (WindowManager.IsOpen("main"))
{
    // Clear screen
    // Update physics
    // Draw stuff

    WindowManager.Update("main");
    WindowManager.Manage();
}

// ===== CLEANUP =====
WindowManager.StopWindow("main");
WindowManager.Stop();
```

---

## 🤝 Contributing

This engine is built by a student on an Android tablet running a hardware-accelerated Debian chroot with Turnip+Zink GPU drivers. If that doesn't stop you, nothing will.

Pull requests welcome.

---

## 📄 License

MIT — Do whatever you want with it.

---

<div align="center">

*Built with stubbornness and zero budget*

**⚛️ PhysicsGL**

</div>
```