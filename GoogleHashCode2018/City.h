#pragma once
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <fstream>
#include "Building.h"

/*
	Structure correspondant à une coordonée sur la map
*/
//Forward declaration
struct Coord
{
	//Constructeurs
	Coord()
	{
		row = 0;
		column =0;
	}

	Coord(short int row,short int column)
	{
		this->row = row;
		this->column = column;
	}
	short int row;
	short int column;
};
Coord operator+(const Coord& A,const Coord& B);
bool operator==(const Coord& A,const Coord& B);
bool operator<(const Coord A,const Coord B);
/*
	Structure correspondant à un building placé sur la map
*/
struct PlacedBuilding
{
	//Constructeurs
	PlacedBuilding(const Building* buildingNum);
	PlacedBuilding(const PlacedBuilding& P);
	PlacedBuilding(PlacedBuilding& P,Coord C={0,0});
	//Uniquement utilisable par un residential
	int use(unsigned int utilityType);
	int undo();
	std::map<unsigned int, short int> connectedUtility;
	const Building* source;
	Coord position;
};

class City {
public:
	friend class Placer;
	City();
	City(unsigned int h,unsigned int w);
	City(const City& c);
	virtual City& operator=(City& c);
	City(unsigned int h,unsigned w,City& c,unsigned int row,unsigned int col);

	bool placeMap(City& c,unsigned int row,unsigned int col);
	double placeBuilding(const Building* building,unsigned int row,unsigned int col,bool test=true);
	void setMapCell(int row, int col, int value);
	int computeScore(PlacedBuilding& A,PlacedBuilding& B);
	void toSolution(string outfileName);
	void PrintMap();
	void undo(int nbBUilding);
	void undo();
	int getScore() const;
	int getRemainingCell() const;
	int getBuildingQuantity() const;
	short int getMapCell(int row, int col) const;
	const unsigned int& getCityWidth()	const{ return width; }
	const unsigned int& getCityHeight() const{ return height; }
	const set<Coord>& getRemainingCellsList()const;
	std::vector<std::set<Coord>>&& getConnexComposant();

private:
	unsigned int width;
	unsigned int height;
	std::vector<std::vector<short int>> buildingMap;
	std::vector<std::vector<short int>> connexMap;
	int score;
	bool connexInit = false;
	//Placed building, index sur les bâtiments posés
	vector<PlacedBuilding> placedBuildingRegister;
	vector<PlacedBuilding*> registeredUtilities;
	vector<PlacedBuilding*> registeredResidentials;
	int connexCount=0;
	int connexResetFrequency=10;
	std::map<int, std::set<Coord>> ConnexComposant;
	set<Coord> RemainingCellsList;
};