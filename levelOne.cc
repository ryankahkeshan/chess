#include "levelOne.h"
#include "computer.h"
#include <vector>
#include <string>
#include <memory>
using namespace std;

LevelOne::LevelOne(Colour c, Colour start) : Computer(c, start) {}

vector<string> LevelOne::cpuMove(const unique_ptr<Board>& b, const unique_ptr<Player>& next) {
    vector<pair<shared_ptr<Piece>, shared_ptr<Square>>> moves = allMoves(b, next);
    vector<string> output;
    // Get random move
    int idx = getRandomNumber(0, moves.size() - 1);
    
    shared_ptr<Piece> p = moves.at(idx).first;
    shared_ptr<Square> s2 = moves.at(idx).second;
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
