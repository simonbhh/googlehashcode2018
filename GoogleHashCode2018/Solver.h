#pragma once
#include "Chooser.h"
#include "Placer.h"
#include <tuple>
#include <thread>
#include <mutex>
class Solver
{
public:
	static void Solve(City* city);//Method use to fill a city with building
	static void SolveSubcity(City* city);
	~Solver();
};

//TO DO : faire une methode assemble() qui assemble tous les bouts de city de 100*100
// pour stocker les city faire un vector de city
// utiliser max() pour trouver les meilleurs
// a la fin de assemble ecrire la solution