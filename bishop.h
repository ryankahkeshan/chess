#ifndef __BISHOP_H__
#define __BISHOP_H__
#include <memory>
#include "piece.h"

class Bishop: public Piece {
  public:
    Bishop(Colour c, std::unique_ptr<Player>& p);

    std::string getStringType() const override;

    std::vector<std::shared_ptr<Square>> findMoves(const std::unique_ptr<Board>& b,
      const std::unique_ptr<Player>& next) const override;
};

#endif
