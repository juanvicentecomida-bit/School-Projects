#ifndef GAME_H
#define GAME_H
#include "playfield.h"
#include "tetromino.h"
#include "score_manager.h"
#include "high_score.h"
#include "renderer.h"
#include "input_handler.h"
#include "timer.h"
#include "game_state.h"

class Game {
private:
    Playfield playfield;
    TetrominoBag bag;
    Tetromino current;
    Tetromino next;
    Tetromino* held;
    ScoreManager scoreManager;
    HighScoreManager highScoreManager;
    Renderer renderer;
    InputHandler inputHandler;
    GameStateManager stateManager;
    Timer dropTimer;

public:
    Game(int rows, int cols, int startLevel, unsigned int seed, bool useColor = true, bool showGhost = false);
    ~Game();
    void run();

    const Tetromino* getHeldPiece() const { return held; }

private:
    void handleInput();
    void update();
    void render();
    void lockCurrentPiece();
    void spawnNewPiece();
    void performSoftDrop();
    void performHardDrop();
    void performRotationCW();
    void performRotationCCW();
    void performHold();
    void handleGameOver();

    bool tryRotateWithKicks(Tetromino& testPiece, bool clockwise);

};

#endif