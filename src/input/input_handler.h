#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

enum class InputAction {
    NONE,
    MOVE_LEFT,
    MOVE_RIGHT,
    SOFT_DROP,
    HARD_DROP,
    ROTATE_CW,
    ROTATE_CCW,
    HOLD,
    PAUSE,
    QUIT
    
};

class InputHandler {
public:
    ~InputHandler();
    InputAction pollInput();
    bool hasInput();
};

#endif