#ifndef __PAWN_H__
#define __PAWN_H__
#include "restrictedPiece.h"

class Pawn: public RestrictedPiece {
    bool enPassantAvailable = false;
  public:
    Pawn(Colour c, std::unique_ptr<Player>& p);

    std::string getStringType() const override;
    
    bool canEnPassant();
    void falsifyEnPassant();

    void move(Square& s1, Square& s2, const std::unique_ptr<Player>& curr,
      const std::unique_ptr<Player>& next, std::unique_ptr<Board>& b) override;
    
    std::vector<std::shared_ptr<Square>> findMoves(const std::unique_ptr<Board>& b,
      const std::unique_ptr<Player>& next) const override;
};

#endif
