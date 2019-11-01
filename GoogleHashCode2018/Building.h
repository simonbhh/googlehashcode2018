#pragma once
#include <vector>
#include <functional>
#include <iostream>
#include <set>
#include <algorithm>
class Coord;

using namespace std;
enum Building_type{
	Residential,
	Utility
};
class Building
{
public:
	Building(const Building &b);
	~Building();
	Building& operator=(Building const& buildingB);
	Building(unsigned short int projectNum_, unsigned short int rowNum_, unsigned short int columnNum_,unsigned short int extra,Building_type t);
	void assignArray(short int** array);
	void buildInfluenceArea();

	int getCell(unsigned int row, unsigned int column);
	unsigned short int getProjectNum() const{ return this->projectNum; }
	unsigned short int getRowNum() const { return this->rowNum; }
	unsigned short int getColumnNum() const { return this->columnNum; }
	unsigned short int getExtra() const { return this->extra; }
	vector<Coord> getShape();
	const std::vector<Coord>& getInfluenceArea() const;
	float getRatio() ;
	float getDensity() ;
	unsigned int getNbCells() const;
	Building_type getType() const;
	const std::vector<Coord>& getCases() const;

	void setCell(unsigned int row, unsigned int column, int value);
	void setProjectNum(unsigned int projectNum_);
	bool cellInRes(unsigned int row, unsigned int column, vector<Coord> result);
	void computeShape();
	void print() const;

  protected:
	unsigned short int projectNum;
	unsigned short int rowNum;
	unsigned short int columnNum;
	unsigned short int extra;//Capacité pour residence et type pour utility.
	Building_type type;
	short int** occupiedCells;
	std::vector<Coord> shape;
	std::vector<Coord> influenceArea;
	std::vector<Coord> cases;//List of all occupied cells of a building
	float ratio;//area of a building divided by all his cases
	float density;//Density of a residentail building
	unsigned int nbCells;

};