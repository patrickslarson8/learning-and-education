#include <iostream>
#include <string>

class Counter
// Creates a counter that can be increased
// or decreased by single integers.
// A memory function allows you to
// add the current counter value and store
// it in a seperate value.
// Negatives are not allowed.

{
private:
    int count{};
    int memory{};

public:
    // Only one constructor is provided that starts
    // the counter and memory at 0.
    Counter() { std::cout << "Counter created.\n"; }

    // Counter controls
    void incrementCount() { count++; }
    void decrementCount()
    {
        if (count == 0)
        {
            std::cout << "Cannot store negative values. Counter remains at 0.\n";
        }
        else
        {
            count--;
        }
    }
    void resetCount() { count = 0; }
    const int getCount() { return count; }

    // Memory controls
    void addMem()
    {
        memory = memory + count;
    }
    void resetMem() { memory = 0; }
    const int getMem() { return memory; }

};

class UserInterface
{
private:
    Counter *counterObj;
    int userChoice{};
    
    // The menu options are stored here
    const std::string choices[6] =
    {
        "1: Increment Counter",
        "2: Decrement Counter",
        "3: Reset Counter",
        "4: Add Memory",
        "5: Reset Memory",
        "6: Quit"
    };

public:
    // Constructor assigns counter object
    UserInterface(Counter* counterIn)
    {
        counterObj = counterIn;
    }

    // Iterates through each element of the choices array
    // and prints them to the console
    void const listChoices()
    {
        for (int i = 0; i < 6; i++)
        {
            std::cout << choices[i] << std::endl;
        }
    }

    // Stores console input to the userChoice variable
    // then checks to make sure that was a valid input
    // resetting the variable if it was invalid
    void getChoice()
    {
        userChoice = 0;
        while (userChoice == 0)
        {
            std::cin >> userChoice;
            if ((userChoice < 1) || (userChoice > 6))
            {
                userChoice = 0;
                std::cout << "Invalid selection. Please choose one of the options listed."
                    << std::endl;
            }
        }
    }

    // Prints the current counter status to the console
    void const displayValues()
    {
        std::cout << "Counter: " << counterObj->getCount() <<
            " Memory: " << counterObj->getMem() << std::endl;
    }

    // The primary program logic is here
    // The interface will list all the choices
    // then wait for an input from the user.
    // Once an input is received then it executes
    // one of the counter objects' functions.
    void runCounter()
    {
        while (userChoice != 6)
        {
            listChoices();
            getChoice();
            switch (userChoice)
            {
            case 1: counterObj->incrementCount(); break;
            case 2: counterObj->decrementCount(); break;
            case 3: counterObj->resetCount(); break;
            case 4: counterObj->addMem(); break;
            case 5: counterObj->resetMem(); break;
            case 6: std::cout << "Quitting Program" << std::endl;  break;
            }
            displayValues();
        }
    }

};

int main()
{
    Counter counter1;
    UserInterface interface1(&counter1);
    interface1.runCounter();
}
