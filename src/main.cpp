#include "game.h"
#include "config_manager.h"
#include "high_score.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include <conio.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void printUsage() {
    std::cout << "Tetris Game - Usage:\n"
              << "  Tetris.exe [options]\n\n"
              << "Options:\n"
              << "  --config <path>  Path to config file (default: config/tetris.json)\n"
              << "  --rows <n>       Board rows (overrides config)\n"
              << "  --cols <n>       Board columns (overrides config)\n"
              << "  --level <n>      Starting level 1-10 (overrides config)\n"
              << "  --seed <n>       Random seed (overrides config, 0=random)\n"
              << "  --save-config    Save current settings to config file\n"
              << "  --help           Show this help\n\n"
              << "Controls (configurable in tetris.json):\n"
              << "  A/D = Move Left/Right\n"
              << "  W = Rotate Clockwise\n"
              << "  E = Rotate Counter-Clockwise\n"
              << "  S = Soft Drop\n"
              << "  SPACE = Hard Drop\n"
              << "  Z = Hold Piece\n"
              << "  P = Pause\n"
              << "  Q = Quit\n\n"
              << "Config file will be created with defaults if not found.\n";
}

int main(int argc, char* argv[]) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    std::string configPath = "config/tetris.json";
    bool saveConfig = false;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--config") == 0 && i + 1 < argc) {
            configPath = argv[++i];
        }
    }
    
    ConfigManager configManager(configPath);
    configManager.loadConfig();
    GameConfig& config = configManager.getConfig();
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            printUsage();
            return 0;
        } else if (strcmp(argv[i], "--high-scores") == 0) {
            HighScoreManager hsm("scores.txt", 10);
            hsm.loadScores();
            hsm.displayScores();
            std::cout << "Press any key to exit...";
            _getch();
            return 0;
        } else if (strcmp(argv[i], "--rows") == 0 && i + 1 < argc) {
            config.rows = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--cols") == 0 && i + 1 < argc) {
            config.cols = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--level") == 0 && i + 1 < argc) {
            config.startLevel = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--seed") == 0 && i + 1 < argc) {
            config.seed = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--save-config") == 0) {
            saveConfig = true;
        } else if (strcmp(argv[i], "--config") == 0 && i + 1 < argc) {
            i++;
        } else {
            std::cerr << "Unknown option: " << argv[i] << "\n";
            printUsage();
            return 1;
        }
    }
    
    if (!configManager.validateConfig()) {
        std::cerr << "Configuration validation failed. Using safe defaults.\n";
        config = GameConfig();
    }
    
    if (saveConfig) {
        if (configManager.saveConfig()) {
            std::cout << "Configuration saved successfully!" << std::endl;
        } else {
            std::cerr << "Failed to save configuration." << std::endl;
        }
        return 0;
    }
    
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int termCols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int termRows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    
    int minCols = config.cols * 2 + 25;
    int minRows = config.rows + 5;
    
    if (termRows < minRows || termCols < minCols) {
        std::cerr << "Warning: Terminal may be too small for optimal display.\n"
                  << "Current: " << termCols << "x" << termRows << "\n"
                  << "Recommended: " << minCols << "x" << minRows << "\n"
                  << "Continue anyway? (y/n): ";
        char answer;
        std::cin >> answer;
        if (answer != 'y' && answer != 'Y') {
            return 1;
        }
    }
    
    std::cout << "\n===========================================\n";
    std::cout << "         TETRIS - Console Edition\n";
    std::cout << "===========================================\n\n";
    configManager.printConfig();
    std::cout << "Press any key to start...\n";
    _getch();
    
    Game game(config.rows, config.cols, config.startLevel, config.seed, config.useColor, config.showGhost);
    game.run();
    
    std::cout << "\nPress any key to exit...";
    _getch();
    
    return 0;
}