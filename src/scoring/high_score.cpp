#include "high_score.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <sstream>
#include <cstring>

HighScoreManager::HighScoreManager(const std::string& path, int maxScores)
    : filepath(path), maxEntries(maxScores) {
}

std::string HighScoreManager::getCurrentDate() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    std::tm* tm_time = std::localtime(&time_t);
    
    std::stringstream ss;
    ss << std::put_time(tm_time, "%m-%d-%Y");
    return ss.str();
}

void HighScoreManager::sortScores() {
    std::sort(scores.begin(), scores.end(), 
              [](const ScoreEntry& a, const ScoreEntry& b) {
                  return a.score > b.score;
              });
    
    if (scores.size() > maxEntries) {
        scores.resize(maxEntries);
    }
}

bool HighScoreManager::loadScores() {
    scores.clear();
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return true;
    }
    
    std::string line;
    if (std::getline(file, line) && line.find("Name") != std::string::npos) {
    } else {
        file.clear();
        file.seekg(0);
    }
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        size_t pos1 = line.find('|');
        if (pos1 == std::string::npos) continue;
        
        size_t pos2 = line.find('|', pos1 + 1);
        if (pos2 == std::string::npos) continue;
        
        size_t pos3 = line.find('|', pos2 + 1);
        if (pos3 == std::string::npos) continue;
        
        size_t pos4 = line.find('|', pos3 + 1);
        if (pos4 == std::string::npos) continue;
        
        try {
            std::string name = line.substr(0, pos1);
            int score = std::stoi(line.substr(pos1 + 1, pos2 - pos1 - 1));
            int lines = std::stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
            int level = std::stoi(line.substr(pos3 + 1, pos4 - pos3 - 1));
            std::string date = line.substr(pos4 + 1);
            
            scores.push_back(ScoreEntry(name, score, lines, level, date));
        } catch (...) {
            continue;
        }
    }
    
    file.close();
    sortScores();
    return true;
}

bool HighScoreManager::saveScores() const {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not save high scores to " << filepath << std::endl;
        return false;
    }
    
    file << "Name|Score|Lines|Level|Date\n";
    
    for (const auto& entry : scores) {
        file << entry.playerName << "|"
             << entry.score << "|"
             << entry.lines << "|"
             << entry.level << "|"
             << entry.date << "\n";
    }
    
    file.close();
    return true;
}

bool HighScoreManager::addScore(const std::string& playerName, int score, int lines, int level) {
    std::string date = getCurrentDate();
    scores.push_back(ScoreEntry(playerName, score, lines, level, date));
    sortScores();
    return saveScores();
}

bool HighScoreManager::isHighScore(int score) const {
    if (scores.size() < maxEntries) {
        return true;
    }
    
    return score > scores.back().score;
}

int HighScoreManager::getHighestScore() const {
    if (scores.empty()) {
        return 0;
    }
    return scores[0].score;
}

void HighScoreManager::displayScores() const {
    std::cout << "\n";
    std::cout << "+=================================================================+\n";
    std::cout << "|                        HIGH SCORES                              |\n";
    std::cout << "+=================================================================+\n";
    std::cout << "| Rank  Name          Score    Lines  Level  Date                 |\n";
    std::cout << "+=================================================================+\n";
    
    if (scores.empty()) {
        std::cout << "|                     No scores yet!                          |\n";
    } else {
        for (size_t i = 0; i < scores.size(); i++) {
            const auto& entry = scores[i];
            std::cout << "| " << std::setw(4) << std::left << (i + 1) << "  ";
            std::cout << std::setw(12) << std::left << entry.playerName.substr(0, 12) << "  ";
            std::cout << std::setw(7) << std::right << entry.score << "  ";
            std::cout << std::setw(5) << std::right << entry.lines << "  ";
            std::cout << std::setw(5) << std::right << entry.level << "  ";
            std::cout << std::setw(10) << std::left << entry.date << " |\n";
        }
    }
    
    std::cout << "+=================================================================+\n";
    std::cout << "\n";
}