#include "rook.h"
using namespace std;

Rook::Rook(Colour c, unique_ptr<Player>& p) : RestrictedPiece(c, p, PieceType::Rook) {}

string Rook::getStringType() const {
    if (getColour() == Colour::White) return "R";
    else return "r";
}

vector<shared_ptr<Square>> Rook::findMoves(const unique_ptr<Board>& b, 
    const unique_ptr<Player>& next) const
{
    int row = getLocation().getRow();
    char column = getLocation().getColumn();
    shared_ptr<Square> location = b->getSquarePointer(row, column);
    vector<shared_ptr<Square>> moves;
    // Get vertical moves
    vector<shared_ptr<Square>> vertical = b->verticalMoves(*location);
    for (auto& move : vertical) moves.emplace_back(move);
    // Get horizontal moves
    vector<shared_ptr<Square>> horizontal = b->horizontalMoves(*location);
    for (auto& move : horizontal) moves.emplace_back(move);
    
    return moves;
}
