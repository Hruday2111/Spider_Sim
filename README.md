# 3D Spider Simulation

OpenGL/GLUT bedroom scene featuring a controllable spider that can move across the floor, walls, ceiling, and selected furniture surfaces.

## Features

- Articulated spider model with animated legs
- Bedroom environment with bed, wardrobe, bookshelf, desk, dresser, armchair, clock, paintings, and door
- Surface traversal system for floor, walls, ceiling, and climbable objects
- Three camera modes:
  - `1` Follow camera
  - `2` Spider POV
  - `3` Free orbit
- Toggleable ceiling light with smooth brightness transition
- Animated bedroom door
- Procedural room textures and wall art images

## Controls

- `W / S` Move forward / backward
- `A / D` Turn left / right
- `C` Change surface at an edge or corner
- `L` Toggle ceiling light
- `O` Open / close door
- `1` Follow camera
- `2` Spider POV
- `3` Free orbit
- `Arrow Up / Down` Free-orbit tilt
- `Arrow Left / Right` Free-orbit orbit
- `Mouse Drag` Free-orbit camera rotate
- `Scroll` or `+ / -` Zoom camera
- `Esc` Quit

## Build

### Linux

```bash
g++ main.cpp -o spider -lGL -lGLU -lglut -ljpeg -lm
```

### macOS

```bash
g++ main.cpp -o spider -framework OpenGL -framework GLUT -lm
```

### Windows (MinGW / FreeGLUT)

```bash
g++ main.cpp -o spider -lfreeglut -lopengl32 -lglu32
```

## Run

```bash
./spider
```

## Project Structure

- `main.cpp` Entry point and module includes
- `common.hpp` Global state, math helpers, tuning constants, collision data
- `app.cpp` Camera logic, input handling, display loop, HUD, timer
- `scene.cpp` Room, furniture, door, lighting, wall art
- `spider.cpp` Spider model, animation, and shadow rendering
- `geometry.cpp` Low-level drawing helpers
- `textures.cpp` Texture loading and procedural texture generation

## Assets

- `Textures/` contains room surface texture assets
- `Images/` contains wall art images

## Notes

- The project uses the fixed-function OpenGL pipeline for portability and simplicity.
