#ifndef __HUMAN_H__
#define __HUMAN_H__
#include <vector>
#include <string>
#include "player.h"
class Player;

class Human: public Player {
  public:
    Human(Colour c, Colour start);

    PlayerType getType() const override;

    std::vector<std::string> cpuMove(const std::unique_ptr<Board>& b,
      const std::unique_ptr<Player>& next);
};

#endif
