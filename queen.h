#ifndef __QUEEN_H__
#define __QUEEN_H__
#include "piece.h"

class Queen: public Piece {
  public:
    Queen(Colour c, std::unique_ptr<Player>& p);

    std::string getStringType() const override;
    
    std::vector<std::shared_ptr<Square>> findMoves(const std::unique_ptr<Board>& b,
      const std::unique_ptr<Player>& next) const override;
};

#endif
