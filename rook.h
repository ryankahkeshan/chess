#ifndef __ROOK_H__
#define __ROOK_H__
#include "restrictedPiece.h"

class Rook: public RestrictedPiece {
  public:
    Rook(Colour c, std::unique_ptr<Player>& p);

    std::string getStringType() const override;

    std::vector<std::shared_ptr<Square>> findMoves(const std::unique_ptr<Board>& b,
      const std::unique_ptr<Player>& next) const override;
};

#endif
