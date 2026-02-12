#include <iostream>
#include <utility>
#include <string>
#include <random>
#include <chrono>
#include <thread>
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

	struct PublicTransport {
		string type; // e.g., bus, tram, ferry
		unsigned int capacity;
	};

	bool is_island(Suburbs suburb) {
		return suburb == prison; // prison is the island suburb as per reference image
	}

	bool is_coastal(Suburbs suburb) {
		return suburb == industrial || suburb == high_residential; // coastal suburbs
	}

	void move_civilians_randomly() {
		// logic to move civilians randomly between suburbs based on public transport availability
	}

	void move_to_island() {
		// logic to move civilians to the island suburb (prison)
	}

	void evacuate_to_closest_suburbs() {
		// logic to evacuate civilians to the closest suburbs in case of a natural disaster
	}

	void public_transport_activity() {
		// logic to show public transport activities on each turn
	}

	void show_suburb_data() {
		// logic to show latest data of suburbs on each turn
	}
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

	// initialize public transport
	City::PublicTransport bus = { "bus", 50 };
	City::PublicTransport tram = { "tram", 100 };
	City::PublicTransport ferry = { "ferry", 200 };

	// simulate city activities for as many turns as player wants
	bool continue_simulation = true;

	while (continue_simulation) {
		// show suburb data and public transport activities
		brisbane.show_suburb_data();
		brisbane.public_transport_activity();

		this_thread::sleep_for(chrono::seconds(1)); // pause for a moment to simulate time passing

		// randomly move civilians between suburbs
		brisbane.move_civilians_randomly();

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