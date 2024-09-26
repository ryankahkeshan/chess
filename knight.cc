#include "knight.h"
using namespace std;

Knight::Knight(Colour c, unique_ptr<Player>& p) : Piece(c, p, PieceType::Knight) {}

string Knight::getStringType() const {
    if (getColour() == Colour::White) return "N";
    else return "n";
}

vector<shared_ptr<Square>> Knight::findMoves(const unique_ptr<Board>& b,
    const unique_ptr<Player>& next) const 
{
    int row = getLocation().getRow();
    char column = getLocation().getColumn();
    shared_ptr<Square> location = b->getSquarePointer(row, column);

    vector<shared_ptr<Square>> moves = b->knightMoves(*location);
    return moves;
}
