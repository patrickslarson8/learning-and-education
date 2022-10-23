#pragma once
#include "myModelElements.h"
#include "myUtilities.h"
#include <vector>

class Application
{
private:
	// Tunable parameters
	// Defaults are what is required in assignment
	// but can specify different numbers when
	// constructing Application object
	const double time_open_per_day{72000000}; //20 hrs in ms
	const double arrival_rate{90};//per hour
	const double credential_rate{120};//avg 30 sec is 120 per hr
	const double scan_rate{24};//avg 2.5 min is 24 per hr

	// Build portions of the simulation
	myQueue credential_queue{};
	Station credential_station{};
	myQueue scanning_queue{};
	std::vector<Station> scanning_stations{ {},{},{},{} };

	// Load required utilities
	// Eventmanager is used to keep track of the
	// next event to action. It is a multimap of
	// event times (double time in milliseconds) 
	// and event types (int). Events are as follows
	// 1: passenger arrives and gets in CredentialQueue
	// 2: Passenger leaves Credential Station to ScanningQueue
	// 3: Passenger leaves ScanningStation1 to gate
	// 4: Passenger leaves ScanningStation2 to gate
	// 5: Passenger leaves ScanningStation3 to gate
	// 6: Passenger leaves ScanningStation4 to gate
	EventManager event_manager{};
	Calculator calc{};

	// Stat trackers
	double current_time{ 0 };
	unsigned int daily_customers{ 0 };

	// Calculates the next arrival time of passengers
	// to the airport until that time exceeds the
	// time open per day. This way all passengers
	// that arrived before closing are served even 
	// past closing time. Adds all of these events to
	// the event manager
	void populate_arrival_times();
	void add_to_cred_queue();
	void advance_to_cred_station();
	void advance_to_sec_queue();
	void advance_to_sec_station();
	void advance_to_gate(int station);

public:
	Application() = default;

	// Functions to retrieve stats after simulating a day
	const unsigned int get_total_passengers_served();

	// These return a time value in minutes
	const double get_average_time_in_system();
	const double get_average_queue_time();
	const double get_average_stage_one_time();
	const double get_average_stage_two_time();

	// Simulate a day at a time
	void run();
};