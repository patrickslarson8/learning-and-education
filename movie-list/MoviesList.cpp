#include <iostream>
#include <list>

class Movies
{
private:
    std::list<std::string> movie_list{};

    // Prints welcome menu to console and waits for
    // user input. Returns the input as integer. Does
    // not check for validity
    unsigned int display_menu() const
    {
        // Takes input as std::string in order to prevent
        //  errors from erroneous input
        unsigned int choice{ 0 };

        while (choice == 0)
        {
            std::cout << "Welcome to Movie Ranker! Please choose an option:"
                << std::endl
                << "1: Enter new list of movies" << std::endl
                << "2: Display current list of movies" << std::endl
                << "3: Change the ranking of a movie" << std::endl
                << "4: Quit program" << std::endl;
            std::cin >> choice;

            // Error checking logic
            if ((choice == 1) || (choice == 2) ||
                (choice == 3) || (choice == 4))
            {
                return choice;
            }
            else
            {
                std::cout << "Please choose a valid option from the list." << std::endl;
                choice = 0;
            }
        }
    }

    // Retrieves a single movie title as input
    // Returns that movie title as a std::string
    std::string input_movie()
    {
        std::string movie{};
        std::cin >> movie;

        // Simple error checking to make sure something was entered
        if (movie.size() > 50) 
        {
            std::cout << "Movie title too long, shortened to first 50 characters."
                << std::endl;
            movie.resize(50);
        }

        return movie;
    }

    // Prints screen to input movies and stores them into
    // stl list container. Ranking is inferred from position
    // in the list. Clears any previously stored movies
    void input_movie_list()
    {
        movie_list.clear();
        // Show prompt and get input
        std::cout << "Please input 5 movies pressing the enter key after each one."
            << std::endl << "Start with your favorite movie first." << std::endl;

        // Store inputs into list container
        for (int i{ 0 }; i < 5; i++)
        {
            movie_list.emplace_back(input_movie());
            std::cout << "Enter the next movie." << std::endl;
        }

        std::cout << "All movies stored." << std::endl;
    }

    // Prints all movies stored in list to display
    void const display_movies()
    {
        if (movie_list.size() == 0) { std::cout << "No movies input." << std::endl; }
        else
        {
            unsigned int rank{ 1 };
            std::cout << "Here are your movies ranked in order:" << std::endl;

            for (auto iter{ cbegin(movie_list) }; iter != cend(movie_list); ++iter)
            {
                // Print movie ranking followed by title
                std::cout << rank << ": " << *iter << std::endl;
                rank++;
            }
        }
    }

    // Overloaded display menu function prints
    // a selection character next to a specific
    // movie whose position in the movies_list
    // is passed in as an argument
    void const display_movies(unsigned int const selected_movie)
    {
        if (movie_list.size() == 0) { std::cout << "No movies input." << std::endl; }
        else
        {
            unsigned int rank{ 1 };
            
            for (auto iter{ cbegin(movie_list) }; iter != cend(movie_list); ++iter)
            {
                // display selection character if on the selected movie
                if (rank == selected_movie) { std::cout << ">"; }
                
                // Print movie ranking followed by title
                std::cout << rank << ": " << *iter << std::endl;
                rank++;
            }
        }
    }

    // Allows user to change the ranking of the
    // movies in the list
    void change_ranking()
    {
        unsigned int selection{0};
        while (true)
        {
            if (selection == 1)
            {
                manual_reranking();
                return;
            }
            else if (selection == 2)
            {
                selection_reranking();
                return;
            }
            else
            {
                std::cout << "Please choose how you would like to rank the movies:"
                    << std::endl
                    << "1: Enter the new ranks for the whole list at once."
                    << std::endl
                    << "2: Select one movie and move it up and down the list."
                    << std::endl;
                std::cin >> selection;
            }
        }
    }

    void manual_reranking()
    {
        //Initialize required temporary variables
        unsigned int input{ 0 };
        std::list<unsigned int> new_rank_order{};
        unsigned int old_position{};
        std::list<std::string> temp_list{};
        std::string movie_title{};
        auto movies_iter{ cbegin(movie_list) };

        // Show menu
        std::cout
            << "Please enter the number associate with each movie in the order you want to rank them."
            << std::endl << "Press enter between each number."
            << std::endl << "For example, entering 23451 would move the first movie to the fifth place."
            << std::endl;

        // Get desired new order
        for (unsigned int i{0}; i < 5; i++)
        {
            std::cin >> input;
            new_rank_order.emplace_back(input);
        }

        // Get the old position of each movie title
        // and emplace them in a temporary list
        // before swapping the temporary list with the
        // movie_list
        for (auto ranking_iter{ cbegin(new_rank_order) }; ranking_iter != cend(new_rank_order); ++ranking_iter)
        {
            old_position = *ranking_iter;

            // Reset movie_iter to beginning and increment
            // to desired position
            movies_iter = cbegin(movie_list);
            for (unsigned int i{1}; i < old_position; i++) { movies_iter++; }
            // store movie title for placement in temp list
            movie_title = *movies_iter;
            temp_list.push_back(movie_title);
        }

        // Store the new order and
        // Display the new order
        movie_list.swap(temp_list);
        display_movies();
    }

    void selection_reranking()
    {
        unsigned int selection{ 0 };
        char cursor_control{ 'n' };

        // Show initial menu to get movie to move up and down
        display_movies();
        std::cout << "Enter the number of the movie you would like to select."
            << std::endl;
        std::cin >> selection;

        while (cursor_control != 'c')
        {
            // Show user has selected a movie
            display_movies(selection);
            std::cout << "Move the movie up and down the list with w and s."
                << std::endl << "Move the cursor up and down with e and d."
                << std::endl << "Confirm the new ranking with c."
                << std::endl;

            // Get new input and action it
            std::cin >> cursor_control;
            switch (cursor_control)
            {
            case 'w': selection = move_movie_up(selection); break;
            case 's': selection = move_movie_down(selection); break;
            case 'e': selection = move_cursor_up(selection); break;
            case 'd': selection = move_cursor_down(selection); break;
            case 'c': break;
            }
        }
    }

    // Cursor utilities move the cursor
    // up and down and control wrapping around
    // at end of the list
    unsigned int move_cursor_down(unsigned int selection)
    {
        // increments selection number and wraps around to
        // 0 if exceed the num of movies
        selection++;
        if (selection > 5) { selection = 1; }
        return selection;
    }

    unsigned int move_cursor_up(unsigned int selection)
    {
        // Decrements selection number and wraps around to
        // top if going below 1
        selection--;
        if (selection == 0) { selection = 5; }
        return selection;
    }

    // Movie rearranging utilities work by
    // leveraging the movie_swap helper
    unsigned int move_movie_down(unsigned int selection)
    {
        // Wrap around if at top
        if (selection == 5) { movie_swap(selection, 1); }
        else { movie_swap(selection, selection+1); }

        // Update cursor location
        return move_cursor_down(selection);
    }

    unsigned int move_movie_up(unsigned int selection)
    {
        // Wrap around if at bottom
        if (selection == 1) { movie_swap(selection, 5); }
        else { movie_swap(selection, selection - 1); }

        // Update cursor location
        return move_cursor_up(selection);
    }

    // Helper function takes two movie indices as input
    // and swaps their places
    void movie_swap(unsigned int movie1_index, unsigned int movie2_index)
    {
        std::list<std::string> temp_movies{};
        std::string movie1{};
        std::string movie2{};

        // Get the titles of movies 1 and 2
        unsigned int index{ 1 };
        for (auto iter{ cbegin(movie_list) }; iter != cend(movie_list); ++iter)
        {
            if (movie1_index == index) { movie1 = *iter; }
            if (movie2_index == index) { movie2 = *iter; }
            index++;
        }

        // Swap the names into their correct position
        index = 1;
        for (auto iter{ cbegin(movie_list) }; iter != cend(movie_list); ++iter)
        {
            if (movie1_index == index) { temp_movies.emplace_back(movie2); }
            else if (movie2_index == index) { temp_movies.emplace_back(movie1); }
            else { temp_movies.emplace_back(*iter); }
            index++;
        }

        movie_list = temp_movies;
    }

public:
    int run()
    {
        while (true)
        {
            switch (display_menu())
            {
            case 1: input_movie_list(); break;
            case 2: display_movies(); break;
            case 3: change_ranking(); break;
            case 4: return 1; break;
            }
        }
    }
};


int main()
{
    Movies myMovies;
    return myMovies.run();
}