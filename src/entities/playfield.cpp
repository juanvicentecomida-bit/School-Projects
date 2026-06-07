#include "playfield.h"

Playfield::Playfield(int r, int c) : rows(r), cols(c) {
    grid = std::vector<std::vector<int>>(rows, std::vector<int>(cols, -1));
}

void Playfield::reset() {
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            grid[r][c] = -1;
}

void Playfield::setCell(int r, int c, int value) {
    if (r >= 0 && r < rows && c >= 0 && c < cols)
        grid[r][c] = value;
}

int Playfield::getCell(int r, int c) const {
    if (r >= 0 && r < rows && c >= 0 && c < cols)
        return grid[r][c];
    return -1;
}

bool Playfield::isValidPosition(const Tetromino& t, int dRow, int dCol) const {
    const auto& shape = t.getShape();
    int r = t.getRow() + dRow;
    int c = t.getCol() + dCol;

    for (int i = 0; i < shape.size(); i++) {
        for (int j = 0; j < shape[i].size(); j++) {
            if (shape[i][j]) {
                int nr = r + i;
                int nc = c + j;
                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) return false;
                if (grid[nr][nc] != -1) return false;
            }
        }
    }
    return true;
}

void Playfield::placeTetromino(const Tetromino& t) {
    const auto& shape = t.getShape();
    int typeValue = static_cast<int>(t.getType());

    for (int i = 0; i < shape.size(); i++)
        for (int j = 0; j < shape[i].size(); j++)
            if (shape[i][j])
                grid[t.getRow() + i][t.getCol() + j] = typeValue;
}

int Playfield::clearFullLines() {
    int cleared = 0;
    for (int r = rows - 1; r >= 0; r--) {
        bool full = true;
        for (int c = 0; c < cols; c++) {
            if (grid[r][c] == -1) {
                full = false;
                break;
            }
        }

        if (full) {
            cleared++;
            for (int rr = r; rr > 0; rr--)
                for (int c = 0; c < cols; c++)
                    grid[rr][c] = grid[rr - 1][c];

            for (int c = 0; c < cols; c++)
                grid[0][c] = -1;
                
            r++;
        }
    }
    return cleared;
}