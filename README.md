# My2727GL

**Authors**:
* Nicolas Blin
* Ilan Guenet

## Features

* OpenGL
* Models rendering
    * Vertex
    * 2D texture
    * Multiple lights
* Camera handling
* Camera motion
    * with keyboard
    * with mouse
* `.obj` parsing
* Particles generator
    * Fire generator
    * Teleporter generator


## Build system (cmake)

Requirements:
* [CMake](https://cmake.org/download/)

### Build release

```
mkdir build
cd build
cmake ..
make render
```

### Build debug

```
mkdir build_debug
cd build_debug
cmake ..
make render
```

### Run

To easily run the program, execute **in the build directory**:
```
make run
```

## Scenes

There are two main scenes:
* `scenes/main_big_fire.scene`
* `scenes/main.scene`

To render a scene, include this file in the `main.cc` after the line
```
//------- Scene settings
```

For example to render the `scenes/main_big_fire.scene`:
```
//------- Scene settings
#include "../scenes/main_big_fire.scene"
```


## Links to the videos

### Demo video

[Watch the video](https://youtu.be/F70_gYaZ6aQ)
[![Watch the video](https://img.youtube.com/vi/F70_gYaZ6aQ/maxresdefault.jpg)](https://youtu.be/F70_gYaZ6aQ)

### Technical presentation


[Watch the video](https://youtu.be/ZTLhIN5_h24)
[![Watch the video](https://img.youtube.com/vi/ZTLhIN5_h24/maxresdefault.jpg)](https://youtu.be/ZTLhIN5_h24)