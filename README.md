# OpenGL Model Loader

This project loads and displays a 3D model using OpenGL.

It uses GLFW for window and input handling, GLAD for OpenGL function loading, GLM for math, and Assimp to load the model file.

The application opens a full-screen GLFW window and renders a backpack model from `assets/models/backpack/backpack.obj` with a simple shader.

Camera movement is controlled with:
- `W` / `S` to move forward/backward
- `A` / `D` to move left/right
- mouse movement to look around
- mouse wheel to zoom the camera

The renderer clears the screen each frame, updates the camera view, moves a light source over time, and draws the loaded model.

## Build

Use CMake and a compiler that supports C++17.

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Run

After building, run the executable created in `build/`.

The project currently uses hard-coded absolute paths for the shader and model files in `src/main.cpp`.
