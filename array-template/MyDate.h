#pragma once
#include <iostream>
#include <string>

namespace N
{

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
        Date();

        Date(int month_in, int day_in, int year_in);

        Date(std::string month_name_in, int day_in, int year_in);

        // Setters provided that allow you to individually set
        // the month, month_name, day, and year properties.

        // Sets the day property of date. Valid input ranges
        // from 1-31. If the month property is already set
        // then it will check if that day is possible that month
        void set_day(int day_in);

        void set_month(int month_in);

        void set_month_name(std::string month_name_in);

        void set_year(int year_in);

        int get_day();

        int get_month();

        int get_year();

        bool get_leap_year();

        std::string get_month_name();

    private:
        // Properties holding the date value
        int day;
        int month;
        int year;

        // Utility properties for internal use
        bool is_leap_year;
        std::string month_name;
        std::string month_names[12] =
        { "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December" };


        // Utility functions used to validate inputs before setting properties
        void sync_month(int month_num_in); // Used to set the month_name property when the month number is given

        bool is_day_possible(int check_day, int check_month, bool check_leap_year);
            // Takes day, month, and leap year as inputs
            // If day is a valid input according to the Gregorian
            // Calendar then return True. Else return False.

        void error(int error_type);
            // Prints an error message to the console
            // error_type can be several inputs
            // 0: Impossible input given

    };
}