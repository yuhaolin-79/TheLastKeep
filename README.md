# The Last Keep · 最后的要塞

A 2D tower defense game built with Qt 6 and C++.

## Overview

The Last Keep is a classic 2D tower defense game where players build and upgrade towers to defend against waves of enemies. The goal is to prevent enemies from reaching and destroying your last stronghold.

> 🎮 **Core Loop:** Build towers → Defeat enemies → Earn gold → Choose upgrade cards → Survive the next wave!

## Tech Stack

| Area | Technology |
|------|-----------|
| **Language** | C++17 |
| **Framework** | Qt 6.11 (Core, Widgets) |
| **Graphics** | QGraphicsView / QGraphicsScene |
| **Build System** | CMake 3.19+ |
| **Compiler** | MSVC 2022 |
| **Platform** | Windows (Linux/macOS planned) |

## Repository

[TheLastKeep on GitHub](https://github.com/yuhaolin-79/TheLastKeep)

```bash
git clone git@github.com:yuhaolin-79/TheLastKeep.git
```

## Project Structure

```
TheLastKeep/
├── README.md
├── dev.md                          # Development log
├── DesignDoc/                      # Architecture design documents
│   ├── TheLastKeep最后的要塞-架构设计.md
│   └── Drawing 2026-07-06 16.38.23.excalidraw.md
│
└── TheLastKeep/
    ├── CMakeLists.txt              # CMake build config
    ├── src/
    │   ├── main.cpp                # Application entry point
    │   ├── ui/
    │   │   ├── MainWindow.h        # Main window (QMainWindow)
    │   │   ├── MainWindow.cpp
    │   │   └── mainwindow.ui       # Qt Designer UI file
    │   └── game/
    │       ├── GameScene.h         # Game canvas (QGraphicsScene)
    │       └── GameScene.cpp
    └── resources/
        ├── resources.qrc           # Qt resource file
        └── images/
            └── background.png      # Game background
```

## Architecture

The game follows a **5-layer design**:

```
┌─────────────────────────────────┐
│   UI Layer (MainWindow, Menus)  │  ← Qt Widgets
├─────────────────────────────────┤
│   Scene Layer (GameScene)       │  ← QGraphicsView/Scene
├─────────────────────────────────┤
│   Control Layer (GameController)│  ← Game rules & state
├─────────────────────────────────┤
│   Entity Layer                  │  ← Enemy, Tower, Bullet, Card, Map
├─────────────────────────────────┤
│   Resource Layer                │  ← Images, Sounds, Game Data
└─────────────────────────────────┘
```

### Core Classes (Planned)

| Class | Responsibility |
|-------|---------------|
| `MainWindow` | Application window, game start/pause/restart |
| `GameScene` | Game canvas — renders map, enemies, towers, bullets |
| `GameController` | Game rules — gold, castle HP, score, win/loss |
| `GameMap` | Grid-based map with path and buildable tiles |
| `Enemy` | Moves along path, has HP/speed/reward |
| `Tower` | Auto-attacks enemies in range, generates bullets |
| `Bullet` | Tracks target enemy, deals damage on hit |
| `WaveManager` | Spawns enemy waves with configurable patterns |
| `CardManager` | Generates 3-card choices between waves |

### Card System

Between waves, players choose from 3 random upgrade cards:

- 🏹 Tower Damage / Range / Speed Up
- 💰 Gold Bonus
- 🏰 Castle Heal
- 🐌 Enemy Slow

### Planned Enemy Types

| Enemy | Trait |
|-------|-------|
| Goblin | Basic, balanced |
| Heavy Orc | High HP, slow |
| Wolf Rider | Low HP, fast |
| Warlock | Magic resistant |
| Boss | Very high HP |

### Planned Tower Types

| Tower | Trait |
|-------|-------|
| Arrow Tower | Balanced DPS |
| Mage Tower | High damage, slow |
| Cannon Tower | AoE splash |
| Ice Tower | Slows enemies |
| Holy Tower | Bonus vs undead |

## Status

### ✅ Completed
- Qt CMake project setup
- `MainWindow` + `GameScene` + `QGraphicsView` framework
- Background image rendering via Qt resource system
- Demo map drawing (path, tower slots, castle marker)

### 🚧 In Progress
- `GameMap` — grid map with road/buildable tiles
- `Enemy` — path-following movement
- `Tower` — placement and auto-attack
- `Bullet` — tracking projectile

### 📋 Planned
- `GameController` — gold & HP system
- `WaveManager` — timed enemy spawns
- `CardManager` — between-wave upgrades
- UI polish — menus, HUD, game-over screen
- Art & sound assets

## Development Order

```
1. ✅ Qt CMake project
2. ✅ GameScene with display
3. 🚧 GameMap — draw road & buildable zones
4. 🚧 Enemy — path-following movement
5. 🚧 Tower — click-to-place & attack
6. ⬜ Bullet — auto-target & hit detection
7. ⬜ GameController — gold & castle HP
8. ⬜ WaveManager — timed spawns
9. ⬜ CardManager — between-wave choices
10. ⬜ Art replacement & UI polish
```

## Build & Run

### Prerequisites

- Qt 6.5+ (with MSVC 2022 on Windows)
- CMake 3.19+
- Visual Studio 2022 (or Qt Creator)

### Build with Qt Creator

1. Open `TheLastKeep/CMakeLists.txt` in Qt Creator
2. Select a Qt 6 kit (e.g., Desktop Qt 6.11.1 MSVC2022 64bit)
3. Build & Run (Ctrl+R)

### Build from Command Line

```powershell
cd TheLastKeep
cmake -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.11.1/msvc2022_64"
cmake --build build --config Debug
```

## Key Parameters

| Parameter | Value |
|-----------|-------|
| Window Size | 1280 × 720 |
| Tile Size | 64 × 64 |
| Map Grid | 16 × 10 |
| Starting Gold | 150 |
| Castle HP | 10 |
| Game Tick | 16ms (~60 FPS) |

## Team

- **陈思睿** — Core combat logic (Enemy, Tower, Bullet, Collision)
- **刘嘉航** — Map & wave system (GameMap, Tile, WayPoint, WaveManager)
- **鱼浩琳** — UI & card system (MainWindow, GameScene, Card, CardManager)

## License

TBD
