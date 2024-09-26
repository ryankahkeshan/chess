#include <memory>
#include "piece.h"
using namespace std;

Piece::Piece(Colour c, unique_ptr<Player>& p, PieceType pt) 
    : colour{c}, type{pt}, owner{p} {}

Colour Piece::getColour() const { return colour; }

PieceType Piece::getType() const { return type; }

std::unique_ptr<Player>& Piece::getOwner() const { return owner; }

void Piece::setLocation(shared_ptr<Square> s) { location = s; }

Square& Piece::getLocation() const { return *location; }

void Piece::removeLocation() { location = nullptr; }

bool Piece::illegalMove(Square& s1, Square& s2, const unique_ptr<Player>& p) const {
    shared_ptr<Piece> s1Piece = s1.getOccupant();
    shared_ptr<Piece> s2Piece = s2.getOccupant();
    Colour playerColour = p->getColour();

    // Illegal moves:
    // s1 is s2
    if (s1.getRow() == s2.getRow() && s1.getColumn() == s2.getColumn()) return true;
    // s1 is not part of the board
    if (s1.getRow() < 1 || s1.getRow() > BOARD_HEIGHT || s1.getColumn() < 'a' ||
        s1.getColumn() > static_cast<char>('a' + BOARD_WIDTH - 1)) return true;
    // s2 is not part of the board
    if (s2.getRow() < 1 || s2.getRow() > BOARD_HEIGHT || s2.getColumn() < 'a' ||
        s2.getColumn() > static_cast<char>('a' + BOARD_WIDTH - 1)) return true;
    
    if (s1Piece == nullptr) return true;  // s1 is empty
    if (s1Piece->getColour() != playerColour) return true; // p doesn't own the piece on s1
    // p owns the piece on s2
    if (s2Piece != nullptr && s2Piece->getColour() == playerColour) return true;

    // Move is minimally legal
    return false;
}

bool Piece::canMove(Square& s1, Square& s2, const unique_ptr<Player>& p, 
    const unique_ptr<Player>& next, const unique_ptr<Board>& b) const {
    // // Check if illegal
    if (illegalMove(s1, s2, p)) return false;
    vector<shared_ptr<Square>> moves = findMoves(b, next);
    for (auto it = moves.begin(); it != moves.end(); it++) {
        // If move is even a possibility (disregarding checks)
        if ((*it)->getRow() == s2.getRow() && (*it)->getColumn() == s2.getColumn()) {
            // If the king isn't the one moving
            if (s1.getOccupant()->getType() != PieceType::King) {
                shared_ptr<Piece> s1Occ = s1.getOccupant(), s2Occ = s2.getOccupant();
                s1.removeOccupant();
                s2.removeOccupant();
                s2.setOccupant(s1Occ);
                if (s2Occ) next->removePiece(s2Occ);
                // If the king is not in check after the attempted move
                if (!next->attacksSquare(p->getKing()->getLocation(), b, p)) {
                    p->removedChecked();
                    s2.removeOccupant();
                    if (s2Occ) next->addPiece(s2Occ);
                    s1.setOccupant(s1Occ);
                    s2.setOccupant(s2Occ);
                    return true;
                } else {
                    s2.removeOccupant(); 
                    if (s2Occ) next->addPiece(s2Occ);
                    s1.setOccupant(s1Occ); 
                    s2.setOccupant(s2Occ); 
                    return false; 
                }
            } else if (!next->attacksSquare(s2, b, p)) {  // Moving king out of check
                p->removedChecked();
                return true;
            } else return false; // The move will put/keep the king in check
        }
    }
    return false;
}

//  bool Piece::canMove(Square& s1, Square& s2, const Player& cpu, 
//       const std::unique_ptr<Player>& next, const std::unique_ptr<Board>& b) const {
//         return true;
// }

void Piece::move(Square& s1, Square& s2, const unique_ptr<Player>& curr,
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
}

