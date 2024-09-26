#ifndef __COMPUTER_H__
#define __COMPUTER_H__
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "player.h"
class Player;
class Board;
enum class Colour;

class Computer : public Player {
  protected:
  std::vector<std::string> promotionMoves;
  std::vector<std::pair<std::shared_ptr<Piece>, std::shared_ptr<Square>>>
    allMoves(const std::unique_ptr<Board>& b, const std::unique_ptr<Player>& next);
  int getRandomNumber(int min, int max);
  public:
    Computer(Colour c, Colour start);

    PlayerType getType() const override;
};

#endif
