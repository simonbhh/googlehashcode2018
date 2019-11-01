#include "Arbitror.h"

using namespace std;

/**
 * Default constructor
 */
Arbitror::Arbitror()
{
}

/**
 * Constructor that set the score to 0
 *
 * @param projectFile
 * @param solutionFile
 */
Arbitror::Arbitror(string projectFile, string solutionFile)
{
	this->projectFile = projectFile;
	this->solutionFile = solutionFile;
	this->score = 0;
}

/**
 * Method that evaluates the score of the solution for each building placed
 */
void Arbitror::evaluate()
{
	FileLoader::loadSolution(this->projectFile, this->solutionFile);
	this->score = Project::globalProject.city->getScore();
}

/**
 * Method that returns the score of the solution
 */
int Arbitror::getScore()
{
	return this->score;
}

/**
 * Getter project file
 *
 * @return the project file
 */
string Arbitror::getProjectFile()
{
	return this->projectFile;
}

/**
 * Getter solution file
 *
 * @return the solution file
 */
string Arbitror::getSolutionFile()
{
	return this->solutionFile;
}

/**
 * Destructor of the class
 */
Arbitror::~Arbitror()
{
}
