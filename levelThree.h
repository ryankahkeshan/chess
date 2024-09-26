#ifndef __LEVEL_THREE_H__
#define __LEVEL_THREE_H__
#include "computer.h"

class LevelThree : public Computer {
  public:
    LevelThree(Colour c, Colour start); // Constructor
    std::vector<std::string> cpuMove(const std::unique_ptr<Board>& b, 
      const std::unique_ptr<Player>& next) override;
};

#endif
