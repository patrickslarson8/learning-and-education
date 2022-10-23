#pragma once
// Disables conversion warning for the
// default engine seed
//#pragma warning( disable : 4244 )
#include <iostream>
#include <random>
#include <ctime>
#include <map>
#include <utility>

class Calculator
{
private:
    std::random_device seeder;
    std::time_t seed = seeder.entropy() ? seeder() : time(nullptr);

    // Conversion to suppress compiler warning
    unsigned int uint_seed = (unsigned int)seed;
    std::default_random_engine generator{uint_seed};

public:

    // Returns the time passed until the
    // next event happens according to 
    // an exponential distribution.
    // Input: number expected to complete in an hour
    // Output: time elapsed in milliseconds
    double get_event_time(double hourly_rate);

    // Takes a vector of times and returns their
    // average time
    double calculate_avg_time(std::vector<double> times);
};

class EventManager
{
private:
    // Upcoming events are stored in a map where the
    // key consists of an event time (double) and the
    // value is an event type (unsigned int).
    // The event type has no meaning and must be
    // decoded by the application layer.
    std::multimap<const double, const unsigned int> upcoming_events;
    double current_time{ 0 };
    unsigned int current_event{ 0 };

public:
    // returns current time and event
    const double get_current_time();
    const unsigned int get_current_event();

    // Updates the current time and removes
    // next event from queue
    void advance_event();

    // Takes event time in ms and event
    // type and slots it into its correct
    // spot based on time
    void add_event(const double event_time, const unsigned int event_type);

    // returns true if there are any events remaining
    const bool has_events();
};
