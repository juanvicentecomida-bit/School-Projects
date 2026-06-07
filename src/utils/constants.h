#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace GameConstants {
    const int BOARD_ROWS = 20;
    const int BOARD_COLS = 10;
    const int FRAME_DELAY_MS = 50; 
    const int INITIAL_DROP_SPEED = 30;  
    const int DROP_SPEED_DECREASE = 3;  
    const int MIN_DROP_SPEED = 3;  
    const int LINES_PER_LEVEL = 10;
}

namespace ScoreConstants {
    const int SINGLE_LINE = 100;
    const int DOUBLE_LINE = 300;
    const int TRIPLE_LINE = 500;
    const int TETRIS_LINE = 800;
    const int SOFT_DROP_BONUS = 1; 
    const int HARD_DROP_BONUS = 2;  
}

#endif