#pragma once
#include <vector>
#include <SFML/Graphics.hpp>    
#include "Cell.h"

class Field {
private:
    int size;
    std::vector<std::vector<bool>> mineFlags;
    std::vector<std::vector<Cell>> cells;
    std::vector<std::pair<int, int>> mineCoordinates;

public:
    Field(int size);
    void clear();
    void generateField(int mines, int firstClickX, int firstClickY);
    void placeMines(int minesAmount, int firstClickX, int firstClickY);
    void setCellsInfo();
    void showOthers(int x, int y);
    Cell& getCell(int x, int y);
    void openCell(int x, int y, bool& lose);
    bool checkWinConditionFlags(int flags);
    bool checkWinConditionOpenCells();
    bool isInField(int x, int y);
    void render(sf::RenderWindow& window, int resolution, sf::Font font);

};
