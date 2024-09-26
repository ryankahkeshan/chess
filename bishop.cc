#include "bishop.h"
using namespace std;

Bishop::Bishop(Colour c, unique_ptr<Player>& p) : Piece(c, p, PieceType::Bishop) {}

string Bishop::getStringType() const {
    if (getColour() == Colour::White) return "B";
    else return "b";
}

vector<shared_ptr<Square>> Bishop::findMoves(const unique_ptr<Board>& b,
    const unique_ptr<Player>& next) const 
{
    int row = getLocation().getRow();
    char column = getLocation().getColumn();
    shared_ptr<Square> location = b->getSquarePointer(row, column);
    vector<shared_ptr<Square>> moves = b->diagonalMoves(*location);
    return moves;
}
