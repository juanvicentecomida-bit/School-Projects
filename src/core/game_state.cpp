#include "game_state.h"

GameStateManager::GameStateManager() : currentState(GameState::PLAYING), canHold(true) {}

void GameStateManager::setState(GameState state) {
    currentState = state;
}

GameState GameStateManager::getState() const {
    return currentState;
}

bool GameStateManager::isPlaying() const {
    return currentState == GameState::PLAYING;
}

bool GameStateManager::isPaused() const {
    return currentState == GameState::PAUSED;
}

bool GameStateManager::isGameOver() const {
    return currentState == GameState::GAME_OVER;
}

void GameStateManager::togglePause() {
    if (currentState == GameState::PLAYING)
        currentState = GameState::PAUSED;
    else if (currentState == GameState::PAUSED)
        currentState = GameState::PLAYING;
}