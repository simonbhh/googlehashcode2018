#include "FileLoader.h"

using namespace std;

/**
 * Function that loads the project file to create the project, the map and the buildings
 *
 * @param projectFile
 * @param solutionFile
 */
void FileLoader::loadProject(string projectFile)
{
	Project& project = Project::globalProject;
	Building *building;

	ifstream openFile(projectFile);
	int height, width, maxWalkingDistance, nbOfBuildingProjects, rowNum, columnNum, capacity;
	char currentChar;
	string currentLine, projectType;
	vector<int> residential, utilities;

	// We check if the file exists
	if (openFile)
	{
		// Reading the first line
		openFile >> height >> width >> maxWalkingDistance >> nbOfBuildingProjects;
		project.setCity(height, width);
		project.setMaxWalkingDistance(maxWalkingDistance);
		// Reading the buildings
		for(int i = 0; i < nbOfBuildingProjects; i++) {
			openFile >> projectType >> rowNum >> columnNum >> capacity;

			if (projectType == "R"){
				building = new Building(i, rowNum, columnNum, capacity, Residential);
				project.addResidential(building);
			}
			else {
				building = new Building(i, rowNum, columnNum, capacity,Utility);
				project.addUtility(building);
			}

			// Construction of the occupied cells list
			for (int j = 0; j < rowNum; j++) {
				for (int k = 0; k < columnNum; k++)
				{
					openFile >> currentChar;
					if (currentChar == '.')
						building->setCell(j,k,0);
					else
						building->setCell(j,k, 1);
				}
			}
			building->computeShape();
		}
		Project::globalProject.prepare();
	}
	else
		cout << "The project file doesn't exist" << endl;
}

/**
 * Function that loads the solution file to place the buildings on the map of the city class
 *
 * @param projectFile
 * @param solutionFile
 */
void FileLoader::loadSolution(string projectFile, string solutionFile)
{
	Project& project = Project::globalProject;
	loadProject(projectFile);

	Building *building;
	City* city=project.city;

	int buildingsToPlace, buildingNum, row, col;

	ifstream openFile(solutionFile);

	// We check if the file exists
	if (openFile)
	{
		// We check if the file is empty
		if (!(openFile.peek() == ifstream::traits_type::eof()))
		{
			// Reading the first line
			openFile >> buildingsToPlace;

			// Reading all the other lines
			for (int i = 0; i < buildingsToPlace; i++)
			{
				openFile >> buildingNum >> row >> col;

				building = project.buildings[buildingNum];
				// If the building can't be placed, the solution is invalid
				if (!city->placeBuilding(building, row, col))
				{
					cout << "Invalid solution: at least 2 buildings are on top of each other" << endl;
					break;
				}
			}
		}
		else
			cout << "The solution file is empty" << endl;
	}
	else
		cout << "The solution file doesn't exist" << endl;
}
