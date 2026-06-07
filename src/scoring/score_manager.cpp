#include "score_manager.h"
#include "constants.h"
#include <algorithm>

ScoreManager::ScoreManager() : score(0), totalLines(0), level(1) {}

void ScoreManager::addLineScore(int linesCleared) {
    if (linesCleared > 0) {
        totalLines += linesCleared;
        int baseScore = getLineBaseScore(linesCleared);
        score += baseScore * level;
        updateLevel();
    }
}

void ScoreManager::addDropBonus(int cells, bool isHardDrop) {
    int bonus = isHardDrop ? ScoreConstants::HARD_DROP_BONUS : ScoreConstants::SOFT_DROP_BONUS;
    score += cells * bonus;
}

void ScoreManager::reset() {
    score = 0;
    totalLines = 0;
    level = 1;
}

void ScoreManager::setLevel(int startLevel) {
    level = std::max(1, std::min(startLevel, 10));
}

void ScoreManager::updateLevel() {
    level = std::min((totalLines / GameConstants::LINES_PER_LEVEL) + 1, 10);
}

int ScoreManager::getLineBaseScore(int linesCleared) const {
    switch (linesCleared) {
        case 1: return ScoreConstants::SINGLE_LINE;
        case 2: return ScoreConstants::DOUBLE_LINE;
        case 3: return ScoreConstants::TRIPLE_LINE;
        case 4: return ScoreConstants::TETRIS_LINE;
        default: return 0;
    }
}

int ScoreManager::calculateDropSpeed() const {
    int speed = GameConstants::INITIAL_DROP_SPEED - (level - 1) * GameConstants::DROP_SPEED_DECREASE;
    return std::max(speed, GameConstants::MIN_DROP_SPEED);
}