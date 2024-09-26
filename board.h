#ifndef __BOARD_H__
#define __BOARD_H__
#include <vector>
#include <memory>
#include <string>
#include "square.h"
#include "piece.h"
#include "game.h"
#include "textDisplay.h"
#include "player.h"
#include "constants.h"
#include "graphicalDisplay.h"
class Square;
class Piece;
class TextDisplay;
class GraphicalDisplay;

class Board {
    std::shared_ptr<TextDisplay> td;
    std::shared_ptr<GraphicalDisplay> gd;
    std::vector<std::vector<std::shared_ptr<Square>>> board;
    // Private methods
    Square& getAboveSquare(Square& s);
    Square& getBelowSquare(Square& s);
    Square& getLeftSquare(Square& s);
    Square& getRightSquare(Square& s);
    Square& getTopLeftSquare(Square& s);
    Square& getTopRightSquare(Square& s);
    Square& getBottomLeftSquare(Square& s);
    Square& getBottomRightSquare(Square& s);
  public:
    Board(); // Default constructor

    void clear(); // Empties the board of all pieces

    // Assigns all white pieces in board to p1, and all black pieces to p2
    void assignPieces(std::unique_ptr<Player>& p1, std::unique_ptr<Player>& p2);

    // Returns true if there is exactly one king of each colour and no pawns on
    //  the first or last row
    bool isValidBoard();

    // Returns true if there are no pieces in a straight line from s1 to s2
    bool isPathEmpty(Square& s1, Square& s2) const;

     // Returns the square in the board corresponding to s, s must follow the
     //   formate of "[letter][number]" (ex. "e2") where letter is between 'a'
     //   and 'h' and number is between 1 and 8 (if BOARD_SIZE = 8)
    Square& getSquare(std::string s);
    Square& getSquare(int r, char c) const;

    std::shared_ptr<Square> getSquarePointer(std::string s);
    std::shared_ptr<Square> getSquarePointer(int r, char c);

    std::vector<std::shared_ptr<Square>> verticalMoves(Square& start, int limit = BOARD_HEIGHT, 
      bool upOnly = false, bool downOnly = false);
    std::vector<std::shared_ptr<Square>> horizontalMoves(Square& start, int limit = BOARD_HEIGHT);
    std::vector<std::shared_ptr<Square>> diagonalMoves(Square& start, int limit = BOARD_HEIGHT);
    std::vector<std::shared_ptr<Square>> knightMoves(Square& start);

    friend std::ostream &operator<<(std::ostream &out, const Board &b);
};

#endif
