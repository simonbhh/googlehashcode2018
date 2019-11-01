#pragma once
#include "Project.h"
#include <random>
/**
 * @brief
 * Class used to place building in a tetris way.
 */
class Placer
{
  public:
	Placer(City *source);
	bool tetrisPlacement(Building *b);
	bool tetrisPlacementTOP(Building *b);
	bool tetrisAleat(Building *b);
	bool ConvexPlacement(Building* b);
	void shuffle();

  private:
	City *src;//City in which we want to place building
	int registerFrequency;//Frequency to update the line register.
	int actualPlacement;//Number of placement since the last buildRegister
	std::vector<int> shuffleCoord;//Shuffled coordinates arrays
	std::vector<int> shuffleCoordCol;
};
