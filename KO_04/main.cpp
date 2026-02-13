#include <iostream>
#include <utility>
#include <string>
#include <string_view>
#include <random>
#include <chrono>
#include <thread>
#include <array>
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
	SuburbPopulation::Civilians distribute_civilians(unsigned int total_population, float worker_ratio = 0.6f, float teacher_ratio = 0.25f, float artist_ratio = 0.15f) {
		unsigned int workers = static_cast<unsigned int>(total_population * worker_ratio);
		unsigned int teachers = static_cast<unsigned int>(total_population * teacher_ratio);
		unsigned int artists = static_cast<unsigned int>(total_population * artist_ratio);

		return {
			{"workers", workers},
			{"teachers", teachers},
			{"artists", artists}
		};
	}

	bool is_Island(Suburbs suburb) {
		return suburb == prison; // prison is the island suburb as per reference image
	}

	bool is_Coastal(Suburbs suburb) {
		return suburb == industrial || suburb == high_residential; // coastal suburbs
	}

	// low residential = 1, high residential = 2, administrative = 3, commercial = 4, entertainment = 5, industrial = 6, train station = 7, prison = 8
	// low residential is adjacent to high residential, administrative, commercial and entertainment
	// high residential is adjacent to low residential, administrative and industrial
	// administrative is adjacent to low residential, high residential, commercial and industrial
	// commercial is adjacent to low residential, administrative, entertainment and industrial
	// entertainment is adjacent to low residential, commercial, industrial and train station
	// industrial is adjacent to high residential, administrative, commercial and entertainment
	// train station is adjacent to entertainment
	// prison is adjacent to nothing
	bool is_Adjacent(Suburbs suburb1, Suburbs suburb2) {
		// logic to determine if two suburbs are adjacent based on a predefined adjacency list
		if ((suburb1 == low_residential && suburb2 == high_residential) || (suburb1 == low_residential && suburb2 == administrative) || (suburb1 == low_residential && suburb2 == commercial) || (suburb1 == low_residential && suburb2 == entertainment) || // low residential
			(suburb1 == high_residential && suburb2 == low_residential) || (suburb1 == high_residential && suburb2 == administrative) || (suburb1 == high_residential && suburb2 == industrial) || // high residential
			(suburb1 == administrative && suburb2 == low_residential) || (suburb1 == administrative && suburb2 == high_residential) || (suburb1 == administrative && suburb2 == commercial) || (suburb1 == administrative && suburb2 == industrial) || // administrative
			(suburb1 == commercial && suburb2 == low_residential) || (suburb1 == commercial && suburb2 == administrative) || (suburb1 == commercial && suburb2 == entertainment) || (suburb1 == commercial && suburb2 == industrial) || // commercial
			(suburb1 == entertainment && suburb2 == low_residential) || (suburb1 == entertainment && suburb2 == commercial) || (suburb1 == entertainment && suburb2 == industrial) || (suburb1 == entertainment && suburb2 == train_station) || // entertainment
			(suburb1 == industrial && suburb2 == high_residential) || (suburb1 == industrial && suburb2 == administrative) || (suburb1 == industrial && suburb2 == commercial) || (suburb1 == industrial && suburb2 == entertainment) || // industrial
			(suburb1 == train_station && suburb2 == entertainment)) { // train station
			return true;
		}
		else false;
	}

	void move_civilians_randomly(SuburbPopulation& population_i, array<SuburbPopulation*, 8>& all_suburbs) {
		mt19937 mt{ static_cast<unsigned int>(chrono::steady_clock::now().time_since_epoch().count()) };
		uniform_int_distribution<int> popul(0, 50); // 0-50% range

		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (i != j && is_Adjacent(static_cast<Suburbs>(i), static_cast<Suburbs>(j))) {
					int moveCount = static_cast<int>(population_i.population * popul(mt) / 100.0f);
					
					population_i.population -= moveCount;
					all_suburbs[j]->population += moveCount;
				}
			}
		}
	}

	void move_to_island() {
		// logic to move civilians to the island suburb (prison)
	}

	void evacuate_to_closest_suburbs() {
		// logic to evacuate civilians to the closest suburbs in case of a natural disaster
	}

	void public_transport_activity() {
		// logic to show public transport activities on each turn
		cout << "Public transport currently unavailable" << endl;
	}

	void show_suburb_data(SuburbPopulation suburb, const char* const suburb_names[]) {
		// logic to show latest data of suburbs on each turn
		cout << "-----------------------------" << endl;
		cout << "Suburb: " << suburb_names[suburb.suburb] << endl;
		cout << "Population: " << suburb.population << endl;
		cout << "Workers: " << suburb.civilians.workers.second << endl;
		cout << "Teachers: " << suburb.civilians.teachers.second << endl;
		cout << "Artists: " << suburb.civilians.artists.second << endl;
	}
};

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
	City::SuburbPopulation low_residential_population = { City::low_residential, brisbane.distribute_civilians(popul(mt)) };
	City::SuburbPopulation high_residential_population = { City::high_residential, brisbane.distribute_civilians(popul(mt)) };
	City::SuburbPopulation entertainment_population = { City::entertainment, brisbane.distribute_civilians(popul(mt), 0.3f, 0.2f, 0.5f) };
	City::SuburbPopulation industrial_population = { City::industrial, brisbane.distribute_civilians(popul(mt), 0.7f, 0.1f, 0.2f) };
	City::SuburbPopulation commercial_population = { City::commercial, brisbane.distribute_civilians(popul(mt)) };
	City::SuburbPopulation administrative_population = { City::administrative, brisbane.distribute_civilians(popul(mt), 0.5f, 0.4f, 0.1f) };
	City::SuburbPopulation train_station_population = { City::train_station, brisbane.distribute_civilians(10000) };
	City::SuburbPopulation prison_population = { City::prison, brisbane.distribute_civilians(500) };

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
		this_thread::sleep_for(chrono::seconds(1)); // pause for a moment to simulate time passing

		// show suburb data and public transport activities
		brisbane.show_suburb_data(low_residential_population, suburb_names);
		brisbane.show_suburb_data(high_residential_population, suburb_names);
		brisbane.show_suburb_data(entertainment_population, suburb_names);
		brisbane.show_suburb_data(industrial_population, suburb_names);
		brisbane.show_suburb_data(commercial_population, suburb_names);
		brisbane.show_suburb_data(administrative_population, suburb_names);
		brisbane.show_suburb_data(train_station_population, suburb_names);
		brisbane.show_suburb_data(prison_population, suburb_names);
		brisbane.public_transport_activity();

		// randomly move civilians between suburbs
		brisbane.move_civilians_randomly(low_residential_population, all_suburbs);
		brisbane.move_civilians_randomly(high_residential_population, all_suburbs);
		brisbane.move_civilians_randomly(entertainment_population, all_suburbs);
		brisbane.move_civilians_randomly(industrial_population, all_suburbs);
		brisbane.move_civilians_randomly(commercial_population, all_suburbs);
		brisbane.move_civilians_randomly(administrative_population, all_suburbs);
		brisbane.move_civilians_randomly(train_station_population, all_suburbs);

		// randomly trigger natural disaster every n turns
		if (uniform_int_distribution<int>(1, 10)(mt) == 1) { // 10% chance of natural disaster each turn
			cout << "Natural disaster occurred! Evacuating civilians to closest suburbs..." << endl;
			brisbane.evacuate_to_closest_suburbs();
		}

		// ask player if they want to continue simulation
		char choice;
		cout << "Continue simulation? (y/n): ";
		cin >> choice;
		continue_simulation = (choice == 'y' || choice == 'Y');
	}

	cout << "End of city simulation " << endl;

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