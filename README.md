# Maze
# Task - C/C++ Graphics Developer
# Procedural Maze Explorer

### Prims Algorithm

## Requirements
* C++20 compiler
* CMake
* OpenGL 3.3
* GLFW
* GLM

## Build
Create a build directory, configure, then build:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

Alternatively, from the project root (no `cd` needed):
```bash
cmake -S . -B build
cmake --build build
```
Run the game:
```bash
cd build
./Maze
```

## Command-line Arguments
You can optionally set the maze size:
```bash
./Maze <width> <height>
```

Example:
```bash
./Maze 10 10
```

* `./Maze` → uses the default maze size
* `./Maze 10 10` → creates a 10 × 10 maze
* More than **2 parameters** → the program exits with an error

The program name itself is counted as an argument by `argc`, so `./Maze 10 10` has `argc == 3`.

## Maze Size
The maze size argument refers to the number of **cells**, not raw grid units. Internally, walls occupy their own rows/columns between cells, so the actual grid dimension is:

```
grid_size = 2 * n - 1
```

For example, the default maze size is `5 x 5` cells, which expands to a `9 x 9` grid (`2*5-1 = 9`) to leave room for walls between each cell.

* `./Maze` → default `5 x 5` cells → `9 x 9` grid
* `./Maze 10 10` → `10 x 10` cells → `19 x 19` grid

## Controls
| Key / Input | Action                    |
| ----------- | ------------------------- |
| `W`         | Move forward              |
| `S`         | Move backward             |
| `A`         | Move left                 |
| `D`         | Move right                |
| `Mouse`     | Look around               |
| `F`         | Toggle flying mode        |
| `B`         | Toggle bounding boxes     |
| `G`         | Toggle grid visualization |
| `ESC`       | Exit                      |

### Flying Mode
Press `F` to switch between normal movement and flying mode.

### Debug Modes
* `B` toggles wall bounding boxes.
* `G` toggles the maze grid visualization.

### Textures
* **Wall texture** — by [ZachVance](https://www.deviantart.com/zachvance/art/Dungeon-Wall-Classic-Tile-683633415)
* **Floor / ceiling texture** — by [MakeStuffHappen](https://www.deviantart.com/makestuffhapen/art/Dungeon-Floor-01-423456584).com/makestuffhapen/art/Dungeon-Floor-01-423456584