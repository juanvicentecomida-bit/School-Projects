#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>

struct GameConfig {
    // Board settings
    int rows;
    int cols;
    
    // Gameplay settings
    int startLevel;
    unsigned int seed;
    
    // Timing settings
    int frameDelayMs;
    int initialDropSpeed;
    int dropSpeedDecrease;
    int minDropSpeed;
    int linesPerLevel;
    
    // Scoring settings
    int scoreSingle;
    int scoreDouble;
    int scoreTriple;
    int scoreTetris;
    int softDropBonus;
    int hardDropBonus;
    
    // Controls (for future expansion)
    char keyLeft;
    char keyRight;
    char keyRotateCW;
    char keyRotateCCW;
    char keySoftDrop;
    char keyHardDrop;
    char keyPause;
    char keyQuit;
    
    // Display settings
    bool showGhost;  // For future ghost piece feature
    bool useColor;   // For future color feature
    
    // Constructor with defaults
    GameConfig();
};

class ConfigManager {
private:
    std::string configPath;
    GameConfig config;
    
public:
    ConfigManager(const std::string& path = "config/tetris.json");
    
    // Load/Save configuration
    bool loadConfig();
    bool saveConfig() const;
    
    // Create default config file if doesn't exist
    bool createDefaultConfig() const;
    
    // Getters
    const GameConfig& getConfig() const { return config; }
    GameConfig& getConfig() { return config; }
    
    // Update specific settings
    void setBoardSize(int rows, int cols);
    void setStartLevel(int level);
    void setSeed(unsigned int seed);
    
    // Validation
    bool validateConfig() const;
    
    // Print current config (for debugging)
    void printConfig() const;
};

#endif