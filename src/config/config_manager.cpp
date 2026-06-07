#include "config_manager.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <direct.h>  
#include <sys/stat.h> 

using json = nlohmann::json;

bool createDirectory(const std::string& path) {
    struct stat info;
    if (stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR)) {
        return true; 
    }
    
#ifdef _WIN32
    return _mkdir(path.c_str()) == 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

std::string getDirectory(const std::string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos != std::string::npos) {
        return filepath.substr(0, pos);
    }
    return "";
}

GameConfig::GameConfig() :
    rows(20),
    cols(10),
    startLevel(1),
    seed(0),
    frameDelayMs(50),
    initialDropSpeed(48),
    dropSpeedDecrease(5),
    minDropSpeed(3),
    linesPerLevel(10),
    scoreSingle(100),
    scoreDouble(300),
    scoreTriple(500),
    scoreTetris(800),
    softDropBonus(1),
    hardDropBonus(2),
    keyLeft('a'),
    keyRight('d'),
    keyRotateCW('w'),
    keyRotateCCW('e'),
    keySoftDrop('s'),
    keyHardDrop(' '),
    keyPause('p'),
    keyQuit('q'),
    showGhost(false),
    useColor(false)
{}

ConfigManager::ConfigManager(const std::string& path) : configPath(path) {
    config = GameConfig(); // Initialize with defaults
}

bool ConfigManager::loadConfig() {
    std::ifstream file(configPath);
    
    if (!file.is_open()) {
        std::cout << "Config file not found at: " << configPath << std::endl;
        std::cout << "Creating default configuration..." << std::endl;
        return createDefaultConfig();
    }
    
    try {
        json j;
        file >> j;
        
        // Load board settings
        if (j.contains("board")) {
            config.rows = j["board"].value("rows", 20);
            config.cols = j["board"].value("cols", 10);
        }
        
        // Load gameplay settings
        if (j.contains("gameplay")) {
            config.startLevel = j["gameplay"].value("startLevel", 1);
            config.seed = j["gameplay"].value("seed", 0);
        }
        
        // Load timing settings
        if (j.contains("timing")) {
            config.frameDelayMs = j["timing"].value("frameDelayMs", 50);
            config.initialDropSpeed = j["timing"].value("initialDropSpeed", 48);
            config.dropSpeedDecrease = j["timing"].value("dropSpeedDecrease", 5);
            config.minDropSpeed = j["timing"].value("minDropSpeed", 3);
            config.linesPerLevel = j["timing"].value("linesPerLevel", 10);
        }
        
        // Load scoring settings
        if (j.contains("scoring")) {
            config.scoreSingle = j["scoring"].value("single", 100);
            config.scoreDouble = j["scoring"].value("double", 300);
            config.scoreTriple = j["scoring"].value("triple", 500);
            config.scoreTetris = j["scoring"].value("tetris", 800);
            config.softDropBonus = j["scoring"].value("softDropBonus", 1);
            config.hardDropBonus = j["scoring"].value("hardDropBonus", 2);
        }
        
        // Load controls
        if (j.contains("controls")) {
            std::string left = j["controls"].value("left", "a");
            std::string right = j["controls"].value("right", "d");
            std::string rotateCW = j["controls"].value("rotateCW", "w");
            std::string rotateCCW = j["controls"].value("rotateCCW", "e");
            std::string softDrop = j["controls"].value("softDrop", "s");
            std::string hardDrop = j["controls"].value("hardDrop", " ");
            std::string pause = j["controls"].value("pause", "p");
            std::string quit = j["controls"].value("quit", "q");
            
            config.keyLeft = left[0];
            config.keyRight = right[0];
            config.keyRotateCW = rotateCW[0];
            config.keyRotateCCW = rotateCCW[0];
            config.keySoftDrop = softDrop[0];
            config.keyHardDrop = hardDrop[0];
            config.keyPause = pause[0];
            config.keyQuit = quit[0];
        }
        
        // Load display settings
        if (j.contains("display")) {
            config.showGhost = j["display"].value("showGhost", false);
            config.useColor = j["display"].value("useColor", false);
        }
        
        std::cout << "Configuration loaded successfully from: " << configPath << std::endl;
        
        if (!validateConfig()) {
            std::cerr << "Warning: Configuration contains invalid values!" << std::endl;
            return false;
        }
        
        return true;
        
    } catch (const json::exception& e) {
        std::cerr << "Error parsing config file: " << e.what() << std::endl;
        std::cerr << "Using default configuration instead." << std::endl;
        config = GameConfig();
        return false;
    }
}

bool ConfigManager::saveConfig() const {
    // Create config directory if it doesn't exist
    std::string dir = getDirectory(configPath);
    if (!dir.empty()) {
        createDirectory(dir);
    }
    
    json j;
    
    // Board settings
    j["board"]["rows"] = config.rows;
    j["board"]["cols"] = config.cols;
    
    // Gameplay settings
    j["gameplay"]["startLevel"] = config.startLevel;
    j["gameplay"]["seed"] = config.seed;
    
    // Timing settings
    j["timing"]["frameDelayMs"] = config.frameDelayMs;
    j["timing"]["initialDropSpeed"] = config.initialDropSpeed;
    j["timing"]["dropSpeedDecrease"] = config.dropSpeedDecrease;
    j["timing"]["minDropSpeed"] = config.minDropSpeed;
    j["timing"]["linesPerLevel"] = config.linesPerLevel;
    
    // Scoring settings
    j["scoring"]["single"] = config.scoreSingle;
    j["scoring"]["double"] = config.scoreDouble;
    j["scoring"]["triple"] = config.scoreTriple;
    j["scoring"]["tetris"] = config.scoreTetris;
    j["scoring"]["softDropBonus"] = config.softDropBonus;
    j["scoring"]["hardDropBonus"] = config.hardDropBonus;
    
    // Controls
    j["controls"]["left"] = std::string(1, config.keyLeft);
    j["controls"]["right"] = std::string(1, config.keyRight);
    j["controls"]["rotateCW"] = std::string(1, config.keyRotateCW);
    j["controls"]["rotateCCW"] = std::string(1, config.keyRotateCCW);
    j["controls"]["softDrop"] = std::string(1, config.keySoftDrop);
    j["controls"]["hardDrop"] = std::string(1, config.keyHardDrop);
    j["controls"]["pause"] = std::string(1, config.keyPause);
    j["controls"]["quit"] = std::string(1, config.keyQuit);
    
    // Display settings
    j["display"]["showGhost"] = config.showGhost;
    j["display"]["useColor"] = config.useColor;
    
    // Comments (stored as _comment keys, ignored by parser)
    j["_comment"] = "Tetris Configuration File - Edit values as needed";
    j["board"]["_comment"] = "Board dimensions (rows: 10-30, cols: 8-20)";
    j["gameplay"]["_comment"] = "Start level (1-10), seed (0=random)";
    j["timing"]["_comment"] = "Frame delay in ms, drop speeds in frames";
    j["scoring"]["_comment"] = "Base scores and drop bonuses";
    j["controls"]["_comment"] = "Single character keys for controls";
    j["display"]["_comment"] = "Visual features (ghost piece, colors)";
    
    std::ofstream file(configPath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not create config file at: " << configPath << std::endl;
        return false;
    }
    
    file << j.dump(4); // Pretty print with 4-space indent
    std::cout << "Configuration saved to: " << configPath << std::endl;
    return true;
}

bool ConfigManager::createDefaultConfig() const {
    std::cout << "Creating default config at: " << configPath << std::endl;
    return saveConfig();
}

void ConfigManager::setBoardSize(int rows, int cols) {
    config.rows = rows;
    config.cols = cols;
}

void ConfigManager::setStartLevel(int level) {
    config.startLevel = level;
}

void ConfigManager::setSeed(unsigned int seed) {
    config.seed = seed;
}

bool ConfigManager::validateConfig() const {
    bool valid = true;
    
    if (config.rows < 10 || config.rows > 30) {
        std::cerr << "Invalid rows: " << config.rows << " (must be 10-30)" << std::endl;
        valid = false;
    }
    
    if (config.cols < 8 || config.cols > 20) {
        std::cerr << "Invalid cols: " << config.cols << " (must be 8-20)" << std::endl;
        valid = false;
    }
    
    if (config.startLevel < 1 || config.startLevel > 10) {
        std::cerr << "Invalid start level: " << config.startLevel << " (must be 1-10)" << std::endl;
        valid = false;
    }
    
    if (config.frameDelayMs < 10 || config.frameDelayMs > 200) {
        std::cerr << "Invalid frame delay: " << config.frameDelayMs << " (must be 10-200ms)" << std::endl;
        valid = false;
    }
    
    if (config.minDropSpeed < 1 || config.minDropSpeed > config.initialDropSpeed) {
        std::cerr << "Invalid drop speeds" << std::endl;
        valid = false;
    }
    
    return valid;
}

void ConfigManager::printConfig() const {
    std::cout << "\n=== Current Configuration ===" << std::endl;
    std::cout << "Board: " << config.cols << "x" << config.rows << std::endl;
    std::cout << "Start Level: " << config.startLevel << std::endl;
    std::cout << "Seed: " << (config.seed == 0 ? "Random" : std::to_string(config.seed)) << std::endl;
    std::cout << "Frame Delay: " << config.frameDelayMs << "ms" << std::endl;
    std::cout << "Ghost Piece: " << (config.showGhost ? "Enabled" : "Disabled") << std::endl;
    std::cout << "Colors: " << (config.useColor ? "Enabled" : "Disabled") << std::endl;
    std::cout << "============================\n" << std::endl;
}