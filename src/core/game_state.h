#ifndef GAME_STATE_H
#define GAME_STATE_H

enum class GameState { PLAYING, PAUSED, GAME_OVER };

class GameStateManager {
private:
    GameState currentState;
    bool canHold;

public:
    GameStateManager();
    
    void setState(GameState state);
    GameState getState() const;
    
    bool isPlaying() const;
    bool isPaused() const;
    bool isGameOver() const;
    
    void togglePause();

    bool canUseHold() const { return canHold; }
    void setCanHold(bool can) { canHold = can; }
    void resetHold() { canHold = true; }
};

#endif