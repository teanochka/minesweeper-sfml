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
    Field(int size, int mines, unsigned int firstClickX, unsigned int firstClickY);
    void placeMines(int minesAmount, unsigned int firstClickX, unsigned int firstClickY);
    void setCellsInfo();
    void showOthers(unsigned int x, unsigned int y);
    Cell& getCell(unsigned int x, unsigned int y);
    void openCell(unsigned int x, unsigned int y);
    void toggleFlag(unsigned int x, unsigned int y);
    bool checkWinCondition();
    bool isInField(unsigned int x, unsigned int y);
    void render(sf::RenderWindow& window, int resolution, sf::Font font);
    Field& operator=(const Field& other);
};
