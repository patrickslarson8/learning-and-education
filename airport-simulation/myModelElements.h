#pragma once
#include <queue>

class Passenger
{
private:
	double time_start_waiting{};
	double time_stop_waiting{};
public:
	// pass in the current time as a double
	// to store when the passenger started waiting
	void start_time_waiting(double current_time);

	// same as start time
	void stop_time_waiting(double current_time);

	// returns the difference between the start
	// and stop times as a double
	double get_time_waiting();

	// resets the wait time for the passenger
	// to zero
	void reset_time_waiting();
};

class myQueue
{
private:
	// Properties
	std::queue<Passenger> line{};
	std::vector<double> wait_times{};

public:
	// pops passenger from queue and updates
	// that person's stop time. Adds their
	// wait time to the wait_times vector
	Passenger remove_passenger(double current_time);

	// adds passenger to line and updates
	// their start time
	void add_passenger(Passenger person, double current_time);

	// Getters
	const unsigned int get_num_in_line();
	const std::vector<double> get_wait_times();
};


class Station
{
private:
	Passenger person_in_service{};
	std::vector<double> wait_times{};
	bool occupied{ false };

public:
	// Occupies a station with a passenger object
	void start_serving_passenger(Passenger person, double current_time);

	// Frees a station of a passenger
	Passenger stop_serving_passenger(double current_time);

	// Getters
	const bool is_occupied();
	const std::vector<double> get_wait_times();
};
