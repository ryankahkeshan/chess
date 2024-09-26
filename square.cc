#include "square.h"
using namespace std;

Square::Square() {}

shared_ptr<Piece> Square::getOccupant() const { return occupant; }

Colour Square::getColour() const { return colour; }

int Square::getRow() const { return row; }

char Square::getColumn() const { return column; }

bool Square::isOccupied() const { return occupant == nullptr ? false : true; }

void Square::setOccupant(shared_ptr<Piece> p, bool notify) {
    occupant = p; 
    if (notify) notifyObservers();
}

void Square::removeOccupant(bool notify) {
    occupant = nullptr;
    if (notify) notifyObservers();
}

void Square::setCoords(int r, char c) { this->row = r; this->column = c; }

void Square::setColour(Colour col) { this->colour = col; }

void Square::attach(shared_ptr<Observer> obs) { this->observers.emplace_back(obs); }

void Square::notifyObservers() {
    for (auto it = observers.begin(); it != observers.end(); it++) {
        (*it)->notify(*this);
    }
}

