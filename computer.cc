#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "computer.h"
using namespace std;

Computer::Computer(Colour c, Colour start) : Player(c, start) {
    if (c == Colour::White) {
        promotionMoves.emplace_back("Q");
        promotionMoves.emplace_back("B");
        promotionMoves.emplace_back("N");
        promotionMoves.emplace_back("R");
    } else if (c == Colour::Black) {
        promotionMoves.emplace_back("q");
        promotionMoves.emplace_back("b");
        promotionMoves.emplace_back("n");
        promotionMoves.emplace_back("r");
    }
}

PlayerType Computer::getType() const { return PlayerType::Computer; }

vector<pair<shared_ptr<Piece>, shared_ptr<Square>>> Computer::allMoves(
    const unique_ptr<Board>& b, const unique_ptr<Player>& next) 
{
    vector<pair<shared_ptr<Piece>, shared_ptr<Square>>> moves;
    vector<shared_ptr<Piece>> pieces = getPieces();
    for (auto& piece : pieces) {
        unique_ptr<Player>& owner = piece->getOwner();
        vector<shared_ptr<Square>> squares = piece->findMoves(b, next);
        for (auto& square : squares) {
            int row = piece->getLocation().getRow();
            char column = piece->getLocation().getColumn();
            int row2 = square->getRow();
            char column2 = square->getColumn();
            if (piece->canMove(b->getSquare(row, column), b->getSquare(row2, column2), 
                owner, next, b)) {
                moves.emplace_back(piece, square);
            }
        }
    }
    return moves;
}

int Computer::getRandomNumber(int min, int max) {
    // Seed the random number generator with the current time
    srand(static_cast<unsigned>(time(nullptr)));
    // Generate a random number between min and max
    return min + rand() % (max - min + 1);
}
