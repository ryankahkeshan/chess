#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <vector>
#include <memory>
#include "game.h"
#include "piece.h"
#include "board.h"
#include "square.h"
class Piece;
enum class Colour;
class Board;
class Square;

enum class PlayerType { Human, Computer };

class Player {
    Colour colour;
    std::vector<std::shared_ptr<Piece>> pieces;
    std::shared_ptr<Piece> king;
    bool inCheck = false;
    bool isMate = false;
    bool turn;
    bool resigned = false;
  public:
    Player(Colour c, Colour start);  // Constructor

    Colour getColour() const;
    virtual PlayerType getType() const = 0;
    std::shared_ptr<Piece>& getKing();

    void resign();
    bool hasResigned();

    bool isChecked() const;
    void setChecked();
    void removedChecked();

    bool isMated() const;
    void setMate();
    
    bool isTurn() const;
    void updateTurn();

    void addPiece(std::shared_ptr<Piece> p);
    void removePiece(std::shared_ptr<Piece> p);
    std::vector<std::shared_ptr<Piece>>& getPieces();

    bool attacksSquare(Square& s, const std::unique_ptr<Board>& b, 
      const std::unique_ptr<Player>& opp) const;

    virtual std::vector<std::string> cpuMove(const std::unique_ptr<Board>& b,
        const std::unique_ptr<Player>& next) = 0;
};

#endif
