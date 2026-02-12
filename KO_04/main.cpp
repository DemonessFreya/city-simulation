#include <iostream>
#include <utility>
#include <string>
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
		int population;

		// city population categorized into three main groups: workers, teachers, artists
		struct Civilians {
			pair<string, int> workers;
			pair<string, int> teachers;
			pair<string, int> artists;
		};

		Civilians civilians;
	};

	struct PublicTransport {
		string type; // e.g., bus, tram, ferry
		int capacity;
	};

	bool is_island(Suburbs suburb) {
		return suburb == prison; // prison is the island suburb as per reference image
	}

	bool is_coastal(Suburbs suburb) {
		return suburb == industrial || suburb == high_residential; // coastal suburbs
	}

	void move_to_island() {
		// logic to move civilians to the island suburb (prison)
	}

	void evacuate_to_closest_suburbs() {
		// logic to evacuate civilians to the closest suburbs in case of a natural disaster
	}

	void move_civilians_randomly() {
		// logic to move civilians randomly between suburbs based on public transport availability
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

	// initialize suburb populations
	City::SuburbPopulation low_residential_population = { City::low_residential, 5000, { {"workers", 3000}, {"teachers", 1500}, {"artists", 500} } };
	City::SuburbPopulation high_residential_population = { City::high_residential, 8000, { {"workers", 5000}, {"teachers", 2000}, {"artists", 1000} } };
	City::SuburbPopulation entertainment_population = { City::entertainment, 3000, { {"workers", 1000}, {"teachers", 500}, {"artists", 1500} } };
	City::SuburbPopulation industrial_population = { City::industrial, 4000, { {"workers", 3500}, {"teachers", 300}, {"artists", 200} } };
	City::SuburbPopulation commercial_population = { City::commercial, 6000, { {"workers", 4000}, {"teachers", 1000}, {"artists", 1000} } };
	City::SuburbPopulation administrative_population = { City::administrative, 2000, { {"workers", 1200}, {"teachers", 500}, {"artists", 300} } };
	City::SuburbPopulation train_station_population = { City::train_station, 1000, { {"workers", 700}, {"teachers", 200}, {"artists", 100} } };
	City::SuburbPopulation prison_population = { City::prison, 500, { {"workers", 300}, {"teachers", 100}, {"artists", 100} } };

	// initialize public transport
	City::PublicTransport bus = { "bus", 50 };
	City::PublicTransport tram = { "tram", 100 };
	City::PublicTransport ferry = { "ferry", 200 };

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