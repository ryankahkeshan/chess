#include "king.h"
#include "rook.h"
using namespace std;

King::King(Colour c, unique_ptr<Player>& p) : RestrictedPiece(c, p, PieceType::King) {}

string King::getStringType() const {
    if (getColour() == Colour::White) return "K";
    else return "k";
}

vector<shared_ptr<Square>> King::findMoves(const unique_ptr<Board>& b,
    const unique_ptr<Player>& next) const 
{
    int row = getLocation().getRow();
    char column = getLocation().getColumn();
    shared_ptr<Square> location = b->getSquarePointer(row, column);
    unique_ptr<Player>& owner = getOwner();

    vector<shared_ptr<Square>> moves;
    // Get vertical moves
    vector<shared_ptr<Square>> vertical = b->verticalMoves(*location, 1);
    for (auto& move : vertical) {
        shared_ptr<Piece> temp = location->getOccupant();
        location->removeOccupant();
        if (!next->attacksSquare(*move, b, owner)) moves.emplace_back(move);
        location->setOccupant(temp);
    } 
    // Get horizontal moves
    vector<shared_ptr<Square>> horizontal = b->horizontalMoves(*location, 1);
    for (auto& move : horizontal) {
        shared_ptr<Piece> temp = location->getOccupant();
        location->removeOccupant();
        if (!next->attacksSquare(*move, b, owner)) moves.emplace_back(move);
        location->setOccupant(temp);
    }
    // Get diagonal moves
    vector<shared_ptr<Square>> diagonal = b->diagonalMoves(*location, 1);
    for (auto& move : diagonal) {
        shared_ptr<Piece> temp = location->getOccupant();
        location->removeOccupant();
        if (!next->attacksSquare(*move, b, owner)) moves.emplace_back(move);
        location->setOccupant(temp);
    }
    // Castling
    // White king
    if (getColour() == Colour::White && row == 1 && column == 'e' && !hasMoved() &&
        !next->attacksSquare(b->getSquare("e1"), b, next)) {   
        shared_ptr<Piece> a1Piece = b->getSquare("a1").getOccupant();
        shared_ptr<Piece> h1Piece = b->getSquare("h1").getOccupant();
        // King side
        if (h1Piece && h1Piece->getType() == PieceType::Rook &&
            !dynamic_pointer_cast<Rook>(h1Piece)->hasMoved() &&
            b->isPathEmpty(b->getSquare("e1"), b->getSquare("h1")) &&
            !next->attacksSquare(b->getSquare("f1"), b, next) &&
            !next->attacksSquare(b->getSquare("g1"), b, next)) {
                moves.emplace_back(make_shared<Square>(b->getSquare("g1")));
        }
        // Queen side
        if (a1Piece && a1Piece->getType() == PieceType::Rook &&
            !dynamic_pointer_cast<Rook>(a1Piece)->hasMoved() &&
            b->isPathEmpty(b->getSquare("e1"), b->getSquare("a1")) &&
            !next->attacksSquare(b->getSquare("d1"), b, next) &&
            !next->attacksSquare(b->getSquare("c1"), b, next)) {
                moves.emplace_back(make_shared<Square>(b->getSquare("c1")));
        }
    } else if (getColour() == Colour::Black && row == 8 && column == 'e' && !hasMoved() &&
            !next->attacksSquare(b->getSquare("e8"), b, next)) {
        shared_ptr<Piece> a8Piece = b->getSquare("a8").getOccupant();
        shared_ptr<Piece> h8Piece = b->getSquare("h8").getOccupant();
        // King side
        if (h8Piece && h8Piece->getType() == PieceType::Rook &&
            !dynamic_pointer_cast<Rook>(h8Piece)->hasMoved() &&
            b->isPathEmpty(b->getSquare("e8"), b->getSquare("h8")) &&
            !next->attacksSquare(b->getSquare("f8"), b, next) &&
            !next->attacksSquare(b->getSquare("g8"), b, next)) {
                moves.emplace_back(make_shared<Square>(b->getSquare("g8")));
        }
        // Queen side
        if (a8Piece && a8Piece->getType() == PieceType::Rook &&
            !dynamic_pointer_cast<Rook>(a8Piece)->hasMoved() &&
            b->isPathEmpty(b->getSquare("e8"), b->getSquare("a8")) &&
            !next->attacksSquare(b->getSquare("d8"), b, next) &&
            !next->attacksSquare(b->getSquare("c8"), b, next)) {
                moves.emplace_back(make_shared<Square>(b->getSquare("c8")));
        }
    }
    return moves;
}

void King::move(Square& s1, Square& s2, const unique_ptr<Player>& curr,
                const unique_ptr<Player>& next, unique_ptr<Board>& b) {
    // Move pieces
    auto s2Piece = s2.getOccupant();
    if (s2Piece != nullptr) {  // Remove piece on s2 if needed
        next->removePiece(s2Piece);  // Remove piece from player's pieces
        s2Piece->removeLocation();  // Remove location of piece
        s2.removeOccupant(true);    // Remove piece from square (fully removing piece)
    }
    // If castling
    if (curr->getColour() == Colour::White) {   // White castling
        // King side
        if (!hasMoved() && s1.getColumn() == 'e' && s1.getRow() == 1 &&
            s2.getColumn() == 'g' && s2.getRow() == 1) {
            // Get rook & move-to square
            auto& rookSquare = b->getSquare(1, 'h');
            auto rook = dynamic_pointer_cast<Rook>(rookSquare.getOccupant());
            if (rook->hasMoved()) return;
            auto& moveTo = b->getSquare(1, 'f');
            moveTo.setOccupant(rook, true);
            rook->setLocation(make_shared<Square>(moveTo));
            rook->setMoved();
            rookSquare.removeOccupant(true);
        } else if (!hasMoved() && s1.getColumn() == 'e' && s1.getRow() == 1 &&
            s2.getColumn() == 'c' && s2.getRow() == 1) {
            // Get rook & move-to square
            auto& rookSquare = b->getSquare(1, 'a');
            auto rook = dynamic_pointer_cast<Rook>(rookSquare.getOccupant());
            if (rook->hasMoved()) return;
            auto& moveTo = b->getSquare(1, 'd');
            moveTo.setOccupant(rook, true);
            rook->setLocation(make_shared<Square>(moveTo));
            rook->setMoved();
            rookSquare.removeOccupant(true);
        }
    } else if (curr->getColour() == Colour::Black) {   // Black castling
        // King side
        if (!hasMoved() && s1.getColumn() == 'e' && s1.getRow() == 8 &&
            s2.getColumn() == 'g' && s2.getRow() == 8) {
            // Get rook & move-to square
            auto& rookSquare = b->getSquare(8, 'h');
            auto rook = dynamic_pointer_cast<Rook>(rookSquare.getOccupant());
            if (rook->hasMoved()) return;
            auto& moveTo = b->getSquare(8, 'f');
            moveTo.setOccupant(rook, true);
            rook->setLocation(make_shared<Square>(moveTo));
            rook->setMoved();
            rookSquare.removeOccupant(true);
        } else if (!hasMoved() && s1.getColumn() == 'e' && s1.getRow() == 8 &&
            s2.getColumn() == 'c' && s2.getRow() == 8) {
            // Get rook & move-to square
            auto& rookSquare = b->getSquare(8, 'a');
            auto rook = dynamic_pointer_cast<Rook>(rookSquare.getOccupant());
            if (rook->hasMoved()) return;
            auto& moveTo = b->getSquare(8, 'd');
            moveTo.setOccupant(rook, true);
            rook->setLocation(make_shared<Square>(moveTo));
            rook->setMoved();
            rookSquare.removeOccupant(true);
        }
    }
    
    // Update location
    s1.getOccupant()->setLocation(make_shared<Square>(s2));
    // Move the piece
    s2.setOccupant(s1.getOccupant(), true);
    s1.removeOccupant(true);
    setMoved();
}
