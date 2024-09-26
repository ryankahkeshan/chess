#include "levelFour.h"
#include <vector>
#include <memory>
using namespace std;

LevelFour::LevelFour(Colour c, Colour start) : Computer(c, start) {}

vector<string> LevelFour::cpuMove(const unique_ptr<Board>& b, const unique_ptr<Player>& next) {
    vector<pair<shared_ptr<Piece>, shared_ptr<Square>>> moves = allMoves(b, next);
    vector<string> output;
    return output;
}
