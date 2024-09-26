#include <vector>
#include <string>
#include "human.h"
using namespace std;

Human::Human(Colour c, Colour start) : Player(c, start) {}

PlayerType Human::getType() const { return PlayerType::Human; }

vector<string> Human::cpuMove(const std::unique_ptr<Board>& b,
      const std::unique_ptr<Player>& next) {
        vector<string> output;
        return output;
}
