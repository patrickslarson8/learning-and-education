#include "SimulationApp.h"

void Application::populate_arrival_times()
{
	double temp_current_time{ 0 };
	double arrival_interval{ 0 };
	while (temp_current_time < time_open_per_day)
	{
		arrival_interval = calc.get_event_time(arrival_rate);
		temp_current_time += arrival_interval;
		event_manager.add_event(temp_current_time, 1);
	}
}

void Application::add_to_cred_queue()
{
	// Create new person object
	Passenger new_person{};

	// If station is open advance directly to it
	if (!credential_station.is_occupied())
	{
		// Cycle them through the queue quickly to ensure
		// average queueing time is correct
		credential_queue.add_passenger(new_person, current_time);
		new_person = credential_queue.remove_passenger(current_time);

		// Send them to the credential station
		// Calculate station serving time and
		// update event manager
		credential_station.start_serving_passenger(new_person, current_time);
		double credential_validation_time = calc.get_event_time(credential_rate);
		event_manager.add_event(credential_validation_time + current_time, 2);
	}
	
	// Add that person to the credentials queue
	else
	{
		credential_queue.add_passenger(new_person, current_time);
	}
}

void Application::advance_to_cred_station()
{	
	// get person from credentialqueue
	Passenger person = credential_queue.remove_passenger(current_time);

	//Add that person to the credentials station
	credential_station.start_serving_passenger(person, current_time);

	// Determine how long it will take
	double credential_validation_time = calc.get_event_time(credential_rate);
	event_manager.add_event(credential_validation_time + current_time, 2);
}

void Application::advance_to_sec_queue()
{
	Passenger person = credential_station.stop_serving_passenger(current_time);

	// If a station is open advance directly to it
	for (int i{ 0 }; i < 4; i++)
	{
		if (scanning_stations[i].is_occupied() == false)
		{
			// Cycle them through scanning queue quickly
			// to ensure correct average times
			scanning_queue.add_passenger(person, current_time);
			person = scanning_queue.remove_passenger(current_time);

			// Send to open security station
			scanning_stations[i].start_serving_passenger(person, current_time);
			double scanning_validation_time = calc.get_event_time(scan_rate);
			event_manager.add_event(scanning_validation_time + current_time, i + 3);
			return;
		}
	}
	
	// pass them to the scanning queue
	scanning_queue.add_passenger(person, current_time);
}

void Application::advance_to_sec_station()
{
	// get person from ScanningQueue
	Passenger person = scanning_queue.remove_passenger(current_time);

	//Add that person to the next available scanning station
	for (int i {0}; i<4; i++)
	{
		if (scanning_stations[i].is_occupied() == false) 
		{
			scanning_stations[i].start_serving_passenger(person, current_time);
			// Determine how long it will take
			// and update event manager
			double scanning_validation_time = calc.get_event_time(scan_rate);
			event_manager.add_event(scanning_validation_time + current_time, i + 3);
			return;
		}
	}
}

void Application::advance_to_gate(int station)
{
	Passenger person = scanning_stations[station].stop_serving_passenger(current_time);
	daily_customers++;
}

const unsigned int Application::get_total_passengers_served()
{
	return daily_customers;
}

const double Application::get_average_time_in_system()
{
	double total_avg{ 0 };
	total_avg += calc.calculate_avg_time(credential_queue.get_wait_times());
	total_avg += calc.calculate_avg_time(credential_station.get_wait_times());
	total_avg += calc.calculate_avg_time(scanning_queue.get_wait_times());

	double scan_avg{ 0 };
	for (auto iter = scanning_stations.begin(); iter != scanning_stations.end(); ++iter)
	{
		scan_avg += calc.calculate_avg_time(iter->get_wait_times());
	}

	scan_avg = scan_avg / 4;
	total_avg += scan_avg;

	// Convert to minutes
	return total_avg/ 60000;
}

const double Application::get_average_queue_time()
{
	double queue_avg{ 0 };

	queue_avg += calc.calculate_avg_time(credential_queue.get_wait_times());
	queue_avg += calc.calculate_avg_time(scanning_queue.get_wait_times());

	// Convert to minutes
	return queue_avg/ 60000;
}

const double Application::get_average_stage_one_time()
{
	double one_avg{ 0 };
	one_avg += calc.calculate_avg_time(credential_queue.get_wait_times());
	one_avg += calc.calculate_avg_time(credential_station.get_wait_times());

	// Convert to minutes
	return one_avg/ 60000;
}

const double Application::get_average_stage_two_time()
{
	double two_avg{ 0 };
	two_avg += calc.calculate_avg_time(scanning_queue.get_wait_times());

	double scan_avg{ 0 };
	for (auto iter = scanning_stations.begin(); iter != scanning_stations.end(); ++iter)
	{
		scan_avg += calc.calculate_avg_time(iter->get_wait_times());
	}

	scan_avg = scan_avg / 4;
	two_avg += scan_avg;

	// Convert to minutes
	return two_avg/60000;
}

void Application::run()
{
	populate_arrival_times();
	while (event_manager.has_events())
	{
		event_manager.advance_event();
		current_time = event_manager.get_current_time();
		switch (event_manager.get_current_event())
		{
		case 1: add_to_cred_queue(); break;
		case 2: advance_to_sec_queue(); break;
		case 3: advance_to_gate(0); break;
		case 4: advance_to_gate(1); break;
		case 5: advance_to_gate(2); break;
		case 6: advance_to_gate(3); break;
		}
		// Logic to fill empty stations if people are waiting
		if ((credential_queue.get_num_in_line() > 0) && (credential_station.is_occupied() == false))
		{
			advance_to_cred_station();
		}
		for (int i{ 0 }; i < scanning_stations.size(); i++)
		{
			if (scanning_stations[i].is_occupied() == false)
			{
				if (scanning_queue.get_num_in_line() > 0) { advance_to_sec_station(); }
			}
		}
	}
}