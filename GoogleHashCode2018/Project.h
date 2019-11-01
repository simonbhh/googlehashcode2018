#pragma once
#include "City.h"

class Project
{
private:
	std::string projectFile;
	std::string solutionFile;
	std::vector<Coord> basic_influenceArea;
	int maxWalkingDistance;
	int minNbCells;
public:
	friend class Building;
	friend class City;
	static Project globalProject;
	Project();
	Project(City* city_); // liste des utilities faisant reference a l index dans building
	~Project();
	City* city;
	std::vector<Building *> buildings; // liste des projects buildings disponibles
	std::vector<Building*> residentials; // liste des residences faisant referance a l index dans building
	std::map<unsigned int, std::vector<Building*> > utilities;

	void prepare();//Sort all containers.
	void setProjectFile(std::string projectFile);
	void setSolutionFile(std::string solutionFile);
	std::string getProjectFile();
	std::string getSolutionFile();
	void setCity(unsigned int width,unsigned int height);
	void addUtility(Building* b);
	void addResidential(Building *r);
	void setMaxWalkingDistance(int MaxWD);

};