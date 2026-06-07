#ifndef COLORS_H
#define COLORS_H

#include <string>
#include "tetromino.h"

namespace Colors {

    const std::string RESET   = "\033[0m";
    const std::string BLACK   = "\033[30m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
    const std::string WHITE   = "\033[37m";
    
    const std::string BRIGHT_BLACK   = "\033[90m";
    const std::string BRIGHT_RED     = "\033[91m";
    const std::string BRIGHT_GREEN   = "\033[92m";
    const std::string BRIGHT_YELLOW  = "\033[93m";
    const std::string BRIGHT_BLUE    = "\033[94m";
    const std::string BRIGHT_MAGENTA = "\033[95m";
    const std::string BRIGHT_CYAN    = "\033[96m";
    const std::string BRIGHT_WHITE   = "\033[97m";
    
    const std::string BG_BLACK   = "\033[40m";
    const std::string BG_RED     = "\033[41m";
    const std::string BG_GREEN   = "\033[42m";
    const std::string BG_YELLOW  = "\033[43m";
    const std::string BG_BLUE    = "\033[44m";
    const std::string BG_MAGENTA = "\033[45m";
    const std::string BG_CYAN    = "\033[46m";
    const std::string BG_WHITE   = "\033[47m";
    
    const std::string BOLD      = "\033[1m";
    const std::string DIM       = "\033[2m";
    const std::string UNDERLINE = "\033[4m";
    const std::string BLINK     = "\033[5m";
    const std::string REVERSE   = "\033[7m";

    const std::string GHOST = "\033[2m\033[37m";
}

inline std::string getTetrominoColor(TetrominoType type) {
    switch (type) {
        case TetrominoType::I: return Colors::BRIGHT_CYAN;   
        case TetrominoType::O: return Colors::BRIGHT_YELLOW;  
        case TetrominoType::T: return Colors::BRIGHT_MAGENTA; 
        case TetrominoType::S: return Colors::BRIGHT_GREEN;   
        case TetrominoType::Z: return Colors::BRIGHT_RED;     
        case TetrominoType::J: return Colors::BRIGHT_BLUE;   
        case TetrominoType::L: return Colors::YELLOW;        
        default: return Colors::WHITE;
    }
}

inline std::string getColoredBlock(TetrominoType type, bool useColor = true) {
    if (!useColor) {
        return "[]";
    }
    return getTetrominoColor(type) + "[]" + Colors::RESET;
}

inline std::string getGhostBlock(TetrominoType type, bool useColor = true) {
    if (!useColor) {
        return "··"; 
    }
    return Colors::GHOST + "[]" + Colors::RESET;
}

#endif