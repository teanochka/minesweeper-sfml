#pragma once

class Cell {
private:
    bool isFlagged;
    bool isMine;
    bool isOpen;
    int minesAround;
    int x;
    int y;

public:
    Cell(int posX, int posY, bool isMine, int minesAround);

    bool getIsFlagged();
    bool getIsMine();
    bool getIsOpen();
    int getMinesAround();
    int getX();
    int getY();

    void setFlag();
    void unflag();
    void setOpen();

};
