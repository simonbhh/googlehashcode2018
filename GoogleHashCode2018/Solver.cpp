#include "Solver.h"
using namespace std;

void Solver::Solve(City* city)
{
	/**
	 * @brief
	 * Solving parameter
	 * Number of cities must be a multiple of the number of thread.
	 *
	 */
	unsigned int nbCities = 20; //Number of sub cities to compute
	unsigned int subcitySize = 100;//Sub cities size
	unsigned int nbThread = 1;//Number of thread
	
	auto start = chrono::steady_clock::now();
	if (city->getCityHeight() < 100 && city->getCityWidth() < 100) //cas de example_a avec un map plus petite que 100x100
	{
		SolveSubcity(city);
	}
	else
	{
		unsigned int nbCityPerThread = nbCities / nbThread;
		vector<vector<City*>> subProc(nbThread);
		vector<City*> plane;
		mutex printMutex;
		unsigned int remainingCities = nbCities;
		for (auto& p : subProc)
		{
			p = vector<City*>(nbCityPerThread);
			for (auto &pt : p)
				pt = new City(subcitySize, subcitySize);
		}
		unsigned int subcitiesNumber = (city->getCityHeight() * city->getCityWidth()) / (subcitySize*subcitySize);
		//COMPUTE SUBCITIES
		cout << "COMPUTING SUB CITIES" << endl;
		vector<thread*> subSolver(nbThread);
		Chooser::initChooser();
		for (int i = 0; i < subSolver.size(); i++)
		{
			vector<City*>& v = subProc[i];
			subSolver[i] = new thread([&v, &i, &printMutex, &remainingCities]()
			{
				for (auto& c : v)
				{
					SolveSubcity(c);
					printMutex.lock();
					remainingCities--;
					cout << "REMAINING CITIES : " << remainingCities << endl;
					printMutex.unlock();
				}
			});
		}
		for (auto& t : subSolver) {
			t->join();
			delete t;
		}
		for (auto& v : subProc)
		{
			plane.insert(plane.begin(),
				make_move_iterator(v.begin()),
				make_move_iterator(v.end()));
		}
		sort(plane.begin(), plane.end(), [](const City *a, const City *b) {
			return a->getScore() > b->getScore();
		});
		//ASSEMBLE
		cout << endl << "ASSEMBLING SUB CITIES" << endl;
		std::vector<City*> best; //prend les 20 meilleurs subcities
		for (unsigned int i = 0; i < nbCities; i++)
			best.push_back(plane[i]);
		random_shuffle(best.begin(), best.end());
		cout << "MAX SCORE FOR A SUB CITY : " <<
			(*max_element(best.begin(), best.end(), [](const City* a, const City* b)
		{
			return a->getScore() < b->getScore();
		}))->getScore()
			<< endl;
		cout << "MIN SCORE FOR A SUB CITY : " << (*min_element(best.begin(), best.end(), [](const City *a, const City *b) {
			return a->getScore() < b->getScore();
		}))
			->getScore()
			<< endl;
		mutex copyTex;
		vector<std::vector<City*>> bestSplit;
		for (unsigned int i = 0; i < nbThread; i++)
		{
			bestSplit.push_back(vector<City*>());
			int begin = (int(best.size()) / nbThread) * i;
			int end = (int(best.size()) / int(nbThread)) * (i + 1);
			for (int k = begin; k < end; k++) //pour chaque emplacement de la map finale on test chaque subcity pour voir quelle est la plus adaptee
			{
				bestSplit.back().push_back(best[k]);
			}
		}
		for (unsigned int i = 0; i < city->getCityWidth() - subcitySize + 1; i += subcitySize)
		{

			for (unsigned int j = 0; j < city->getCityHeight() - subcitySize + 1; j += subcitySize)
			{
				vector<thread *> subTest;
				vector<pair<City*, int>> scores(nbThread);
				fill(scores.begin(), scores.end(), pair<City*, int>(nullptr, 0));
				City c(*city);
				for (unsigned int e = 0; e < nbThread; e++)
				{
					auto& scoresPair = scores[e];
					auto& subBest = bestSplit[e];
					subTest.push_back(new thread([&scoresPair, c, &subBest, e, i, j, nbThread, &copyTex]() mutable {
						for (auto& sBest : subBest) //pour chaque emplacement de la map finale on test chaque subcity pour voir quelle est la plus adaptee
						{
							c.placeMap(*sBest, i, j); //on place une sous-map
							if (scoresPair.second < c.getScore())
							{
								scoresPair.first = sBest;
								scoresPair.second = c.getScore();
							}
							c.undo(sBest->getBuildingQuantity());
						}
					}));
				}
				for (const auto& t : subTest)
				{
					t->join();
					delete t;
				}
				auto max = std::max_element(scores.begin(), scores.end(), //cacul du meilleur score pour recuperer sa position
					[](const pair<City *, int> &s1, const pair<City *, int> &s2) { return s1.second < s2.second; });

				city->placeMap(*(*max).first, i, j);//on place la meilleure subcity dans la city finale
			}
		}
			//=================================================
			cout << "CLEANING MEMORY" << endl;
			for(auto&p:plane)
				delete p;
	}
	
	auto end = chrono::steady_clock::now();
	chrono::duration<double> execTime = end - start;
	cout << "END OF SOLVING ALGORITHM" << endl;
	cout << "ELAPSED TIME : " << execTime.count() << endl;
}


void Solver::SolveSubcity(City* city)
{
	/**
	 * Solver uses a combination of a chooser and a placer to optimize placement on city
	 * Work in a raisonable time for small cities.
	 *
	 * The chooser is used to sort and find building using random generated list.
	 * The placer is used in 5 ways to optimize calculus time.
	 *
	 */
	 /**
	  * Type : probabilty to choose a type
	  * 		High : more probability to choose a residential
	  * 		Low : more probabilitu to choose an utility
	  * U : probability of choosing an utility
	  * 		High : more probability to choose a small utility
	  * 		Low : more probability to choose a big utility
	  * UT : probability of choosing a type of utility
	  * 		High : more probability of choosing most represented utility
	  * 		Low : more probability of choosing a less represented utility
	  * R : probability of choosing a residential
	  * 		High : more probabilityu to choose a small residential
	  * 		Low : more probability to choose a big residential
	  */
	double Type = 0.5;
	double U = 0.2;
	double UT = 0.9;
	double R = 0.8;
	Chooser c(Type,U,UT,R,&Project::globalProject);
	Placer p(city);
	Building* b;
	//threshold for each placement type
	//Placement from the top
	/**int seuil = (city->getCityHeight() * city->getCityWidth())*0.5;
	//Random placement
	int seuilMIN = seuil*0.7;
	//Placement with last cells
	int seuilShuffle = seuilMIN*0.1;
	int seuilEnd = seuilShuffle*0.8;**/
	//Count of placement
	int actualPlacement = 0;
	//Threslhold for reinitialising the chooser
	int reinitSeuil = 10;
	//DEFINING PLACEMENT FUNCTION
	auto bottomRightPlacement = [](Building* b, Placer* p)->bool {
		return p->tetrisPlacement(b);
	};
	auto topRightPlacement = [](Building *b, Placer *p) -> bool {
		return p->tetrisPlacementTOP(b);
	};
	auto aleatPlacement = [](Building *b, Placer *p) -> bool {
		return p->tetrisAleat(b);
	};
	auto convexPlacement = [](Building* b, Placer* p) -> bool {
		return p->tetrisPlacementTOP(b);
	};
	//DEFINING CHOOSER GET METHOD
	auto baseGet = [](Chooser* c)->Building*
	{
		return c->get();
	};
	auto endGet = [](Chooser* c) -> Building * {
		return c->getEnd();
	};
	//DEFINING TYPE OF A COMPUTING STEP
	using placementGetPair = tuple<function<bool(Building *, Placer *)>,
		function<Building *(Chooser*)>, double, string>;

	vector<placementGetPair> placements;
	double seuil = (city->getCityHeight() * city->getCityWidth());
	//FILLING PLACEMENTS STEPS
	/**
	 * Tuple explaination :
	 * 		Placement method
	 * 		Chooser method
	 *		Threshold to the next step, if the current number of remaining cells
	 *		 	if inferior to the Threshold defined by the next step
				go to the next step
	 */
	placements.push_back({ bottomRightPlacement,baseGet,1,"BOTTOM RIGHT,BASEGET" });
	placements.push_back({ topRightPlacement,baseGet,0.8,"TOP RIGHT, BASEGET" });
	placements.push_back({ aleatPlacement,baseGet,0.7,"RANDOM, BASEGET" });
	placements.push_back({ convexPlacement,baseGet,0.7,"CONNEX, BASEGET" });
	placements.push_back({ convexPlacement,endGet,0.8,"CONNEX, ENDGET" });
	unsigned int lastPlacement = 1;
	unsigned int nbPlacement = (unsigned int) placements.size();
	placementGetPair actualP=placements[0];
	auto nextSeuil = [&seuil, city, placements, &lastPlacement, &actualP]()
		-> bool {
		if (city->getRemainingCell() <= seuil)
		{
			if (lastPlacement >= placements.size())
				return true;
			actualP = placements[lastPlacement++];
			if(lastPlacement<placements.size())
				seuil = get<2>(placements[lastPlacement])* seuil;
		}
		return true;
	};
	while ((b = (get<1>(actualP)(&c))) != nullptr)
	{
		if (get<0>(actualP)(b, &p))
		{
			actualPlacement++;
			if (actualPlacement > reinitSeuil)
			{
				c.initialize();
				actualPlacement = 0;
			}
			else
			{
				c.refill();
			}
		}
		nextSeuil();
	}
	cout << "SUB MAP SCORE : " << city->getScore() << endl;
}


Solver::~Solver()
{
}
