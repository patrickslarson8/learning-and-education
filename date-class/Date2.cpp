#include <iostream>
#include <string>

class Date
// Contains day, month, year, and month_name properties
// All values are initialized to zero. Input is validated
// to ensure conformity with the Gregorian Calendar.
// Will not accept year values less than 0001 or greater
// than 3000.

{
public:
    // Constructors allow for initilization with three or no arguments.
    // If provided, the month, day, and year must be in that order.
    // The month can be an integer or a string. If a string, it must
    // be capitalized.
    Date() = default;

    Date(int month_in, int day_in, int year_in)
    {
        set_month(month_in);
        set_day(day_in);
        set_year(year_in);
    }

    Date(std::string month_name_in, int day_in, int year_in)
    {
        set_month_name(month_name_in);
        set_day(day_in);
        set_year(year_in);
    }

    // Setters provided that allow you to individually set
    // the month, month_name, day, and year properties.
    void set_day(int day_in)
    // Sets the day property of date. Valid input ranges
    // from 1-31. If the month property is already set
    // then it will check if that day is possible that month

    {
        if (day_in <= 0) //Reject 0 or negative values
        {
            error(0);
        }

        else if (month == 0) //Allow any valid day value if month not set
        {
            if (day_in <= 31) 
            {
                day = day_in;
            }
        }

        else //If a month is set then ensure the day is possible
        {
            if (is_day_possible(day_in, month, is_leap_year))
            {
                day = day_in;
            }
            else
            {
                error(0);
            }
        }
    }

    void set_month(int month_in)
    {
        if ((month_in <= 0) || (month_in > 12)) //Reject invalid inputs
        {
            error(0);
        }

        else if (day == 0) //If day has not been set, accept any valid input
        {
            month = month_in;
            sync_month(month_in);
        }

        else //if day has been set then make sure day is possible
            if (is_day_possible(day, month_in, is_leap_year))
            {
                month = month_in;
                sync_month(month_in);
            }

            else
            {
                error(0);
            }
    }

    void set_month_name(std::string month_name_in)
    {
        bool month_found{ false }; // Flag to track if the input was valid

        for (int i = 0; i <= 11; i++)
        {
            if (month_name_in == month_names[i]) // Search through the list of months for a match
            {
                set_month(i + 1);
                month_found = true;
            }
        }

        if (!month_found) // Print error message if input does not match a month name
        {
            error(0);
        }

    }

    void set_year(int year_in)
    {
        bool leap_year_holder {false};
        if ((year_in <= 0) || (year_in > 3000)) // Reject negative, 0, or over 3000 values
        {
            error(0);
        }
        else // For valid inputs, set the leap year property before checking if the day is valid
        {
            if ((year_in % 4) == 0)
            {
                leap_year_holder = true;
            }

            if (day == 0) // If day is not set, accept and valid input
            {
                year = year_in;
                is_leap_year = leap_year_holder;
            }
            else // If day is set, then ensure day is possible before setting
            {
                if (is_day_possible(day, month, leap_year_holder))
                {
                    year = year_in;
                    is_leap_year = leap_year_holder;
                }
                else
                {
                    error(0);
                }
            }
        }

    }

    int get_day()
    {
        if (day != 0) // Check if day has been set previously before returning
        {
            return day;
        }
        else
        {
            error(1);
        }
       
    }

    int get_month()
    {
        if (month != 0) // Check if month has been set previously before returning
        {
            return month;
        }
        else
        {
            error(1);
        }
        
    }

    int get_year()
    {
        if (year != 0) // Check if year has been set previously before returning
        {
            return year;
        }
        else
        {
            error(1);
        }
    }

    std::string get_month_name()
    {
        if (month != 0) // Check if month has been set previously before returning
        {
            return month_name;
        }
        else
        {
            error(1);
            return "Error: No month name set.";
        }
    }

private:
    // Properties holding the date value
    int day{0};
    int month{0};
    int year{0};

    // Utility properties for internal use
    bool is_leap_year = false;
    std::string month_name{};
    const std::string month_names[12]
        = { "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December" };


    // Utility functions used to validate inputs before setting properties
    void sync_month(int month_num_in) // Used to set the month_name property when the month number is given
    {
        month_name = month_names[month_num_in - 1];
    }

    bool is_day_possible(int check_day, int check_month, bool check_leap_year)
    // Takes day, month, and leap year as inputs
    // If day is a valid input according to the Gregorian
    // Calendar then return True. Else return False.
    {
        if (check_month == 2)
        {
            if (check_leap_year == false)

                if (check_day > 28) //In February, only accept day greater than 28 on leap year
                {
                    return false;
                }

                else
                {
                    return true;
                }

            else
            {
                if (check_day > 29)
                {
                    return false;
                }

                else
                {
                    return true;
                }
            }
        }

        else if ((check_month == 4) || (check_month == 6) || (check_month == 9) || (check_month == 11)) // Check the months that allow 30 days
        {
            if (check_day > 30)
                return false;
            else
                return true;
        }

        else // All other months allow up to 31 days
        {
            if (check_day > 31)
                return false;
            else
                return true;
        }
    }

    void error(int error_type)
    // Prints an error message to the console
    // error_type can be several inputs
    // 0: Impossible input given
    {
        std::cout << "Error: " << error_type << " \n";
        switch (error_type)
        {
            case 0:
                std::cout << "The input you provided is outside of expected bounds. Your input was not saved.\n";
                break;
            case 1:
                std::cout << "The output cannot be returned because the property has not been set.\n";
                break;
        }
    }
};


// Short program showing the use of the Date class.
int main()
{
    std::cout << "Date 1: Initilize using setters and run through two loops showing all months and possible days for non-leap years.\n";
    Date date1; // Using defaults initilizes values to 0

    date1.set_day(21);
    date1.set_month(1);
    date1.set_year(2019); // No leap year on this date

    for (int i = 1; i <= 12; i++) // Check all months set the name appropriately
    {
        date1.set_day(1);
        date1.set_month(i);
        std::cout << date1.get_month() << ": " << date1.get_month_name() << " | ";

        
        for (int j = 2; j <= 31; j++) // Check which days are allowed for each month
        {
            date1.set_day(j);
            std::cout << date1.get_day() << ", ";
        }
        std::cout << "\n";
    }
    
    Date date2(9, 21, 2020); // Using constructor and leap year
    std::cout << "\n\nDate 2: Made using a constructor passing in month, day, and year.\n";
    std::cout << "Day: " << date2.get_day() << " | Month: " << date2.get_month_name() << " | Year: " << date2.get_year() << "\n";

    date2.set_month(2); // Set to february and test leap year
    date2.set_day(29);
    std::cout << date2.get_year() << " is a leap year so we allow up to " << date2.get_day() << " days in " << date2.get_month_name() << ".\n";

    Date date3;
    std::cout << "\n\nDate 3: The class will print an error message if you try to receive a value not set yet.\n";
    date3.get_day();
    date3.get_month();
    date3.get_month_name();
    date3.get_year();

    Date date4(std::string("January"), 30, 2801);
    std::cout << "\nHere's an example using the month name constructor: " << date4.get_month() << " (" << date4.get_month_name() << "), " << date4.get_day() << ", " << date4.get_year() << "\n";
}

