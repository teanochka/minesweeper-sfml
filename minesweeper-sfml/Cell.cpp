#include "Cell.h"

Cell::Cell(unsigned int posX, unsigned int posY)
    : isFlagged(false), isMine(false), isOpen(false), minesAround(0), x(posX), y(posY) {}

Cell::Cell(unsigned int posX, unsigned int posY, bool isMine, int minesAround)
    : isFlagged(false), isMine(isMine), isOpen(false), minesAround(minesAround), x(posX), y(posY) {}

bool Cell::getIsFlagged() const {
    return isFlagged;
}

bool Cell::getIsMine() const {
    return isMine;
}

bool Cell::getIsOpen() const {
    return isOpen;
}

unsigned int Cell::getMinesAround() const {
    return minesAround;
}

int Cell::getX() const {
    return x;
}

int Cell::getY() const {
    return y;
}

void Cell::setFlag() {
    isFlagged = true;
}

void Cell::unflag() {
    isFlagged = false;
}

void Cell::setOpen() {
    isOpen = true;
}

Cell& Cell::operator=(const Cell& other) {
    isFlagged = other.isFlagged;
    isMine = other.isMine;
    isOpen = other.isOpen;
    minesAround = other.minesAround;
    x = other.x;
    y = other.y;

    return *this;
}