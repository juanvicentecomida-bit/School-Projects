Tetris - Console Edition (C++)

Course: CC104 - Data Structures and Algorithm
Group: Monozoid  
Platform: Windows (Console/ASCII)  
Language: C++14  

---

Table of Contents
1. Build & Run Instructions
2. Game Controls
3. Configuration Options
4. Architecture Overview
5. Data Structures & Complexity Analysis
6. Scoring System
7. Features
8. Known Issues & Limitations
9. Testing Environment

---

Build & Run Instructions

Prerequisites
- Operating System:Windows 10 or later
- Compiler: MinGW with C++14 support OR Visual Studio 2015+
- CMake: Version 3.10 or higher
- Terminal: Windows Command Prompt or PowerShell (minimum size: 40×25)

Build Steps

1. Navigate to project directory
cd Tetris

2. Create build directory
mkdir build
cd build

3. Generate build files
cmake ..

4. Compile
cmake --build .

5. Run the game
cd Debug
.\Tetris.exe

Quick Run Commands

Basic gameplay
.\Tetris.exe

Start at specific level
.\Tetris.exe --level 5

Use deterministic seed for testing
.\Tetris.exe --seed 12345

Custom board size
.\Tetris.exe --rows 15 --cols 12

View high scores
.\Tetris.exe --high-scores

Show all options
.\Tetris.exe --help

---

Game Controls

 Key    Description
 A - Move piece one column left
 D - Move piece one column right
 W - Rotate piece 90° clockwise 
 E - Rotate piece 90° counter-clockwise 
 S - Faster descent (+1 point per cell) 
 SPACE - Instant placement (+2 points per cell) 
 Z - Save current piece (once per drop)
 P - Pause or resume gameplay
 Q - Exit to game over screen

Note: All keys are case-insensitive (A = a, W = w, etc.)

---

Configuration Options

Command-Line Arguments

 Option | Default | Valid Range |
 `--rows <n>` | 20 | 10-30 |
 `--cols <n>` | 10 | 8-20 |
 `--level <n>` | 1 | 1-10 |
 `--seed <n>`  | 0 | 0-2³¹ |
 `--config <path>` | config/tetris.json | Any valid path |
 `--save-config` - Save current settings 
 `--high-scores` - Display leaderboard 
 `--help` - Show help message

Configuration File (config/tetris.json)

The game supports runtime configuration via JSON:

{
  "board": { "rows": 20, "cols": 10 },
  "gameplay": { "startLevel": 1, "seed": 0 },
  "timing": {
    "frameDelayMs": 50,
    "initialDropSpeed": 30,
    "dropSpeedDecrease": 3,
    "minDropSpeed": 3
  },
  "scoring": {
    "single": 100, "double": 300,
    "triple": 500, "tetris": 800,
    "softDropBonus": 1, "hardDropBonus": 2
  },
  "controls": {
    "left": "a", "right": "d",
    "rotateCW": "w", "rotateCCW": "e",
    "softDrop": "s", "hardDrop": " ",
    "hold": "z", "pause": "p", "quit": "q"
  },
  "display": { "showGhost": true, "useColor": true }
}

---

Architecture Overview

Component Structure

Game (Main Controller)
├── Playfield (10×20 grid state)
├── TetrominoBag (Fair piece randomization)
├── Tetromino (Current/Next/Held pieces)
├── ScoreManager (Score, lines, level tracking)
├── HighScoreManager (Persistent leaderboard)
├── GameStateManager (PLAYING/PAUSED/GAME_OVER)
├── InputHandler (Non-blocking keyboard input)
├── Renderer (Flicker-free ASCII display)
└── Timer (Frame-based gravity system)

State Machine

        ┌─────────┐
    ┌──→│ PLAYING │←──┐
    │   └─────────┘   │
    │        │        │
    │ (P key)│        │ (P key)
    │        ↓        │
    │   ┌────────┐    │
    └───│ PAUSED │────┘
        └────────┘
             │
          (Q key)
             ↓
        ┌──────────┐
        │GAME_OVER │
        └──────────┘
             │
         (top-out)

Design Principles

1. Separation of Concerns - Each module has single responsibility
2. Encapsulation - Game state protected by clear interfaces
3. Modularity - Components independently testable
4. Determinism - Seeded RNG for reproducible gameplay

---

Data Structures & Complexity Analysis

1. Playfield
-Structure: `vector<vector<int>>` (R×C matrix)
-Cell Storage: Integer encoding (0-6 for piece types, -1 for empty)
-Operations:
  -Cell access: O(1)
  -Line detection: O(C) per row, O(R×C) worst case
  -Row compaction: O(R×C) per clear event
-Space: O(R×C) where R=20, C=10 (default: 200 integers)
-Rationale: 2D vector provides O(1) random access needed for collision checks and rendering

2. Tetromino Representation
-Structure: `vector<vector<int>>` for shape matrix
-Rotation States: Stored implicitly (regenerated on rotation)
-Operations:
  -Rotation: O(k) where k=max dimension (≤4) = O(1)
  -Collision check: O(4) = O(1) (always 4 blocks)
-Space: O(16) worst case (4×4 bounding box)
-Rationale: Matrix representation simplifies rotation algorithm (transpose + reverse)

3. TetrominoBag (Fair Randomization)
-Structure: `queue<TetrominoType>`
-Algorithm: Fisher-Yates shuffle of all 7 pieces
-Operations:
  -Get next piece: Amortized O(1)
  -Bag refill: O(7 log 7) for shuffle ≈ O(1) (constant)
- Space: O(7) per bag
- Rationale: Ensures all 7 pieces appear once before any repeat (prevents droughts)

4. Score & Level Management
-Operations:
  -Score update: O(1)
  -Level calculation: O(1)
  -Drop speed lookup: O(1)
-Space: O(1) (3 integers)

5. High Score Persistence
-Structure: `vector<ScoreEntry>` + file I/O
-Operations:
  -Load scores: O(n) where n=number of scores (max 10)
  -Add score: O(n log n) for sorting
  -Save scores: O(n)
-Space: O(10) (fixed max entries)
-Rationale: File-based persistence survives game restarts; small dataset allows simple sorting

6. Rendering
-Operations:
  -Full frame render: O(R×C) per frame
  -Ghost piece calculation: O(R) (drop until collision)
  -HUD rendering: O(1)
- Rationale: Buffered output reduces flicker; cursor repositioning avoids full screen clear

7. Input Handling
-Method: Non-blocking with `_kbhit()` and `_getch()`
-Operations: O(1) per frame
-Rationale: Polling-based input doesn't block game loop

Overall Game Loop Complexity
-Per Tick: O(R×C)** dominated by rendering
-Typical Performance: 20 FPS stable on modern hardware

---

Scoring System

Line Clear Base Scores
Lines Cleared | Points
 1 | 100 × level |
 2 | 300 × level |
 3 | 500 × level |
 4 | 800 × level |

Example: Clearing 4 lines at Level 5 = 800 × 5 = 4,000 points

Drop Bonuses
Action | Points per Cell
Soft Drop | +1 
Hard Drop | +2 

Example: Hard dropping 15 cells = 15 × 2 = 30 bonus points

Level Progression
-Level Up: Every 10 lines cleared
-Maximum Level: 10 (speed caps at level 10)
-Drop Speed: Decreases from 30 frames (1.5s) to 3 frames (0.15s)

| Level | Frames/Drop | Approx Time | Difficulty |
|-------|-------------|-------------|------------|
| 1 | 30 | 1.5s | Beginner |
| 3 | 24 | 1.2s | Intermediate |
| 5 | 18 | 0.9s | Advanced |
| 7 | 12 | 0.6s | Expert |
| 10 | 3 | 0.15s | Master |

---

Features

Core Features (Required)
-All 7 standard tetromino pieces (I, O, T, S, Z, J, L)  
-10×20 playfield (configurable 10-30 rows, 8-20 cols)  
-Rotation system (CW/CCW with wall kicks)  
-Fair bag randomization (no droughts)  
-Deterministic seeding for reproducible games  
-Line clearing with row compaction  
-Progressive difficulty (10 levels)  
-Scoring system with level multipliers  
-Soft/hard drop with bonuses  
-Non-blocking input  
-Fixed 20 FPS tick rate  
-Pause/resume functionality  
-Game over detection (top-out)  

Advanced Features (Bonus)
-Ghost Piece - Shows landing position preview  
-Hold Piece - Save piece for later (Z key, once per drop)  
-Wall Kicks - Advanced rotation near walls/blocks  
-ANSI Colors - Colored pieces (configurable)  
-High Score System - Top 10 persistent leaderboard  
-Flicker-Free Rendering - Buffered output with cursor repositioning  
-JSON Configuration - Runtime settings via config file  

---

Known Issues & Limitations

Platform Limitations
-Windows Only: Uses `<conio.h>` and `<windows.h>` for input/timing
-Not Portable: Will not compile on Linux/macOS without modification
-Future Work: Cross-platform support using termios (Linux) and ncurses

Terminal Requirements
-Minimum Size: 40 columns × 25 rows
-Font Recommendation: Consolas, Cascadia Code, or similar monospace font
-ANSI Support: Requires Windows 10+ for color support

Gameplay Limitations
-No Extended Lock Delay: Piece locks immediately when can't move down
-Basic Wall Kicks: Simple kick patterns (not full SRS specification)
-Single Next Piece: Only shows 1 upcoming piece (not multiple)

Known Bugs
-None currently identified in core functionality
-Color support may not work on very old Windows terminals

---

Testing Environment

Successfully Tested On
-OS: Windows 11 (24H2)
-Compiler: MinGW-w64 8.1.0, Visual Studio 2022
-Terminal: Windows Terminal 1.15+, Command Prompt
-Board Sizes: 10×20 (default), 15×25, 20×30

Deterministic Testing
Use seeds for reproducible gameplay:
Test 1: Early game behavior
.\Tetris.exe --seed 12345

Test 2: Mid-game difficulty
.\Tetris.exe --seed 67890 --level 5

Test 3: High-speed play
.\Tetris.exe --seed 99999 --level 10

Performance
-Frame Rate: Stable 20 FPS on AMD Ryzen 3 or equivalent
-Memory Usage: < 5 MB (efficient for console game)
-CPU Usage: < 2% during normal gameplay

---

Project Structure

Tetris/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── design.pdf              # Design brief with diagrams
├── RUN.txt                 # Quick start commands
├── config/
│   └── tetris.json         # Runtime configuration
├── include/
│   └── json.hpp            # JSON library (nlohmann)
├── src/
│   ├── main.cpp
│   ├── core/               # Game loop & state management
│   │   ├── game.h/cpp
│   │   └── game_state.h/cpp
│   ├── entities/           # Game objects
│   │   ├── tetromino.h/cpp
│   │   └── playfield.h/cpp
│   ├── input/              # Keyboard handling
│   │   └── input_handler.h/cpp
│   ├── scoring/            # Score & level management
│   │   ├── score_manager.h/cpp
│   │   └── high_score.h/cpp
│   ├── rendering/          # Display & graphics
│   │   └── renderer.h/cpp
│   ├── config/             # Configuration loader
│   │   └── config_manager.h/cpp
│   └── utils/              # Helper utilities
│       ├── constants.h
│       ├── timer.h
│       ├── colors.h
│       ├── wall_kicks.h
│       └── ghost_piece.h
├── tests/
│   ├── test_matrix.md      # Test documentation
│   ├── seeded_run_1.log
│   ├── seeded_run_2.log
│   └── seeded_run_3.log
├── screenshots/
│   ├── gameplay.png
│   ├── line_clear.png
│   ├── tetris.png
│   ├── level_up.png
│   └── game_over.png
└── build/                  # Generated build files

---

Learning Outcomes Achieved

1. Grid-based puzzle modeling with clean abstractions
2. Data structure selection for playfield, pieces, queues
3. Algorithm implementation for collision, rotation, line clearing
4. Modular engineering with stable timing and non-blocking input
5. Complexity analysis of all major operations
6. State management with clear invariants
7. File I/O for configuration and persistence
8. Real-time systems with fixed frame rate

---

References

- Original Tetris: Alexey Pajitnov (1985)
- Tetris Guideline: https://tetris.wiki/
- SRS (Super Rotation System): https://tetris.wiki/SRS
- JSON for Modern C++: https://github.com/nlohmann/json

---

Credits

Group:
Monozoid

Course: CC104 - Data Structures and Algorithms    
Instructor: Ralfh Edwin Panti 
Submission Date: December 2025

---

Game Version: 1.0  
Development Time: 4 weeks

---