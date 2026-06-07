#include "input_handler.h"

#ifdef _WIN32
    #include <conio.h>
#endif

bool InputHandler::hasInput() {
#ifdef _WIN32
    return _kbhit();
#else
    return kbhit();
#endif
}

InputAction InputHandler::pollInput() {
#ifdef _WIN32
    if (!_kbhit()) return InputAction::NONE;
    char ch = _getch();
#else
    if (!kbhit()) return InputAction::NONE;
    char ch = getch();
#endif
    
    switch (ch) {
        case 'a': case 'A': return InputAction::MOVE_LEFT;
        case 'd': case 'D': return InputAction::MOVE_RIGHT;
        case 's': case 'S': return InputAction::SOFT_DROP;
        case ' ':           return InputAction::HARD_DROP;
        case 'w': case 'W': return InputAction::ROTATE_CW;
        case 'e': case 'E': return InputAction::ROTATE_CCW;
        case 'z': case 'Z': return InputAction::HOLD;
        case 'p': case 'P': return InputAction::PAUSE;
        case 'q': case 'Q': return InputAction::QUIT;
        default:            return InputAction::NONE;
    }
}

InputHandler::~InputHandler() {
#ifndef _WIN32
    restoreTerminal();
#endif
}