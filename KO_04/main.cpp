#include <iostream>
#include <utility>
#include <string>
#include <string_view>
#include <random>
#include <chrono>
#include <thread>
#include <array>
#include <vector>
using namespace std;

struct City {
	enum Suburbs {
		low_residential,
		high_residential,
		entertainment,
		industrial,
		commercial,
		administrative,
		train_station,
		prison,
	};

	struct SuburbPopulation {
		Suburbs suburb;
		unsigned int population;

		// city population categorized into three main groups: workers, teachers, artists
		struct Civilians {
			pair<string, unsigned int> workers;
			pair<string, unsigned int> teachers;
			pair<string, unsigned int> artists;
		};

		Civilians civilians;

		// constructor to initialize all fields and set population from civilians
		SuburbPopulation(Suburbs s, Civilians c) : suburb(s), civilians(c) {
			population = civilians.workers.second + civilians.teachers.second + civilians.artists.second;
		}
	};

	// helper function to distribute population among civilian types
	SuburbPopulation::Civilians distributeCivilians(unsigned int total_population, float worker_ratio = 0.6f, float teacher_ratio = 0.25f, float artist_ratio = 0.15f) {
		unsigned int workers = static_cast<unsigned int>(total_population * worker_ratio);
		unsigned int teachers = static_cast<unsigned int>(total_population * teacher_ratio);
		unsigned int artists = static_cast<unsigned int>(total_population * artist_ratio);

		return {
			{"workers", workers},
			{"teachers", teachers},
			{"artists", artists}
		};
	}

	SuburbPopulation::Civilians redistributePopulation(unsigned int total_population, float worker_ratio = 0.6f, float teacher_ratio = 0.25f, float artist_ratio = 0.15f) {
		// redistribute population after civilians have moved to new suburbs
		unsigned int workers = static_cast<unsigned int>(total_population * worker_ratio);
		unsigned int teachers = static_cast<unsigned int>(total_population * teacher_ratio);
		unsigned int artists = static_cast<unsigned int>(total_population * artist_ratio);

		return {
			{"workers", workers},
			{"teachers", teachers},
			{"artists", artists}
		};
	}

	bool isCoastalOrPrison(Suburbs suburb) {
		// logic to determine if a suburb is coastal or prison based on its enum value (refer to reference image)
		return suburb == industrial || suburb == high_residential || suburb == prison; // coastal and prison suburbs
	}

	// low residential = 1, high residential = 2, administrative = 3, commercial = 4, entertainment = 5, industrial = 6, train station = 7, prison = 8
	// low residential is adjacent to high residential, administrative, commercial and entertainment
	// high residential is adjacent to low residential, administrative, industrial and prison
	// administrative is adjacent to low residential, high residential, commercial and industrial
	// commercial is adjacent to low residential, administrative, entertainment and industrial
	// entertainment is adjacent to low residential, commercial, industrial and train station
	// industrial is adjacent to high residential, administrative, commercial, entertainment and prison
	// train station is adjacent to entertainment
	// prison is adjacent to high residential and industrial as they are coastal suburbs
	bool isAdjacent(Suburbs suburb1, Suburbs suburb2) {
		// logic to determine if two suburbs are adjacent based on a predefined adjacency list (refer to comments above and reference image)
		if ((suburb1 == low_residential && suburb2 == high_residential) || (suburb1 == low_residential && suburb2 == administrative) || (suburb1 == low_residential && suburb2 == commercial) || (suburb1 == low_residential && suburb2 == entertainment) || // low residential
			(suburb1 == high_residential && suburb2 == low_residential) || (suburb1 == high_residential && suburb2 == administrative) || (suburb1 == high_residential && suburb2 == industrial) || // high residential
			(suburb1 == administrative && suburb2 == low_residential) || (suburb1 == administrative && suburb2 == high_residential) || (suburb1 == administrative && suburb2 == commercial) || (suburb1 == administrative && suburb2 == industrial) || // administrative
			(suburb1 == commercial && suburb2 == low_residential) || (suburb1 == commercial && suburb2 == administrative) || (suburb1 == commercial && suburb2 == entertainment) || (suburb1 == commercial && suburb2 == industrial) || // commercial
			(suburb1 == entertainment && suburb2 == low_residential) || (suburb1 == entertainment && suburb2 == commercial) || (suburb1 == entertainment && suburb2 == industrial) || (suburb1 == entertainment && suburb2 == train_station) || // entertainment
			(suburb1 == industrial && suburb2 == high_residential) || (suburb1 == industrial && suburb2 == administrative) || (suburb1 == industrial && suburb2 == commercial) || (suburb1 == industrial && suburb2 == entertainment) || // industrial
			(suburb1 == train_station && suburb2 == entertainment) || // train station
			(isCoastalOrPrison(suburb1) && isCoastalOrPrison(suburb2))) { // prison is adjacent to coastal suburbs
			return true;
		}
		else false;
	}

	void moveCiviliansRandomly(int suburb_index, SuburbPopulation& population_i, array<SuburbPopulation*, 8>& all_suburbs, const char* const suburb_names[]) {
		// logic to randomly move civilians between adjacent suburbs using public transport or ferry line for coastal suburbs
		mt19937 mt{ static_cast<unsigned int>(chrono::steady_clock::now().time_since_epoch().count()) };
		uniform_int_distribution<int> popul(0, 50); // 0-50% range

		for (int j = 0; j < 8; ++j) {
			if (suburb_index != j && isAdjacent(static_cast<Suburbs>(suburb_index), static_cast<Suburbs>(j))) {
				int moveCount = static_cast<int>(population_i.population * popul(mt) / 100.0f);
				
				population_i.population -= moveCount;
				all_suburbs[j]->population += moveCount;

				cout << moveCount << " civilians moved from " << suburb_names[suburb_index] << " to " << suburb_names[j] << endl;

				this_thread::sleep_for(chrono::milliseconds(100)); // simulate half a second for civilians to move between suburbs
			}
		}

		cout << "-----------------------------" << endl;
	}

	void evacuateToClosestSuburbs(int suburb_index, SuburbPopulation& population_i, array<SuburbPopulation*, 8>& all_suburbs, const char* const suburb_names[]) {
		// logic to evacuate civilians to the closest suburbs in case of a natural disaster
		vector<int> adjacent_suburbs;

		for (int j = 0; j < 8; ++j) {
			if (suburb_index != j && isAdjacent(static_cast<Suburbs>(suburb_index), static_cast<Suburbs>(j))) {
				adjacent_suburbs.push_back(j);
			}
		}

		int evacCount = static_cast<int>(population_i.population / adjacent_suburbs.size());
		for (int sub : adjacent_suburbs) {
			population_i.population -= evacCount;
			all_suburbs[sub]->population += evacCount;
			this_thread::sleep_for(chrono::milliseconds(300)); // simulate 300 milliseconds for evacuation
			cout << evacCount << " civilians evacuated from " << suburb_names[suburb_index] << " to " << suburb_names[sub] << endl;
		}
	}

	void showSuburbData(SuburbPopulation suburb, const char* const suburb_names[]) {
		// logic to show latest data of suburbs on each turn
		cout << "-----------------------------" << endl;
		cout << "Suburb: " << suburb_names[suburb.suburb] << endl;
		cout << "Population: " << suburb.population << endl;
		cout << "Workers: " << suburb.civilians.workers.second << endl;
		cout << "Teachers: " << suburb.civilians.teachers.second << endl;
		cout << "Artists: " << suburb.civilians.artists.second << endl;
	}
};

// array of suburb names for display purposes
constexpr const char* suburb_names[8] = {
    "Low Residential",      // index 0 = low_residential
    "High Residential",     // index 1 = high_residential
    "Entertainment",        // index 2 = entertainment
    "Industrial",           // index 3 = industrial
    "Commercial",           // index 4 = commercial
    "Administrative",       // index 5 = administrative
    "Train Station",        // index 6 = train_station
    "Prison"                // index 7 = prison
};

int main() {
	// initialize city
	City brisbane;

	mt19937 mt{static_cast<unsigned int>(chrono::steady_clock::now().time_since_epoch().count())};

	uniform_int_distribution<int> popul(1000, 200000); // for random suburb selection

	// initialize suburb populations with distributed civilians
	City::SuburbPopulation low_residential_population = { City::low_residential, brisbane.distributeCivilians(popul(mt)) };
	City::SuburbPopulation high_residential_population = { City::high_residential, brisbane.distributeCivilians(popul(mt)) };
	City::SuburbPopulation entertainment_population = { City::entertainment, brisbane.distributeCivilians(popul(mt), 0.3f, 0.2f, 0.5f) };
	City::SuburbPopulation industrial_population = { City::industrial, brisbane.distributeCivilians(popul(mt), 0.7f, 0.1f, 0.2f) };
	City::SuburbPopulation commercial_population = { City::commercial, brisbane.distributeCivilians(popul(mt)) };
	City::SuburbPopulation administrative_population = { City::administrative, brisbane.distributeCivilians(popul(mt), 0.5f, 0.4f, 0.1f) };
	City::SuburbPopulation train_station_population = { City::train_station, brisbane.distributeCivilians(10000) };
	City::SuburbPopulation prison_population = { City::prison, brisbane.distributeCivilians(500) };

	// array of pointers to all suburb populations for easy access when moving civilians
	array<City::SuburbPopulation*, 8> all_suburbs = {
		&low_residential_population,
		&high_residential_population,
		&entertainment_population,
		&industrial_population,
		&commercial_population,
		&administrative_population,
		&train_station_population,
		&prison_population
	};

	// simulate city activities for as many turns as player wants
	bool continue_simulation = true;

	while (continue_simulation) {
		this_thread::sleep_for(chrono::seconds(1)); // pause for 1 second to simulate processing time

		// show suburb population data
		brisbane.showSuburbData(low_residential_population, suburb_names);
		brisbane.showSuburbData(high_residential_population, suburb_names);
		brisbane.showSuburbData(entertainment_population, suburb_names);
		brisbane.showSuburbData(industrial_population, suburb_names);
		brisbane.showSuburbData(commercial_population, suburb_names);
		brisbane.showSuburbData(administrative_population, suburb_names);
		brisbane.showSuburbData(train_station_population, suburb_names);
		brisbane.showSuburbData(prison_population, suburb_names);

		// ask player if they want to continue simulation or exit before moving civilians
		char choice;
		cout << "Enter any key to continue or 'n' if you want to exit" << endl;
		cin >> choice;
		if (choice == 'n' || choice == 'N') {
			continue_simulation = false;
			break;
		}

		cout << "-----------------------------" << endl;
		cout << "Civilians are moving between suburbs..." << endl;
		this_thread::sleep_for(chrono::seconds(1)); // allow 1 second for player to read data before moving civilians

		// randomly move civilians between suburbs
		brisbane.moveCiviliansRandomly(0, low_residential_population, all_suburbs, suburb_names);
		brisbane.moveCiviliansRandomly(1, high_residential_population, all_suburbs, suburb_names);
		brisbane.moveCiviliansRandomly(2, entertainment_population, all_suburbs, suburb_names);
		brisbane.moveCiviliansRandomly(3, industrial_population, all_suburbs, suburb_names);
		brisbane.moveCiviliansRandomly(4, commercial_population, all_suburbs, suburb_names);
		brisbane.moveCiviliansRandomly(5, administrative_population, all_suburbs, suburb_names);
		brisbane.moveCiviliansRandomly(6, train_station_population, all_suburbs, suburb_names);
		brisbane.moveCiviliansRandomly(7, prison_population, all_suburbs, suburb_names);

		// randomly trigger natural disaster
		if (uniform_int_distribution<int>(1, 10)(mt) == 1) { // 10% chance of natural disaster each turn
			uniform_int_distribution<int> suburb_disaster(0, 7);
			int disaster_suburb = suburb_disaster(mt);

			cout << "Natural disaster occurred in " << suburb_names[disaster_suburb] << "! Enter any key to evacuate civilians." << endl;
			cin >> choice;

			brisbane.evacuateToClosestSuburbs(disaster_suburb, *all_suburbs[disaster_suburb], all_suburbs, suburb_names);
		}

		// redistribute civilian type population
		low_residential_population = { City::low_residential, brisbane.redistributePopulation(low_residential_population.population)};
		high_residential_population = { City::high_residential, brisbane.redistributePopulation(high_residential_population.population) };
		entertainment_population = { City::entertainment, brisbane.redistributePopulation(entertainment_population.population) };
		industrial_population = { City::industrial, brisbane.redistributePopulation(industrial_population.population) };
		commercial_population = { City::commercial, brisbane.redistributePopulation(commercial_population.population) };
		administrative_population = { City::administrative, brisbane.redistributePopulation(administrative_population.population) };
		train_station_population = { City::train_station, brisbane.redistributePopulation(train_station_population.population) };
		prison_population = { City::prison, brisbane.redistributePopulation(prison_population.population) };

		// ask player if they want to continue simulation
		cout << "Continue simulation? (y/n): ";
		cin >> choice;
		continue_simulation = (choice == 'y' || choice == 'Y');
	}

	cout << "End of city simulation " << endl; // flush output buffer before exiting

	return 0;
}

//Homework: City Simulation
//Details :
//City Simulation
//Develop a turn - based simulation of a modern city.
//The city has 8 suburbs.One of these suburbs is located on an island.
//The city employs a public transport system that facilitates the movement of civilians between suburbs that share "common borders".However, for the island suburb,
//civilians utilize a ferry line that operates exclusively for the coastal suburbs.
//Each suburb is home to a population of "Civilians", categorized into three main groups :
//Workers
//Teachers
//Artist
//
//
//On each turn show latest data of suburbs and public transport activities.
//On each n turn the city may randomly experienced a natural disaster.The population must get evac to closet suburbs.