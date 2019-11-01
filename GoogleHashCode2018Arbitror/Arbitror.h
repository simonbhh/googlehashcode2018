#pragma once

#include "../GoogleHashCode2018/City.h"
#include "../GoogleHashCode2018/Project.h"
#include "../GoogleHashCode2018/FileLoader.h"

class Arbitror
{
private:
	std::string projectFile;
	std::string solutionFile;
	int score;
public:
	Arbitror();
	Arbitror(std::string projectFile, std::string solutionFile);

	void evaluate();
	int getScore();
	std::string getProjectFile();
	std::string getSolutionFile();

	~Arbitror();
};


