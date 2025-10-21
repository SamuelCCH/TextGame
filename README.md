# TextAdventure (C++ Console Game)

A simple, turn-based text adventure written in C++ for the console. Choose a class, explore, fight goblins, manage your inventory, and reach one of multiple endings.

## Overview
- Single-file gameplay (`main.cpp`) with supporting headers for core types.
- Turn-based combat with positional distance and weapon ranges.
- Three player classes with distinct starting weapons.
- Inventory management with equip/throw actions.
- Multiple endings based on final fight outcomes.

## Features
- Classes: Mage (Staff), Knight (Sword), Thief (Bow)
- Enemies: Archer Goblin, Melee Goblin, King Goblin (final boss)
- Loot: Weapons drop from goblins (bows, swords, staffs) with varying stats
- Combat: Attack, flee, or move closer; range matters
- Inventory: Up to 5 slots, equip or discard items

## Controls
- Main Menu: Enter `1` to Start Game, `2` to Quit
- Adventure Menu:
  - `1` Move forward
  - `2` Rest (fully heal)
  - `3` Inventory (equip or throw)
  - `4` Player stats
- Combat:
  - `a` Attack
  - `f` Flee
  - `m` Move (close the distance)
- Prompts: Press Enter when prompted to continue

## Requirements
- Windows with a C++ compiler (MinGW `g++`, MSYS2, or Code::Blocks)
- C++17 or later recommended

## Run (Prebuilt Binary)
- Double-click `bin\Debug\TextAdventure.exe` (provided) to play
  - Alternatively, `main.exe` exists at project root; use either if present

## Build From Source
Using MinGW or MSYS2 on Windows:

```bash
# From the project root
# If using PowerShell:
g++ -std=c++17 main.cpp -o TextAdventure.exe

# Run
./TextAdventure.exe
```

Using Code::Blocks:
- Open `TextAdventure.cbp`
- Choose target `Debug` or `Release`
- Build and run

## Gameplay Notes
- Pick a class and start with its basic weapon.
- Combat depends on distance and weapon range; moving reduces the distance to the enemy.
- Fleeing the final boss results in a “retreated” ending; victory or defeat produce different endings.

## Project Structure
```
TextAdventure/
├── Enemy.h
├── GameManager.h
├── Player.h
├── Weapon.h
├── main.cpp
├── TextAdventure.cbp
├── bin/Debug/TextAdventure.exe
└── obj/Debug/main.o
```

## Architecture
- `GameManager`: Handles game state (start, pause, end, player turn)
- `Player` and subclasses:
  - `Mage`, `Knight`, `Thief` with stats and inventory
  - Inventory (`std::vector<Weapon*>`), equipped weapon, stats management
- `Enemy` and subclasses:
  - `ArcherGoblin`, `MeleeGoblin`, `KingGoblin` with name, health, damage, distance
- `Weapon` and subclasses (Bow, Sword, Staff): damage and range values

## Configuration & Tweaks
- Inventory size: `#define INVENTORY_SIZE 5` in `main.cpp`
- Starting weapons and class stats: set in class initialization paths in `main.cpp`
- Narrative and UI text: printed from `main.cpp` (search for `std::cout` lines)

## Known Limitations
- Single-file gameplay logic (most implementations reside in `main.cpp`)
- No save/load system
- Minimal error handling beyond input validation for menu choices

## Future Improvements
- Save/load game state
- Additional enemy types and areas
- Balance tuning for damage and ranges
- Unit tests and CI for builds

## Attribution
Created as a learning project for turn-based console game mechanics, inventory management, and simple object-oriented design in C++.
