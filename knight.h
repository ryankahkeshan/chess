#ifndef __KNIGHT_H__
#define __KNIGHT_H__
#include "piece.h"

class Knight: public Piece {
  public:
    Knight(Colour c, std::unique_ptr<Player>& p);

    std::string getStringType() const override;

    std::vector<std::shared_ptr<Square>> findMoves(const std::unique_ptr<Board>& b,
      const std::unique_ptr<Player>& next) const override;
};

#endif
