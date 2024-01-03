#pragma once

class Cell {
private:
    bool isFlagged;
    bool isMine;
    bool isOpen;
    int minesAround;
    unsigned int x;
    unsigned int y;

public:
    Cell(unsigned int posX, unsigned int posY);
    Cell(unsigned int posX, unsigned int posY, bool isMine, int minesAround);

    bool getIsFlagged() const;
    bool getIsMine() const;
    bool getIsOpen() const;
    unsigned int getMinesAround() const;
    int getX() const;
    int getY() const;

    void setFlag();
    void unflag();
    void setOpen();

    Cell& operator=(const Cell& other);
};
