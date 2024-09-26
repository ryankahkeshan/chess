#ifndef __KING_H__
#define __KING_H__
#include "restrictedPiece.h"
#include "board.h"

class King: public RestrictedPiece {
  public:
    King(Colour c, std::unique_ptr<Player>& p);

    std::string getStringType() const override;

    void move(Square& s1, Square& s2, const std::unique_ptr<Player>& curr,
      const std::unique_ptr<Player>& next, std::unique_ptr<Board>& b) override;

    std::vector<std::shared_ptr<Square>> findMoves(const std::unique_ptr<Board>& b,
      const std::unique_ptr<Player>& next) const override;
};

#endif
