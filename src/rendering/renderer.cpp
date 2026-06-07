#include "renderer.h"
#include "colors.h"
#include "ghost_piece.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <windows.h>

#define ANSI_CLEAR_SCREEN "\033[2J"
#define ANSI_CURSOR_HOME "\033[H"
#define ANSI_HIDE_CURSOR "\033[?25l"
#define ANSI_SHOW_CURSOR "\033[?25h"

Renderer::Renderer(bool enableColor, bool enableGhost) 
    : useColor(enableColor), showGhost(enableGhost) {}

void Renderer::hideCursor() {
    std::cout << ANSI_HIDE_CURSOR;
}

void Renderer::showCursor() {
    std::cout << ANSI_SHOW_CURSOR;
}

void Renderer::clearScreen() {
    std::cout << ANSI_CLEAR_SCREEN << std::flush;
}

void Renderer::repositionCursor() {
    std::cout << ANSI_CURSOR_HOME;
}

void Renderer::render(const Playfield& field,
                     const Tetromino& current,
                     const Tetromino& next,
                     const Tetromino* held,
                     const ScoreManager& scoreManager,
                     const GameStateManager& stateManager) {

    buffer.str("");
    buffer.clear();
    
    buffer << ANSI_CLEAR_SCREEN << ANSI_CURSOR_HOME;
    
    if (!stateManager.isGameOver()) {
        drawBoard(field, current, next, held, scoreManager);
        drawControls(stateManager);
    } else {
        drawGameOver(scoreManager);
    }

    std::cout << buffer.str() << std::flush;
}

void Renderer::drawBoard(const Playfield& field, const Tetromino& current, 
                        const Tetromino& next, const Tetromino* held, const ScoreManager& scoreManager) {
    const auto& shape = current.getShape();
    TetrominoType currentType = current.getType();
    
    for (int r = 0; r < field.getRows(); r++) {
        buffer << "|";
        
        for (int c = 0; c < field.getCols(); c++) {
            bool isCurrentPiece = false;
            bool isGhost = false;
            
            for (int i = 0; i < shape.size() && !isCurrentPiece; i++) {
                for (int j = 0; j < shape[i].size() && !isCurrentPiece; j++) {
                    if (shape[i][j] && current.getRow() + i == r && current.getCol() + j == c) {
                        isCurrentPiece = true;
                    }
                }
            }
            
            if (!isCurrentPiece && showGhost) {
                isGhost = GhostPiece::isGhostCell(current, field, r, c);
            }
            if (isCurrentPiece) {
                buffer << getColoredBlock(currentType, useColor);
            } else if (isGhost) {
                buffer << getGhostBlock(currentType, useColor);
            } else {
                int cellValue = field.getCell(r, c);
                if (cellValue >= 0) {
                    TetrominoType lockedType = static_cast<TetrominoType>(cellValue);
                    buffer << getColoredBlock(lockedType, useColor);
                } else {
                    buffer << "  ";
                }
            }
        }
        
        buffer << "|";
        
        if (r == 0) buffer << "  SCORE: " << scoreManager.getScore();
        else if (r == 1) buffer << "  LINES: " << scoreManager.getLines();
        else if (r == 2) buffer << "  LEVEL: " << scoreManager.getLevel();
        else if (r == 4) buffer << "  HOLD:";    
        else if (r >= 5 && r < 9) {
            int shapeRow = r - 5;
            
            if (held != nullptr) {
                const auto& heldShape = held->getShape();
                TetrominoType heldType = held->getType();
                buffer << "  ";
                for (int j = 0; j < 4; j++) {
                    if (shapeRow < heldShape.size() && j < heldShape[shapeRow].size() && heldShape[shapeRow][j])
                        buffer << getColoredBlock(heldType, useColor);
                    else
                        buffer << "  ";
                }
            } else {
                buffer << "  --------";
            }
        }

        else if (r == 10) buffer << "  NEXT:";
        else if (r >= 11 && r < 15) {
            int shapeRow = r - 11;
            const auto& nextShape = next.getShape();
            TetrominoType nextType = next.getType();
            buffer << "  ";
            for (int j = 0; j < 4; j++) {
                if (shapeRow < nextShape.size() && j < nextShape[shapeRow].size() && nextShape[shapeRow][j])
                    buffer << getColoredBlock(nextType, useColor);
                else
                    buffer << "  ";
            }
        }
        
        buffer << "\n";
    }
    
    buffer << "+";
    for (int i = 0; i < field.getCols() * 2; i++) buffer << "-";
    buffer << "+\n";
}

void Renderer::drawControls(const GameStateManager& stateManager) {
    buffer << "\n";

    if (stateManager.isPaused()) {
        buffer << "*** PAUSED ***                                      \n";
        buffer << "Press P to resume                                   \n";
        buffer << "                                                    \n";
    } else {
        buffer << "\nA/D=Left/Right | W=Rotate CW | E=Rotate CCW\n";
        buffer << "S=Soft Drop | SPACE=Hard Drop | P=Pause | Z=Hold\n";
        buffer << "           Q = Quit GAme               \n";
    }

    buffer << "                                                    \n";
    buffer << "                                                    \n";
    buffer << "                                                    \n";
}

void Renderer::drawGameOver(const ScoreManager& scoreManager) {
    buffer << ANSI_CLEAR_SCREEN << ANSI_CURSOR_HOME;

    buffer << "\n";
    buffer << "+=====================================+\n";
    buffer << "|                                     |\n";
    buffer << "|          G A M E   O V E R          |\n";
    buffer << "|                                     |\n";
    buffer << "+=====================================+\n";
    buffer << "|                                     |\n";
    
    buffer << "|  Final Score:      ";
    std::ostringstream scoreStr;
    scoreStr << std::setw(15) << std::left << scoreManager.getScore();
    buffer << scoreStr.str() << "  |\n";
    
    buffer << "|  Lines Cleared:    ";
    std::ostringstream linesStr;
    linesStr << std::setw(15) << std::left << scoreManager.getLines();
    buffer << linesStr.str() << "  |\n";
    
    buffer << "|  Final Level:      ";
    std::ostringstream levelStr;
    levelStr << std::setw(15) << std::left << scoreManager.getLevel();
    buffer << levelStr.str() << "  |\n";
    
    buffer << "|                                     |\n";
    buffer << "+=====================================+\n";
    
    std::string rating;
    int score = scoreManager.getScore();
    if (score >= 50000) rating = "LEGENDARY!";
    else if (score >= 30000) rating = "EXCELLENT!";
    else if (score >= 20000) rating = "GREAT!";
    else if (score >= 10000) rating = "GOOD!";
    else if (score >= 5000) rating = "Not Bad!";
    else rating = "Keep Trying!";
    
    buffer << "|  Performance:      ";
    std::ostringstream ratingStr;
    ratingStr << std::setw(15) << std::left << rating;
    buffer << ratingStr.str() << "  |\n";
    
    buffer << "|                                     |\n";
    buffer << "+=====================================+\n";
    buffer << "\n";
    buffer << "      Thanks for playing Tetris!\n";
    buffer << "\n";

    buffer << "                                                    \n";
    buffer << "                                                    \n";
    buffer << "                                                    \n";
}