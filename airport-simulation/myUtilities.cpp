#include "myUtilities.h"


// Calculator Functions
double Calculator::get_event_time(double hourly_rate)
    {
        std::exponential_distribution<> expoRandNums(hourly_rate);

        // Get the required random number
        double time_in_hours = expoRandNums(generator);

        // Convert to millisecond
        double time_elapsed = time_in_hours * 3600000;
        return time_elapsed;
    }
double Calculator::calculate_avg_time(std::vector<double> times)
{
    double sum{ 0 };
    for (auto iter = times.begin(); iter != times.end(); ++iter)
    {
        sum += *iter;
    }
    return sum / times.size();
}
;

const double EventManager::get_current_time() { return current_time; }

const unsigned int EventManager::get_current_event() { return current_event; }

void EventManager::advance_event()
{
    // Bail out if no more events
    if (has_events() == false) { return; }

    // Update current time
    auto iter = upcoming_events.begin();
    current_time = iter->first;
    current_event = iter->second;

    // Erase event at beginning of set
    upcoming_events.erase(iter);
}

void EventManager::add_event(const double event_time, const unsigned int event_type)
{

    std::pair<const double, const unsigned int>
        new_event{event_time, event_type};
    upcoming_events.insert(new_event);
}

const bool EventManager::has_events()
{
    return upcoming_events.empty() ? false : true;
}
//
//int main()
//{
    //Calculator myCalc;
    //double total_time{};
    //double next_arrival{};

    //for (int i{}; i < 40; i++)
    //{
    //   next_arrival = myCalc.get_next_arrival(40);
    //   total_time += next_arrival;
    //   std::cout << "Next: " << next_arrival
    //       << "  Total: " << total_time
    //       << std::endl;
    //}
  /*  std::random_device seeder;
    const auto seed = seeder.entropy() ? seeder() : time(nullptr);
    std::default_random_engine e(seed);
    std::exponential_distribution<> expoRandNums(40);

    double interArrivalTime = 0;
    double absoluteArrivalTime = 0;

    for (int i = 0; i < 40; i++)
    {
        double v = expoRandNums(e);
        absoluteArrivalTime += v;
        std::cout << "InterArrival: " << v <<
            "  ArrivalTime: " << absoluteArrivalTime
            << "  Minutes: " << absoluteArrivalTime * 60
            << std::endl;
    }*/

    //EventManager myEvents;
    //myEvents.add_event(0, 1);
    //myEvents.add_event(100, 2);
    //myEvents.add_event(50, 3);

    //auto next_event = myEvents.get_next_event();
    //auto next_time = myEvents.get_current_time();
    //std::cout << "type: " << next_event << " time: " << next_time << std::endl;

    //myEvents.advance_event();
    //next_event = myEvents.get_next_event();
    //next_time = myEvents.get_current_time();
    //std::cout << "type: " << next_event << " time: " << next_time << std::endl;

    //myEvents.advance_event();
    //next_event = myEvents.get_next_event();
    //next_time = myEvents.get_current_time();
    //std::cout << "type: " << next_event << " time: " << next_time << std::endl;
//}