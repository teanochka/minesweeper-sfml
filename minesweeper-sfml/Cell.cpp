#include "Cell.h"

Cell::Cell(int posX, int posY, bool isMine, int minesAround)
    : isFlagged(false), isMine(isMine), isOpen(false), minesAround(minesAround), x(posX), y(posY) {}

bool Cell::getIsFlagged() {
    return isFlagged;
}

bool Cell::getIsMine() {
    return isMine;
}

bool Cell::getIsOpen() {
    return isOpen;
}

int Cell::getMinesAround() {
    return minesAround;
}

int Cell::getX() {
    return x;
}

int Cell::getY() {
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
