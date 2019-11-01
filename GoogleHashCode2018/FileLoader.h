#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include "Project.h"


class FileLoader
{
public:

	static void loadProject(std::string projectFile);
	static void loadSolution(std::string projectFile, std::string solutionFile);
};

