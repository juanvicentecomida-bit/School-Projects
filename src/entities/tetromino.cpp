#include "tetromino.h"
#include <cstdlib>
#include <ctime>

Tetromino::Tetromino(TetrominoType t) : type(t), row(0), col(4), rotationState(0) {
    initializeShape();
}

void Tetromino::initializeShape() {
    switch (type) {
        case TetrominoType::I:
            shape = {{1, 1, 1, 1}};
            break;
        case TetrominoType::O:
            shape = {{1, 1}, {1, 1}};
            break;
        case TetrominoType::T:
            shape = {{0, 1, 0}, {1, 1, 1}};
            break;
        case TetrominoType::S:
            shape = {{0, 1, 1}, {1, 1, 0}};
            break;
        case TetrominoType::Z:
            shape = {{1, 1, 0}, {0, 1, 1}};
            break;
        case TetrominoType::J:
            shape = {{1, 0, 0}, {1, 1, 1}};
            break;
        case TetrominoType::L:
            shape = {{0, 0, 1}, {1, 1, 1}};
            break;
    }
}

void Tetromino::rotateCW() {
    if (type == TetrominoType::O) return;

    int n = shape.size();
    int m = shape[0].size();
    std::vector<std::vector<int>> rotated(m, std::vector<int>(n));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            rotated[j][n - 1 - i] = shape[i][j];

    shape = rotated;

    rotationState = (rotationState + 1) % 4;
}

void Tetromino::rotateCCW() {
    if (type == TetrominoType::O) return;

    int n = shape.size();
    int m = shape[0].size();
    std::vector<std::vector<int>> rotated(m, std::vector<int>(n));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            rotated[m - 1 - j][i] = shape[i][j];

    shape = rotated;

    rotationState = (rotationState + 3) % 4;
}

void Tetromino::moveDown()  { row++; }
void Tetromino::moveLeft()  { col--; }
void Tetromino::moveRight() { col++; }

const std::vector<std::vector<int>>& Tetromino::getShape() const { return shape; }
int Tetromino::getRow() const { return row; }
int Tetromino::getCol() const { return col; }
TetrominoType Tetromino::getType() const { return type; }

TetrominoType Tetromino::getRandomType() {
    return static_cast<TetrominoType>(rand() % 7);
}

TetrominoBag::TetrominoBag(unsigned int s) : seed(s) {
    if (seed == 0) {
        seed = static_cast<unsigned int>(time(0));
    }
    refillBag();
}

int TetrominoBag::customRand() {
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return seed;
}

void TetrominoBag::refillBag() {
    std::vector<TetrominoType> pieces = {
        TetrominoType::I, 
        TetrominoType::O, 
        TetrominoType::T,
        TetrominoType::S, 
        TetrominoType::Z, 
        TetrominoType::J,
        TetrominoType::L
    };
    
    for (int i = pieces.size() - 1; i > 0; i--) {
        int j = customRand() % (i + 1);
        std::swap(pieces[i], pieces[j]);
    }
    
    for (auto piece : pieces) {
        bag.push(piece);
    }
    
}

TetrominoType TetrominoBag::getNext() {
    if (bag.empty()) {
        refillBag();
    }
    TetrominoType next = bag.front();
    bag.pop();
    return next;
}