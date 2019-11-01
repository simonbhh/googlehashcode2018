#include "Arbitror.h"
#include "FileHandling.h"
#include <iostream>
#include <sys/stat.h>
#include <chrono>
#include <Windows.h>
#include <utility>
#include <algorithm>
#include <fstream>

/*
	Number of execution of each executable
*/
#define NUMBER_EXEC 10

using namespace std;

/**
 * Vector that contains the executables
 */
vector<string> executables;
/**
 * Arbitror object
 */
Arbitror arbitror;

/**
 * Function that inserts the executable files in a vector
 *
 * @param file
 */
void handleExecutables(string file)
{
	cout << file << endl; //debug
	executables.push_back(file);
}

/**
 * Method that calculates the execution time and the score of the solution's computation
 *
 * @param command
 * @return pair<int, double>	the pair containing the socre and execution time of the executable
 */
pair<int, double> computeExecutionTime(const string command)
{	
	cout << "command: " << command << endl; //debug

	auto start = chrono::steady_clock::now();
	// We execute the executable
	system(command.c_str());
	auto end = chrono::steady_clock::now();

	// Execution time
	chrono::duration<double> executionTime = end - start;

	// We evaluate the solution to return the score
	arbitror.evaluate();
	return make_pair(arbitror.getScore(), executionTime.count());
}

/**
 * Main function
 *
 * @param argc
 * @param argv
 */
int main(int argc, char **argv)
{
	// We check the number of arguments
	if (argc == 3)
	{
		Project project = Project::globalProject;
		// We set the input file of the project here
		project.setProjectFile("input/a_example.in");
		// We set the solution file of the project here
		project.setSolutionFile("solution.txt");

		const char * executableDirectory;
		executableDirectory = argv[1];

		// We check if the second argument is a directory
		if (!((GetFileAttributesA(executableDirectory) & FILE_ATTRIBUTE_DIRECTORY) != 0))
		{
			cerr << "Error: the second argument is not a directory" << endl;
			cout << "Usage: " << argv[0] << " [executablesDirectory] [outputFile]" << endl;
			exit(0);
		}
		
		//We iterate through the directory
		IterateOnFileDir(executableDirectory, handleExecutables);

		vector<pair<int, double>> result;

		for (auto file : executables)
		{
			arbitror = Arbitror(project.getProjectFile(), project.getSolutionFile());
			double meanExecTime = 0;
			int meanScore = 0;
			pair<int, double> means;
			const string command = file + " " + arbitror.getProjectFile() 
				+ " "  + arbitror.getSolutionFile();

			// We compute the execution time of each execution of one executable
			// to have a mean, same for the score
			for (int i = 0; i < NUMBER_EXEC; i++)
			{
				means = computeExecutionTime(command);
				meanScore += means.first;
				meanExecTime += means.second;
			}
			result.push_back(make_pair(meanScore / NUMBER_EXEC, meanExecTime / NUMBER_EXEC));
		}
		
		//Sorting the vector on the first element of the pair (here the score)
		sort(result.begin(), result.end());

		// Writing the results in the output file
		ofstream outputFile;
		outputFile.open(argv[2]);
		outputFile << "Table of the sorted results" << endl;
		for (int i = 0; i < result.size(); i++)
		{
			outputFile << "Score: " << result[i].first
				<< "\t execution time: " << result[i].second << endl;
		}
	}
	else
	{
		cout << "Usage: " << argv[0] << " [executablesDirectory] [outputFile]" << endl;
		exit(0);
	}

	return EXIT_SUCCESS;
}
