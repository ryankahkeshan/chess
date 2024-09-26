#ifndef __PIECE_H__
#define __PIECE_H__
#include <vector>
#include "game.h"
#include "square.h"
#include "player.h"
#include "board.h"
class Square;
class Board;
class Player;
enum class Colour;

enum class PieceType { King, Queen, Rook, Bishop, Knight, Pawn};

class Piece {
    Colour colour;
    PieceType type;
    std::shared_ptr<Square> location;
    std::unique_ptr<Player>& owner;
  public:
    Piece(Colour c, std::unique_ptr<Player>& p, PieceType pt);

    Colour getColour() const;  // Returns the colour of the piece
    PieceType getType() const;  // Returns the type of the piece
    // Return the string type of the piece (ex: "Q" for a white queen, "b" for a black bishop)
    virtual std::string getStringType() const = 0;
    // Gets the player owner of the piece
    std::unique_ptr<Player>& getOwner() const;

    void setLocation(std::shared_ptr<Square> s);  // Sets the pieces location
    Square& getLocation() const; // Retrieves the location
    void removeLocation();  // Removes the pieces location

    // Returns true if the move is illegal, that is:
    //  - s1 is s2
    //  - s1 or s2 isn't on the board
    //  - there's no piece on s1
    //  - s1 is not owned by p
    //  - s2 is owned by p
    bool illegalMove(Square& s1, Square& s2, const std::unique_ptr<Player>& p) const;

    // Determines if the move is a possible move given the piece's type
    bool canMove(Square& s1, Square& s2, const std::unique_ptr<Player>& p, 
      const std::unique_ptr<Player>& next, const std::unique_ptr<Board>& b) const;

    // Moves piece from s1 to s2
    virtual void move(Square& s1, Square& s2, const std::unique_ptr<Player>& curr,
      const std::unique_ptr<Player>& next, std::unique_ptr<Board>& b);

    // Finds all possible moves for the piece
    virtual std::vector<std::shared_ptr<Square>> findMoves(const std::unique_ptr<Board>& b,
      const std::unique_ptr<Player>& next) const = 0;
};

#endif
