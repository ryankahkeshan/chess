#include "levelTwo.h"
#include <vector>
#include <memory>
using namespace std;

LevelTwo::LevelTwo(Colour c, Colour start) : Computer(c, start) {}

vector<string> LevelTwo::cpuMove(const unique_ptr<Board>& b, const unique_ptr<Player>& next) {
    vector<pair<shared_ptr<Piece>, shared_ptr<Square>>> moves = allMoves(b, next);
    vector<string> output;
    vector<pair<shared_ptr<Piece>, shared_ptr<Square>>> attackingMoves;
    // Add all attacking moves to attackingMoves
    for (auto it = moves.begin(); it != moves.end(); it++) {
        if ((*it).second->getOccupant() != nullptr) { // Attacking move
            attackingMoves.emplace_back(*it);
        }
    }
    // Get random move
    shared_ptr<Piece> p;
    shared_ptr<Square> s2;
    if (attackingMoves.size() > 0) {    // Attacking moves exist
        int idx = getRandomNumber(0, attackingMoves.size() - 1);
        p = attackingMoves.at(idx).first;
        s2 = moves.at(idx).second;
    } else {    // No attacking moves
        int idx = getRandomNumber(0, moves.size() - 1);
        p = moves.at(idx).first;
        s2 = moves.at(idx).second;
    }
    Square &s1 = b->getSquare(p->getLocation().getRow(), p->getLocation().getColumn());

    // Get s1 string
    string sOne {s1.getColumn()};
    sOne += to_string(s1.getRow());
    output.emplace_back(sOne);

    // Get s2 string
    string sTwo {s2->getColumn()};
    sTwo  += to_string(s2->getRow());
    output.emplace_back(sTwo);

    // Check if promotion
    if (getColour() == Colour::White) {
        if (s1.getOccupant()->getType() == PieceType::Pawn &&
            s2->getRow() == 8 && s1.getRow() == 7) {
            int promotionIdx = getRandomNumber(0, promotionMoves.size() - 1);
            string p = promotionMoves.at(promotionIdx);
            output.emplace_back(p);
        }
    } else if (getColour() == Colour::Black) {
        if (s1.getOccupant()->getType() == PieceType::Pawn &&
            s2->getRow() == 1 && s1.getRow() == 1) {
            int promotionIdx = getRandomNumber(0, promotionMoves.size() - 1);
            string p = promotionMoves.at(promotionIdx);
            output.emplace_back(p);
        }
    }
    return output;
}
