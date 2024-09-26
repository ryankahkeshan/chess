#include "pawn.h"
using namespace std;

Pawn::Pawn(Colour c, unique_ptr<Player>& p) : RestrictedPiece(c, p, PieceType::Pawn) {}

string Pawn::getStringType() const { 
    if (getColour() == Colour::White) return "P";
    else return "p";
}

bool Pawn::canEnPassant() { return enPassantAvailable; }

void Pawn::falsifyEnPassant() { enPassantAvailable = false; }

// moveEnPassant(s1, s2, curr, next, b) should only be used if the move s1 to s2 is
//   an en passant move. It will get rid of the piece that is getting taken
//   by en passant. The client has to then ensure the correct movement from s1 
//   to s2 of the pawn on s1
void moveEnPassant(Square& s1, Square& s2, const unique_ptr<Player>& curr,
    const unique_ptr<Player>& next, unique_ptr<Board>& b) 
{
    int row = s1.getRow();
    char column = s2.getColumn();
    // Remove piece from other player
    vector<shared_ptr<Piece>> pieces = next->getPieces();
    for (auto it = pieces.begin(); it != pieces.end(); it++) {
        if ((*it)->getLocation().getRow() == row && (*it)->getLocation().getColumn() == column) {
            int row = (*it)->getLocation().getRow();
            char column = (*it)->getLocation().getColumn();
            (*it)->removeLocation();
            b->getSquare(row, column).removeOccupant(true);
            next->removePiece(*it);
            break;
        }
    }
}

void Pawn::move(Square& s1, Square& s2, const unique_ptr<Player>& curr,
                const unique_ptr<Player>& next, unique_ptr<Board>& b) {
    // Move pieces
    auto s2Piece = s2.getOccupant();
    if (s2Piece != nullptr) {  // Remove piece on s2 if needed
        next->removePiece(s2Piece);  // Remove piece from player's pieces
        s2Piece->removeLocation();  // Remove location of piece
        s2.removeOccupant(true);    // Remove piece from square (fully removing piece)
    }
    // If en-passant
    if (s2Piece == nullptr && abs(s2.getColumn() - s1.getColumn()) == 1 &&
        abs(s2.getRow() - s1.getRow()) == 1) {
            moveEnPassant(s1, s2, curr, next, b);        
    }
    // Update location
    s1.getOccupant()->setLocation(make_shared<Square>(s2));
    // Move the piece
    s2.setOccupant(s1.getOccupant(), true);
    s1.removeOccupant(true);
    // Turn enPassant status to true
    if (!hasMoved() && abs(s2.getRow() - s1.getRow()) == 2) {
        enPassantAvailable = true;
    }
    setMoved();
}

// diagonalMoves(to, below, moves, c, row, enPassantRow) determines if a pawn of
//   colour c can move to the squares to (normal capture) or below (en passant)
//   while prividing its current row and the enPassantRow for its colour. If a
//   move is legal, a pointer to the square will be added to move
// effects: may modify moves
void diagonalMoves(Square& to, Square& below, vector<shared_ptr<Square>>& moves, 
    Colour c, int row, int enPassantRow)
{
    shared_ptr<Piece> toPiece = to.getOccupant();
    // Normal capture
    if (toPiece != nullptr && toPiece->getColour() != c) {
        moves.emplace_back(make_shared<Square>(to));
    }
    // En passant
    if (row == enPassantRow && toPiece == nullptr) {
        shared_ptr<Piece> belowPiece = below.getOccupant();
        if (belowPiece && belowPiece->getType() == PieceType::Pawn &&
        dynamic_pointer_cast<Pawn>(belowPiece)->canEnPassant()) {
            moves.emplace_back(make_shared<Square>(to));
        }
    }
}

vector<shared_ptr<Square>> Pawn::findMoves(const unique_ptr<Board>& b,
    const unique_ptr<Player>& next) const 
{
    vector<shared_ptr<Square>> moves;
    int row = getLocation().getRow();
    char column = getLocation().getColumn();
    shared_ptr<Square> location = b->getSquarePointer(row, column);
    if (getColour() == Colour::White) {  // If white
        // Straight line moves
        if (!hasMoved() && row == 2) {
            moves = b->verticalMoves(*location, 2, true);
        } else { moves = b->verticalMoves(*location, 1, true);}
        // Diagonal captues
        if (row < 8 && row > 1 && column >= 'b') {
            Square& to = b->getSquare(row + 1, static_cast<char>(column - 1));
            Square& below = b->getSquare(5, static_cast<char>(column - 1));
            diagonalMoves(to, below, moves, getColour(), row, 5);
        }
        if (row < 8 && row > 1 && column <= 'g') {
            Square& to = b->getSquare(row + 1, static_cast<char>(column + 1));
            Square& below = b->getSquare(5, static_cast<char>(column + 1));
            diagonalMoves(to, below, moves, getColour(), row, 5);
        }
    }
    if (getColour() == Colour::Black) {  // If black
        // Straight line moves
        if (!hasMoved() && row == 7) {
            moves = b->verticalMoves(*location, 2, false, true);
        } else moves = b->verticalMoves(*location, 1, false, true);
        // Diagonal captures
        if (row < 8 && row > 1 && column >= 'b') {
            Square& to = b->getSquare(row - 1, static_cast<char>(column - 1));
            Square& below = b->getSquare(4, static_cast<char>(column - 1));
            diagonalMoves(to, below, moves, getColour(), row, 4);
        }
        if (row < 8 && row > 1 && column <= 'g') {
            Square& to = b->getSquare(row - 1, static_cast<char>(column + 1));
            Square& below = b->getSquare(4, static_cast<char>(column + 1));
            diagonalMoves(to, below, moves, getColour(), row, 4);
        }
    }
    return moves;
}
