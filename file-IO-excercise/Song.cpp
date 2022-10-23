#include <iostream>
#include "Song.h"
#include <exception>
#include <algorithm>


// Song class defintions
song::song(std::string title, std::string artist, std::string genre)
{
    set_title(title);
    set_artist(artist);
    set_genre(genre);
}

void song::set_title(std::string title_in)
{
    if (title_in.size() > 255) 
    {
        throw std::invalid_argument( "Title name too long.");
    }
    title = title_in;
}

void song::set_artist(std::string artist_in)
{
    if (artist_in.size() > 255)
    {
        throw std::invalid_argument("Artist name too long.");
    }
    artist = artist_in;
}

void song::set_genre(std::string genre_in)
{
    if (genre_in.size() > 255)
    {
        throw std::invalid_argument("Genre name too long.");
    }
    genre = genre_in;
}

std::array<std::string, 3> const song::get_information()
{
    std::array<std::string, 3> info_array =
    { get_title(), get_artist(), get_genre() };
    return  info_array;
}

std::string const song::get_title()
{
    return title;
}

std::string const song::get_artist()
{
    return artist;
}

std::string const song::get_genre()
{
    return genre;
}


bool song::operator<(const song& rhs)
{
	// Compare the length of the shorter artist
	if (this->artist.length() > rhs.artist.length())
	{
		for (int i{ 0 }; i < rhs.artist.length(); i++)
		{
			// if individual character equal then go to next one
			// ignoring case
			if (std::tolower(this->artist[i]) == std::tolower(rhs.artist[i])) { continue; }
			else
			{
				// when different chars encountered use
				// normal less than comparison
				return (std::tolower(this->artist[i])) < (std::tolower(rhs.artist[i]));
			}
		}
		// if all else equal, consider the shorter one less than
		// the longer one
		return false;
	}

	// same as above but opposite
	else
	{
		for (int i{ 0 }; i < this->artist.length(); i++)
		{
			if (std::tolower(this->artist[i]) == std::tolower(rhs.artist[i])) { continue; }
			else
			{
				return (std::tolower(this->artist[i])) < (std::tolower(rhs.artist[i]));
			}
		}
		return true;
	}
}

bool song::operator==(const song& rhs)
{
    if (this->title != rhs.title) { return false; }
    if (this->artist != rhs.artist) { return false; }
    if (this->genre != rhs.genre) { return false; }
    return true;
}

void song_collection::add_song(song new_song)
{
    // Add to both containers
    unsorted_collection.push_back(new_song);
    sorted_collection.push_back(new_song);

    // Sort the sorted container
    std::sort(sorted_collection.begin(), sorted_collection.end());
}

void song_collection::remove_song(unsigned int index)
{
    // iterate through the vector and remove the indicated
    // song
    unsigned int counter{ 0 };
    for (auto iter_sorted{ cbegin(unsorted_collection) }; iter_sorted != cend(unsorted_collection); ++iter_sorted)
    {
        if (counter == index) 
        {
            // When match is found, remove from both
            // sorted and unsorted containers
            auto iter_unsorted = find(unsorted_collection.begin(), unsorted_collection.end(), *iter_sorted);

            // Exception if song not found
            if (iter_unsorted == unsorted_collection.end())
            {
                throw std::invalid_argument("Song not found in unsorted collection.");
            }
            
            // Remove both songs
            unsorted_collection.erase(iter_unsorted);
            sorted_collection.erase(iter_sorted);
            return;
        }
        counter++;
    }

    // if input was invalid then throw an exception
    throw std::out_of_range("Given index is out of range of song collection.");
}

song const song_collection::get_song(unsigned int index)
{
    // iterate through the vector and get the indicated
    // song
    unsigned int counter{ 0 };
    for (auto iter{ cbegin(unsorted_collection) }; iter != cend(unsorted_collection); ++iter)
    {
        if (counter == index) { return *iter; }
        counter++;
    }
    
    // if input was invalid then throw an exception
    throw std::out_of_range("Given index is out of range of song collection.");
}

std::vector<song> const song_collection::get_unsorted()
{
    return unsorted_collection;
}

std::vector<song> const song_collection::get_sorted()
{
    return sorted_collection;
}

std::vector<std::vector<char>> const song_collection::get_for_storage()
{
	std::vector<std::vector<char>> outer_vector{};

	// Loop through all song objects in the unsorted collection
	// to preserve song entry order. Unordered collection is 
	// reconstructed when reading from storage
	for (auto iter = unsorted_collection.begin(); iter != unsorted_collection.end(); ++iter)
	{
		song inidividual_song = *iter;

		// store song elements in temporary variables
		std::string title = inidividual_song.get_title();
		std::string artist = inidividual_song.get_artist();
		std::string genre = inidividual_song.get_genre();

		// convert those to byte objects
		/*std::vector<char> vec_title{};
		std::copy(title.begin(), title.end(), std::back_inserter(vec_title));*/
		std::vector<char> vec_title(title.begin(), title.end());
		std::vector<char> vec_artist(artist.begin(), artist.end());
		std::vector<char> vec_genre(genre.begin(), genre.end());

		// and save to primary vector
		outer_vector.push_back(vec_title);
		outer_vector.push_back(vec_artist);
		outer_vector.push_back(vec_genre);
	}
	return outer_vector;
}

void song_collection::load_from_storage(std::vector<std::vector<char>> outer_vector)
{
	// purge old information
	unsorted_collection.clear();
	sorted_collection.clear();

	// iterate through loaded vector
	for (auto iter = outer_vector.begin(); iter != outer_vector.end(); ++iter)
	{
		// Assumes data is properly ordered
		// as title, artist, genre
		// First get individual byte vector, build a string from it
		// repeat for all song information
		std::string title = build_string(*iter);
		++iter;

		std::string artist = build_string(*iter);
		++iter;

		std::string genre = build_string(*iter);

		// save to collection
		song temp_song{ title, artist, genre };
		add_song(temp_song);
	}
}

std::string song_collection::build_string(std::vector<char> char_vec)
{
	std::string temp_string{};
	// Loop through and build a string
	for (auto iter = char_vec.begin(); iter != char_vec.end(); ++iter)
	{
		temp_string.push_back(*iter);
	}

	return temp_string;
}

// Song Interface
std::string song_interface::adjust(std::string input, unsigned int size)
{
	// Append an ellipsis if too long to display
	int input_size = input.size();
	if ((input_size - (int)size) > 0)
	{
		input.resize((size - 3));
		input.append("...");
	}
	// else pad with spaces
	else { input.resize(size, ' '); }

	return input;
}

void song_interface::display_songs(std::vector<song> songs)
{
	// Console header
	std::cout
		<< "|**********************************************************| \n"
		<< "Here are your songs in the order you entered them.\n"
		<< "|       Title       |*|     Artist     |*|      Genre      |\n";

	// Iterate through and show songs
	for (int i{}; i < songs.size(); i++)
	{
		// Print title
		std::cout << "|" << adjust(songs[i].get_title(), 19) << "|*";
		// Print artist
		std::cout << "|" << adjust(songs[i].get_artist(), 16) << "|*";
		// Print genre
		std::cout << "|" << adjust(songs[i].get_genre(), 17) << "|\n";
	}

	// Visually close menu
	std::cout
		<< "|**********************************************************|\n\n";

}

unsigned int song_interface::home_menu()
{
	// show menu
	std::cout
		<< "|**********************************************************|\n"
		<< "Welcome to Pat's Music Library!\n"
		<< "Please select an option below to begin.\n"
		<< "1: Add a new song to the library.\n"
		<< "2: Display all songs in the order they were entered.\n"
		<< "3: Display all songs sorted alphabetically by Artist.\n"
		<< "4: Quit program.\n"
		<< "|**********************************************************|\n\n";

	//get choice
	unsigned int choice{};
	std::cin >> choice;
	std::cin.ignore(1);

	// Validate choice
	if (std::find(valid_input.begin(), valid_input.end(), choice) == valid_input.end())
	{
		choice = 0;
		std::cin.clear();
		std::cin.ignore(100, '\n');
		throw std::invalid_argument("Invalid input. Please try again");
	}
	return choice;
}

void song_interface::input_song()
{
	std::cout
		<< "| **********************************************************| \n"
		<< "Please enter the new song information.\n"
		<< "Enter the song title, artist, and genre\n"
		<< "pressing enter between each input.\n";

	// set up memory for information
	std::string title{};
	std::string artist{};
	std::string genre{};

	// Get information

	std::getline(std::cin, title);
	std::getline(std::cin, artist);
	std::getline(std::cin, genre);

	song newSong{ title, artist, genre };

	// Put information into collection
	my_collection.add_song(newSong);

	// Visually close menu
	std::cout
		<< "|**********************************************************|\n\n";
}

void song_interface::display_sorted()
{
	display_songs(my_collection.get_sorted());
}

void song_interface::display_unsorted()
{
	display_songs(my_collection.get_unsorted());
}


// Program to run menu and catch exceptions
// Exiting with 0 indicates no issues encountered
// exiting with 1 indicates a crash.
unsigned int song_interface::run()
{
	unsigned int selection{};
	while (selection != 4)
	{
		try
		{
			selection = home_menu();
			switch (selection)
			{
			case 1: input_song(); break;
			case 2: display_unsorted();  break;
			case 3: display_sorted();  break;
			case 4: return 0; break;
			}
		}

		// Routine exceptions just start over from the beginning
		catch (const std::invalid_argument& e)
		{
			std::cout << e.what() << "\n";
			selection = 0;
		}

		catch (const std::out_of_range& e)
		{
			std::cout << e.what() << "\n";
			selection = 0;
		}

		catch (...)
		{
			std::cout << "Unexpected behavior encountered: Aborting.\n";
			return 1;
		}
	}
}