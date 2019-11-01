#include "Project.h"
using namespace std;
Project Project::globalProject;

Project::Project() {
	this->buildings = vector<Building *>();
	this->residentials = vector<Building*>();
	this->utilities = map<unsigned int,vector<Building*> >();
	city = nullptr;
}

Project::Project(City* city_)
{

	this->city = city_;
	this->buildings = vector<Building*>();
	this->residentials = vector<Building *>();
	this->utilities = map<unsigned int, vector<Building *>>();
}
Project::~Project()
{
	delete city;
	for(auto b:buildings)
		delete b;
}
void Project::setProjectFile(string projectFile)
{
	this->projectFile = projectFile;
}

void Project::setSolutionFile(string solutionFile)
{
	this->solutionFile = solutionFile;
}

string Project::getProjectFile()
{
	return this->projectFile;
}

string Project::getSolutionFile()
{
	return this->solutionFile;
}

void Project::addUtility(Building *u)
{
	this->buildings.push_back(u);
	if(utilities.find(u->getExtra())==utilities.end())
	{
		utilities[u->getExtra()] = vector<Building*>();

	}
		utilities[u->getExtra()].push_back(u);
}

void Project::addResidential(Building *r)
{
	this->buildings.push_back(r);
	this->residentials.push_back(r);
}

void Project::setCity(unsigned int width, unsigned int height)
{

	if(city!=nullptr)
		delete city;
	city = new City(height,width);
}
void Project::prepare()
{
	sort(residentials.begin(),residentials.end(),[](Building* a,Building* b)->bool
	{
		return a->getDensity()<b->getDensity();
	});
	for(auto it:utilities)
	{
		sort(it.second.begin(), it.second.end(), [](Building *a,Building *b) -> bool {
			return a->getRatio() < b->getRatio();
		});
	}
	minNbCells =(*min_element(buildings.begin(),buildings.end(),[](const Building* a,const Building* b){
		return a->getNbCells()<a->getNbCells();
	}))->getNbCells();
}
/**
 * @brief
 * Set the maximal walking distance of this city.
 * Define at the same time all coordinates relative to a cell that can be accesible.
 * @param MaxWD
 */
void Project::setMaxWalkingDistance(int MaxWD)
{
	int width = 0;
	for(int row=-MaxWD;row<=MaxWD;row++)
	{
		for(int col=-width;col<=width;col++)
		{
			if(!(row==0&&col==0))
			{
				this->basic_influenceArea.push_back({short(row),short(col)});
			}
		}
		if(row<0)
			width++;
		else
			width--;
	}
	this->maxWalkingDistance = MaxWD;
}
