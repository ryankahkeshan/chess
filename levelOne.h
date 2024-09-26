#ifndef __LEVEL_ONE_H__
#define __LEVEL_ONE_H__
#include "computer.h"

class LevelOne : public Computer {
  public:
    LevelOne(Colour c, Colour start); // Constructor
    std::vector<std::string> cpuMove(const std::unique_ptr<Board>& b, 
      const std::unique_ptr<Player>& next) override;
};

#endif
