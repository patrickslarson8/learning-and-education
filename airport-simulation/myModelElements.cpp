#include "myModelElements.h"

// myQueue
Passenger myQueue::remove_passenger(double current_time)
{
	Passenger person = line.front();
	line.pop();
	person.stop_time_waiting(current_time);
	wait_times.push_back(person.get_time_waiting());
	person.reset_time_waiting();
	return person;
}

void myQueue::add_passenger(Passenger person, double current_time)
{
	person.start_time_waiting(current_time);
	line.push(person);
}

const unsigned int myQueue::get_num_in_line()
{
	return (unsigned int)line.size();
}

const std::vector<double> myQueue::get_wait_times()
{
	return wait_times;
}

// Station
void Station::start_serving_passenger(Passenger person, double current_time)
{
	person_in_service = person;
	person_in_service.start_time_waiting(current_time);
	occupied = true;
}

Passenger Station::stop_serving_passenger(double current_time)
{
	person_in_service.stop_time_waiting(current_time);
	wait_times.push_back(person_in_service.get_time_waiting());
	person_in_service.reset_time_waiting();
	occupied = false;
	return person_in_service;
}

const bool Station::is_occupied()
{
	return occupied;
}

const std::vector<double> Station::get_wait_times()
{
	return wait_times;
}


// Passenger
void Passenger::start_time_waiting(double start_time)
{
	time_start_waiting = start_time;
}

void Passenger::stop_time_waiting(double stop_time)
{
	time_stop_waiting = stop_time;
}

double Passenger::get_time_waiting()
{
	return time_stop_waiting - time_start_waiting;
}

void Passenger::reset_time_waiting()
{
	time_start_waiting = 0;
	time_stop_waiting = 0;
}