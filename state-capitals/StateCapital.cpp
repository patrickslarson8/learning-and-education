#include <iostream>
#include <set>
#include <string>

// Object that represents a state and
// its capital city as strings. Performs very 
// little input checking when settting so 
// caution must be used. Only valid for U.S. states.
class State
{
private:
    std::string state{};
    std::string capital{};

public:
    State() = default;
    State(std::string state_in, std::string capital_in)
    {
        set_state(state_in);
        set_capital(capital_in);
    }

    //Getters
    std::string get_state() const {return state;}
    std::string get_capital() const { return capital; }

    //Setters
    void set_state(std::string state_in)
    {
        // Checks if input is a valid length
        // Shortest state is Iowa (and a couple others)
        // longest are the Carolinas 
        if ((state_in.size() > 14) || (state_in.size() < 4))
        {
            std::cout <<
                "Error: Input not saved. Please enter a valid state name."
                << std::endl;
        }
        else { state = state_in; }
    }

    // Checks if input is a valid length
    // Shortest capital is Boiise (and a couple of others)
    // longest capital is Salt Lake City (and others)
    void set_capital(std::string capital_in)
    {
        if ((capital_in.size() > 14) || (capital_in.size() < 5))
        {
            std::cout <<
                "Error: Input not saved. Please enter a valid capital name."
                << std::endl;
        }
        else { capital = capital_in; }
    }
};

// Comparator function to sort set
// alphabetically by state names
bool cmp(State const  *lhs, State const  *rhs)
{
    return lhs->get_state() < rhs->get_state();
}

int main()
{
    // Load information into State objects
    State state1{ "Illinois","Springfield" };
    State state2{ "Wisconsin", "Madison" };
    State state3{ "California","Sacramento" };
    State state4{ "New York","Albany" };
    State state5{ "Alaska","Juneau" };

    // Create pointers to load into set
    State* ptr1{ &state1 };
    State* ptr2{ &state2 };
    State* ptr3{ &state3 };
    State* ptr4{ &state4 };
    State* ptr5{ &state5 };

    // Create set and initialize with values
    std::set<State*, decltype(cmp)*> state_set(cmp);
    state_set.insert(ptr1);
    state_set.insert(ptr2);
    state_set.insert(ptr3);
    state_set.insert(ptr4);
    state_set.insert(ptr5);

    // Iterate over set and print to screen
    std::cout << "State Name : Capital City" << std::endl;

    for (auto iter{ cbegin(state_set) }; iter != cend(state_set); ++iter)
    {
        std::cout
            << (*iter)->get_state()
            << " : "
            << (*iter)->get_capital()
            << std::endl;
    }
}