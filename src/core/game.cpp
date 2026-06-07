#include "game.h"
#include "constants.h"
#include "wall_kicks.h"
#include "high_score.h"
#include <windows.h>
#include <ctime>
#include <iostream>
#include <conio.h>

Game::Game(int rows, int cols, int startLevel, unsigned int seed, bool useColor, bool showGhost) : 
    playfield(rows, cols),
    bag(seed),
    current(bag.getNext()),
    next(bag.getNext()),
    held(nullptr),
    scoreManager(),
    highScoreManager("scores.txt", 10),
    renderer(useColor, showGhost),
    inputHandler(),
    stateManager(),
    dropTimer() {
    
    scoreManager.setLevel(startLevel);
    dropTimer.setTarget(scoreManager.calculateDropSpeed());

    highScoreManager.loadScores();
}

Game::~Game() {
    if (held != nullptr) {
        delete held;
        held = nullptr;
    }
}

void Game::run() {
    std::cout << "\033[?25l" << std::flush;

    std::cout << "\033[2J" << std::flush;

    while (!stateManager.isGameOver()) {
        handleInput();
        update();
        render();
        Sleep(GameConstants::FRAME_DELAY_MS);
    }

    std::cout << "\033[?25h" << std::flush;

    handleGameOver();
}

void Game::handleInput() {
    InputAction action = inputHandler.pollInput();
    
    switch (action) {
        case InputAction::PAUSE:
            stateManager.togglePause();
            break;
        case InputAction::QUIT:
            stateManager.setState(GameState::GAME_OVER);
            break;
        case InputAction::MOVE_LEFT:
            if (stateManager.isPlaying() && playfield.isValidPosition(current, 0, -1))
                current.moveLeft();
            break;
        case InputAction::MOVE_RIGHT:
            if (stateManager.isPlaying() && playfield.isValidPosition(current, 0, 1))
                current.moveRight();
            break;
        case InputAction::SOFT_DROP:
            if (stateManager.isPlaying()) performSoftDrop();
            break;
        case InputAction::HARD_DROP:
            if (stateManager.isPlaying()) performHardDrop();
            break;
        case InputAction::ROTATE_CW:
            if (stateManager.isPlaying()) performRotationCW();
            break;
        case InputAction::ROTATE_CCW:
            if (stateManager.isPlaying()) performRotationCCW();
            break; 
        case InputAction::HOLD:
            if (stateManager.isPlaying()) performHold();
            break;
        default:
            break;
    }
}

void Game::update() {
    if (!stateManager.isPlaying()) return;
    
    dropTimer.tick();
    
    if (dropTimer.isReady()) {
        dropTimer.reset();
        
        if (playfield.isValidPosition(current, 1, 0)) {
            current.moveDown();
        } else {
            lockCurrentPiece();
        }
    }
}

void Game::render() {
    renderer.render(playfield, current, next, held, scoreManager, stateManager);
}

void Game::lockCurrentPiece() {
    playfield.placeTetromino(current);
    int linesCleared = playfield.clearFullLines();
    scoreManager.addLineScore(linesCleared);
    
    spawnNewPiece();
    dropTimer.setTarget(scoreManager.calculateDropSpeed());

    stateManager.resetHold();
}

void Game::spawnNewPiece() {
    current = next;
    next = Tetromino(bag.getNext());
    
    if (!playfield.isValidPosition(current, 0, 0)) {
        stateManager.setState(GameState::GAME_OVER);
    }
}

void Game::performSoftDrop() {
    if (playfield.isValidPosition(current, 1, 0)) {
        current.moveDown();
        scoreManager.addDropBonus(1, false);
    }
}

void Game::performHardDrop() {
    int cells = 0;
    while (playfield.isValidPosition(current, 1, 0)) {
        current.moveDown();
        cells++;
    }
    scoreManager.addDropBonus(cells, true);
    lockCurrentPiece();
}

bool Game::tryRotateWithKicks(Tetromino& testPiece, bool clockwise) {
    int originalRow = testPiece.getRow();
    int originalCol = testPiece.getCol();
    int originalRotation = testPiece.getRotationState();
    
    if (clockwise) {
        testPiece.rotateCW();
    } else {
        testPiece.rotateCCW();
    }
    
    int newRotation = testPiece.getRotationState();
    
    std::vector<KickOffset> kicks;
    TetrominoType type = testPiece.getType();
    
    if (type == TetrominoType::I) {
        kicks = WallKicks::getIPieceKicks(originalRotation, newRotation);
    } else if (type == TetrominoType::O) {
        kicks = WallKicks::getOPieceKicks(originalRotation, newRotation);
    } else {
        kicks = WallKicks::getNormalKicks(originalRotation, newRotation);
    }
    
    for (const auto& kick : kicks) {
        int testRow = originalRow + kick.row;
        int testCol = originalCol + kick.col;
        
        testPiece.setPosition(testRow, testCol);
        
        if (playfield.isValidPosition(testPiece, 0, 0)) {
            return true;
        }
    }
    
    testPiece.setPosition(originalRow, originalCol);
    
    if (clockwise) {
        testPiece.rotateCCW();
    } else {
        testPiece.rotateCW();
    }
    
    return false;
}

void Game::performRotationCW() {
    Tetromino testPiece = current;
    
    if (tryRotateWithKicks(testPiece, true)) {
        current = testPiece;
    }
}

void Game::performRotationCCW() {
    Tetromino testPiece = current;
    
    if (tryRotateWithKicks(testPiece, false)) {
        current = testPiece;
    }
}

void Game::performHold() {
    if (!stateManager.canUseHold()) {
        return;
    }
    
    if (held == nullptr) {
        held = new Tetromino(current.getType());
        current = next;
        next = Tetromino(bag.getNext());
    } else {
        TetrominoType heldType = held->getType();
        delete held;
        held = new Tetromino(current.getType());
        current = Tetromino(heldType);
    }
    
    current = Tetromino(current.getType());
    
    if (!playfield.isValidPosition(current, 0, 0)) {
        stateManager.setState(GameState::GAME_OVER);
        return;
    }
    
    stateManager.setCanHold(false);
}

void Game::handleGameOver() {
    int finalScore = scoreManager.getScore();
    int finalLines = scoreManager.getLines();
    int finalLevel = scoreManager.getLevel();
    
    std::cout << "\n";
    
    if (highScoreManager.isHighScore(finalScore)) {
        std::cout << "+=======================================+\n";
        std::cout << "|     CONGRATULATIONS! HIGH SCORE!      |\n";
        std::cout << "+=======================================+\n";
        std::cout << "\nYour score: " << finalScore << "\n";
        std::cout << "Previous best: " << highScoreManager.getHighestScore() << "\n\n";
        
        std::cout << "Enter your name (max 12 chars): ";
        std::string playerName;
        std::getline(std::cin, playerName);
        
        if (playerName.length() > 12) {
            playerName = playerName.substr(0, 12);
        }
        
        if (playerName.empty()) {
            playerName = "Anonymous";
        }
        
        if (highScoreManager.addScore(playerName, finalScore, finalLines, finalLevel)) {
            std::cout << "\nScore saved successfully!\n";
        } else {
            std::cout << "\nError saving score.\n";
        }
    }
    
    highScoreManager.displayScores();
}