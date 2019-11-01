#pragma once
#include "Project.h"
#include <time.h>
#include <deque>
#include <mutex>
/**
 * @brief
 * Class use to generate list of building.
 */
class Chooser
{
	public:
		Chooser();
		Chooser(double type_,double utilities_,
			double utilitiesType_,double residential_,Project* p);
		Chooser(Project* p);
		static void initChooser();
		void initialize();
		void refill();
		Building* get();
		Building* getEnd();
	private:
		static bool dice(double proba);//Generate a random number and test if it's inferior to the proba
		static double randNumber();
		static bool isRandInit;
		Project* projectRef;
		double type;
		double utilities;
		double utilitiesType;
		double residential;
		bool lastGet=false;
		std::deque<Building*> pile;
		std::deque<Building*> save;
		std::vector<Building*> residentialList;
		std::vector<std::pair<unsigned int, std::vector<Building *>> > utilitiesSorted;
		static std::map<unsigned int,unsigned int> utilitiesRegister;
		static mutex registerMutex;
		std::map<unsigned int,std::vector<Building*>> utilitiesLists;


};
