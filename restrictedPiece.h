#ifndef __RESTRICTED_PIECE_H__
#define __RESTRICTED_PIECE_H__
#include "piece.h"
#include "player.h"
#include "square.h"

class RestrictedPiece: public Piece {
    bool moved = false;
  public:
    RestrictedPiece(Colour c, std::unique_ptr<Player>& p, PieceType pt);

    bool hasMoved() const;  // True if the piece has moved, false otherwise
    void setMoved();  // Sets that piece has moved

    virtual void move(Square& s1, Square& s2, const std::unique_ptr<Player>& curr,
      const std::unique_ptr<Player>& next, std::unique_ptr<Board>& b) override;
};

#endif
