#include "MyDate.h"
#include <iostream>
#include <string>

using namespace N;

// Constructors allow for initilization with three or no arguments.
// If provided, the month, day, and year must be in that order.
// The month can be an integer or a string. If a string, it must
// be capitalized.
Date::Date() = default;
//{
//    set_day(1);
//    set_month(1);
//    set_year(1);
//}

Date::Date(int month_in, int day_in, int year_in)
{
    set_month(month_in);
    set_day(day_in);
    set_year(year_in);
}

Date::Date(std::string month_name_in, int day_in, int year_in)
{
    set_month_name(month_name_in);
    set_day(day_in);
    set_year(year_in);
}

// Setters provided that allow you to individually set
// the month, month_name, day, and year properties.
void Date::set_day(int day_in)
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

void Date::set_month(int month_in)
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

void Date::set_month_name(std::string month_name_in)
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

void Date::set_year(int year_in)
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

int Date::get_day()
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

int Date::get_month()
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

int Date::get_year()
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

std::string Date::get_month_name()
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

bool Date::get_leap_year() { return is_leap_year; }

// Utility functions used to validate inputs before setting properties
void Date::sync_month(int month_num_in) // Used to set the month_name property when the month number is given
{
    month_name = month_names[month_num_in - 1];
}

bool Date::is_day_possible(int check_day, int check_month, bool check_leap_year)
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

void Date::error(int error_type)
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