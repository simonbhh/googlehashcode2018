#include "Placer.h"

using namespace std;
Placer::Placer(City* source)
{
	src = source;
	registerFrequency=50;
	actualPlacement=0;
	for (unsigned int i = 0; i < source->getCityHeight(); i++)
		shuffleCoord.push_back(i);
	for (unsigned int i = 0; i < source->getCityWidth(); i++)
		shuffleCoordCol.push_back(i);
	shuffle();
}
/**
 * @brief
 * Placement from the bottom right corner
 * @param b 
 * @return true 
 * @return false 
 */
bool Placer::tetrisPlacement(Building *b)
{
	for (int row = src->height - b->getRowNum(); row >= 0; row--)
	{

		for (int col = src->width - b->getColumnNum(); col >= 0; col--)
		{
			if (src->placeBuilding(b, row, col))
			{
				return true;
			}

		}
	}
	return false;
}
/**
 * @brief
 * Placement from the top right corner
 * @param b 
 * @return true 
 * @return false 
 */
bool Placer::tetrisPlacementTOP(Building *b)
{
	for (unsigned int row = 0; row <= src->height - b->getRowNum(); row++)
	{
		for (int col = src->width - b->getColumnNum(); col >= 0; col--)
		{
			if (src->placeBuilding(b, row, col))
			{
				return true;
			}
		}
	}
	return false;
}
/**
 * @brief
 * Place on a random coordinante.
 * @param b 
 * @return true 
 * @return false 
 */
bool Placer::tetrisAleat(Building* b)
{
	for(int row : shuffleCoord)
	{
		for(int col : shuffleCoordCol)
		{
		//	cout << "\nB";
		//	cout << row << "," << col << endl;
			if (src->placeBuilding(b, row, col))
			{
		//		cout << "FOUND" << endl;
				actualPlacement++;
				if (actualPlacement >= registerFrequency)
				{
		//			cout << "SHUFFLE" << endl;
					actualPlacement = 0;
					shuffle();
				}

				return true;

			}
		//	cout << "\nA\n";
		}
	}
	return false;
}
/**
 * @brief
 * Placement using only the connex composant.
 * @param b 
 * @return true 
 * @return false 
 */
bool Placer::ConvexPlacement(Building *b)
{
	for (auto CList: src->getConnexComposant())
	{
		for(auto C : CList)
		{
			if (src->placeBuilding(b, C.row, C.column))
			{
				return true;
			}
		}
	}

	return false;
}
/**
 * @brief
 * Shffule the coordinates array.
 */
void Placer::shuffle()
{
	random_shuffle(shuffleCoord.begin(), shuffleCoord.end());
	random_shuffle(shuffleCoordCol.begin(), shuffleCoordCol.end());
}