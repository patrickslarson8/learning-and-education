#include "myModelElements.h"
#include "myUtilities.h"
#include "SimulationApp.h"
#include <iostream>

int num_days{ 5 };

int main()
{

	int total_customers{ 0 };
	double total_avg_time{ 0 };
	double avg_queue_time{ 0 };
	double avg_stg_one{ 0 };
	double avg_stg_two{ 0 };

	// Run for five days
	for (int i{ 0 }; i < num_days; i++)
	{
		Application mySimulator;
		// Run the day
		mySimulator.run();

		// Store pertinent information
		int daily_customers = mySimulator.get_total_passengers_served();
		double daily_avg_time = mySimulator.get_average_time_in_system();
		double daily_avg_queue_time = mySimulator.get_average_queue_time();
		double daily_avg_stg_one = mySimulator.get_average_stage_one_time();
		double daily_avg_stg_two = mySimulator.get_average_stage_two_time();

		// Print out information
		std::cout
			<< "**************************************\n"
			<< "Total Passengers today: " << daily_customers
			<< "\n Avg total time in system: " << daily_avg_time
			<< "\n Avg time spent in queues: " << daily_avg_queue_time
			<< "\n Avg time spent in stage one: " << daily_avg_stg_one
			<< "\n Avg time spent in stage two: " << daily_avg_stg_two
			<< "\n**************************************\n";

		// Update running averages
		total_customers += daily_customers;
		total_avg_time += daily_avg_time;
		avg_queue_time += daily_avg_queue_time;
		avg_stg_one += daily_avg_stg_one;
		avg_stg_two += daily_avg_stg_two;
	}

	// Calculate final averages
	total_avg_time = total_avg_time/num_days;
	avg_queue_time = avg_queue_time / num_days;
	avg_stg_one = avg_stg_one / num_days;
	avg_stg_two = avg_stg_two / num_days;

	// Print out final summary
	std::cout
		<< "**************************************\n"
		<< "Total Passengers from all days: " << total_customers
		<< "\n Avg total time in system: " << total_avg_time
		<< "\n Avg time spent in queues: " << avg_queue_time
		<< "\n Avg time spent in stage one: " << avg_stg_one
		<< "\n Avg time spent in stage two: " << avg_stg_two
		<< "\n**************************************";
}