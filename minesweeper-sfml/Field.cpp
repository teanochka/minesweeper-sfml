#include "Field.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <unordered_set>

Field::Field(int size) {
    this->size = size;

    mineFlags.resize(size, std::vector<bool>(size, false));
    cells.resize(size, std::vector<Cell>(size, Cell(0, 0, false, 0)));
}

void Field::generateField(int minesAmount, int firstClickX, int firstClickY) {
    placeMines(minesAmount, firstClickX, firstClickY);
    setCellsInfo();
}

void Field::placeMines(int minesAmount, int firstClickX, int firstClickY) {
    std::srand((int)(std::time(nullptr)));

    for (int i = 0; i < minesAmount; ++i) {
        int randomRow = std::rand() % size;
        int randomCol = std::rand() % size;

        while (mineFlags[randomRow][randomCol] || (randomRow == firstClickX && randomCol == firstClickY)) {
            randomRow = std::rand() % size;
            randomCol = std::rand() % size;
        }


        mineFlags[randomRow][randomCol] = true;
        mineCoordinates.push_back(std::make_pair(randomRow, randomCol));
    }
}

void Field::setCellsInfo() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int minesAround = 0;

            for (int a = -1; a <= 1; ++a) {
                for (int b = -1; b <= 1; ++b) {
                    if (a == 0 && b == 0) {
                        continue;
                    }

                    int newRow = i + a;
                    int newCol = j + b;

                    if (newRow >= 0 && newRow < size && newCol >= 0 && newCol < size) {
                        if (mineFlags[newRow][newCol]) {
                            minesAround++;
                        }
                    }
                }
            }
            cells[i][j] = Cell(i, j, mineFlags[i][j], minesAround);
        }
    }
}

void Field::clear() {
    mineFlags.clear();
    cells.clear();
    mineCoordinates.clear();

    mineFlags.resize(size, std::vector<bool>(size, false));
    cells.resize(size, std::vector<Cell>(size, Cell(0, 0, false, 0)));
}


void Field::showOthers(int x, int y) {
    if (x >= 0 && x < size && y >= 0 && y < size) {
        std::queue<std::pair<int, int>> queue;
        std::unordered_set<int> registry;

        queue.push({ x, y });
        registry.insert(x * size + y);

        std::vector<std::pair<int, int>> dirs = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} , {1, 1}, {-1, 1}, {1, -1}, {-1,-1} };

        while (!queue.empty()) {
            auto currentTile = queue.front();
            queue.pop();

            for (const auto& dir : dirs) {
                int xx = currentTile.first + dir.first;
                int yy = currentTile.second + dir.second;
                int index = xx * size + yy;

                if (registry.count(index) || xx < 0 || xx >= size || yy < 0 || yy >= size) {
                    continue;
                }

                if (!cells[xx][yy].getIsOpen() && cells[xx][yy].getMinesAround() == 0 && !cells[xx][yy].getIsMine()) {
                    if (!cells[xx][yy].getIsFlagged()) {
                        cells[xx][yy].setOpen();
                    }
                    registry.insert(index);
                    queue.push({ xx, yy });
                }
                else if (!cells[xx][yy].getIsOpen() && cells[xx][yy].getMinesAround() > 0 && !cells[xx][yy].getIsMine()) {
                    if (!cells[xx][yy].getIsFlagged()) {
                        cells[xx][yy].setOpen();
                    }
                    registry.insert(index);
                }
            }
        }
    }
}

bool Field::checkWinConditionFlags(int flags) {
    for (const auto& coordinates : mineCoordinates) {
        auto [x, y] = coordinates;
        if (!cells[x][y].getIsFlagged()) {
            return false;
        }
    }
    if (flags == 0) {
        return true;
    }
    else return false;
}

bool Field::checkWinConditionOpenCells() {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (!cells[i][j].getIsOpen() && !cells[i][j].getIsMine()) {
                return false;
            }
        }
    }
    return true;
}

bool Field::isInField(int x, int y) {
    if (x < size && y < size) {
        return true;
    }
    else return false;
}

void Field::render(sf::RenderWindow& window, int resolution, sf::Font font) {
    const float cellSize = static_cast<float>(resolution) / size;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setPosition(j * cellSize, i * cellSize);
            cell.setOutlineThickness(cellSize * 0.02);
            cell.setOutlineColor(sf::Color::White);

            if (cells[i][j].getIsOpen()) {
                if (cells[i][j].getIsMine()) {
                    cell.setFillColor(sf::Color::Black);
                    window.draw(cell);
                }
                else {
                    cell.setFillColor(sf::Color(197, 254, 254));
                    window.draw(cell);
                    if (cells[i][j].getMinesAround()!=0){
                        //sf::Font font;
                        //font.loadFromFile("assets/fonts/mine-sweeper.TTF");
                        sf::Text text(std::to_string(cells[i][j].getMinesAround()), font, cellSize);
                        text.setPosition(j * cellSize, i * cellSize);
                        text.setFillColor(sf::Color(126, 57, 182));
                        window.draw(text);
                    }
                }
            }
            else {
                cell.setFillColor(sf::Color(157, 137, 254));
                window.draw(cell);
                if (cells[i][j].getIsFlagged()) {
                    sf::Texture flagTexture; flagTexture.loadFromFile("assets/images/flag.jpg");
                    sf::Sprite flagSprite(flagTexture);
                    flagSprite.setScale(cellSize / flagTexture.getSize().x, cellSize / flagTexture.getSize().y);
                    flagSprite.setPosition(j * cellSize, i * cellSize);
                    window.draw(flagSprite);
                }
            }
        }
    }
}

Cell& Field::getCell(int x, int y)
{
    return cells[x][y];
}

void Field::openCell(int x, int y, bool& lose) {
    if (cells[x][y].getIsMine()) {
        lose = true;
    }
    else if (!cells[x][y].getIsOpen()) {
        cells[x][y].setOpen();
        if (!cells[x][y].getMinesAround()) {
            showOthers(x, y);
        }
    }
}


