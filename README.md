# PacMan

Old university assignment that implements a simplified version of PacMan using C++ and a object oriented programming based style.

In cooperation with [Paddy222](https://github.com/Paddy222). :)

## Features

- Classic PacMan gameplay mechanics
- Multiple AI-controlled ghosts (Blinky, Pinky, Inky, Clyde)
- Score tracking and leaderboard system
- Smooth entity movement and collision detection

## Requirements

- CMake (3.10 or higher)
- C++ compiler (C++11 or later)

## Installation

### Clone the Repository

```bash
git clone https://github.com/SoerenFox/PacMan.git
cd PacMan
```

### Build with CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Running the Game

After building, run the executable:

```bash
./PacMan
```

## Project Structure

- **Game.cpp/Game.h** - Main game logic and state management
- **Entity.cpp/Entity.h** - Base class for game entities
- **Pacman.cpp/Pacman.h** - PacMan character implementation
- **Ghost.cpp/Ghost.h** - Base ghost AI class
- **Blinky/Pinky/Inky/Clyde** - Individual ghost implementations with unique behaviors
- **Map.cpp/Map.h** - Game map and collision handling
- **Display.cpp/Display.h** - Rendering and graphics
- **Audio.cpp/Audio.h** - Sound effects and music
- **Leaderboard.cpp/Leaderboard.h** - Score tracking system

## Acknowledgments

Sound effects used in this project are sourced from [thirumuruganra/PACMAN-using-C-and-RAYLIB](https://github.com/thirumuruganra/PACMAN-using-C-and-RAYLIB.git).

## License

MIT License. See [LICENSE](./LICENSE) file for details.
