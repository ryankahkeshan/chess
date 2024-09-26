#ifndef __SQUARE_H__
#define __SQUARE_H__
#include <vector>
#include <memory>
#include "piece.h"
#include "observer.h"
#include "board.h"

class Piece;
enum class Colour;

class Square {
    int row;
    char column;
    Colour colour;
    std::shared_ptr<Piece> occupant = nullptr;
    std::vector<std::shared_ptr<Observer>> observers;
  public:
    Square(); // Default constructor

    // Returns the piece that occupies the square (nullptr if none)
    std::shared_ptr<Piece> getOccupant() const; 

    Colour getColour() const;  // Returns the colour of the square
    int getRow() const;  // Returns the row this is in
    char getColumn() const;  // Returns the column this is in
    bool isOccupied() const;  // Returns true if square is occupied, false otherwise
    
    void setOccupant(std::shared_ptr<Piece> p, bool notify = false);  // Sets occupant as p
    void removeOccupant(bool notify = false);  // Removes the squares occupant
    void setCoords(int r, char c);  // Sets the row and the column of this square
    void setColour(Colour col);  // Sets the colour of this square
    
    void attach(std::shared_ptr<Observer> obs);  // Attach obs to observers vector
    void notifyObservers();  // Alert observers that this square has changed
};

#endif
