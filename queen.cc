#include "queen.h"
using namespace std;

Queen::Queen(Colour c, unique_ptr<Player>& p) : Piece(c, p, PieceType::Queen) {}

string Queen::getStringType() const {
    if (getColour() == Colour::White) return "Q";
    else return "q";
}

vector<shared_ptr<Square>> Queen::findMoves(const unique_ptr<Board>& b,
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
    // Get diagonal moves
    vector<shared_ptr<Square>> diagonal = b->diagonalMoves(*location);
    for (auto& move : diagonal) moves.emplace_back(move);

    return moves;
}

