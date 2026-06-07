#ifndef RENDERER_H
#define RENDERER_H
#include "playfield.h"
#include "tetromino.h"
#include "score_manager.h"
#include "game_state.h"
#include "colors.h"
#include <sstream>

class Renderer {
private:
    std::ostringstream buffer;
    bool useColor;
    bool showGhost;

public:
    Renderer(bool enableColor = true, bool enableGhost = false);

    void render(const Playfield& field, 
                const Tetromino& current,
                const Tetromino& next,
                const Tetromino* held,
                const ScoreManager& scoreManager,
                const GameStateManager& stateManager);

    void setUseColor(bool enable) { useColor = enable; }
    void setShowGhost(bool enable) { showGhost = enable; }

private:
    void clearScreen();
    void repositionCursor();
    void hideCursor();
    void showCursor();
    void drawBoard(const Playfield& field, const Tetromino& current, 
                   const Tetromino& next,const Tetromino* held, 
                   const ScoreManager& scoreManager);
    void drawStats(const ScoreManager& scoreManager, const Tetromino& next, const Tetromino* held, int row);
    void drawControls(const GameStateManager& stateManager);
    void drawGameOver(const ScoreManager& ScoreManager);

    void bufferLine(const std::string& line);
};

#endif