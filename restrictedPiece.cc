#include "restrictedPiece.h"
#include <memory>
using namespace std;

RestrictedPiece::RestrictedPiece(Colour c, unique_ptr<Player>& p, PieceType pt) 
    : Piece(c, p, pt) {}

bool RestrictedPiece::hasMoved() const { return moved; }

void RestrictedPiece::setMoved() { moved = true; }

void RestrictedPiece::move(Square& s1, Square& s2, const unique_ptr<Player>& curr,
    const unique_ptr<Player>& next, unique_ptr<Board>& b) {
    // Move pieces
    auto s2Piece = s2.getOccupant();
    if (s2Piece != nullptr) {  // Remove piece on s2 if needed
        next->removePiece(s2Piece);  // Remove piece from player's pieces
        s2Piece->removeLocation();  // Remove location of piece
        s2.removeOccupant(true);    // Remove piece from square (fully removing piece)
    }
    // Update location
    s1.getOccupant()->setLocation(make_shared<Square>(s2));
    // Move the piece
    s2.setOccupant(s1.getOccupant(), true);
    s1.removeOccupant(true);
    setMoved();
}
