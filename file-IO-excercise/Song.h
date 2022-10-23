#pragma once
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <cstddef>

// Class that defines a song title, artist
// and genre. Each member is a std::string
// with a maximum size of 255. Trying to 
// input anything larger will throw an 
// invalid argument exception.
// Default construction consists of empty
// members but can be passed title, artist,
// and genre in that order.
class song
{
private:
	std::string title{};
	std::string artist{};
	std::string genre{};

public:
	//Constructors
	// Default construction consists of empty
	// members but can be passed title, artist,
	// and genre in that order.
	song() = default;
	song(std::string title, std::string artist, std::string genre);

	//Setters
	// checks for maximum size of 255 and throws
	// invalid argument exception if too long
	void set_title(std::string title_in);
	void set_artist(std::string artist_in);
	void set_genre(std::string genre_in);

	//Getters
	const std::array<std::string,3> get_information();
	const std::string get_title();
	const std::string get_artist();
	const std::string get_genre();

	// Overloaded Operators
	// less than operator designed to compare
	// artists even if they contain whitespace
	// and ignores case
	bool operator<(const song& rhs);

	// All three members must be equal
	// to return true
	bool operator==(const song& rhs);
};

// Class that contains several songs in two 
// seperate vectors. One is sorted in the
// order they were entered and the other
// sorted alphabetically by artist.
class song_collection
{
private:
	std::vector<song> unsorted_collection{};
	std::vector<song> sorted_collection{};
	std::string build_string(std::vector<char> char_vec);

public:
	// Setter
	// will automatically sort the sorted_collection
	// upon adding.
	void add_song(song new_song);

	// removes song based on index location in
	// sorted container
	void remove_song(unsigned int index);

	// Getters
	// get_song will pull information from the sorted
	// container or can use other methods
	// to get entire vectors as required
	song const get_song(unsigned int index);
	std::vector<song> const get_unsorted();
	std::vector<song> const get_sorted();
	
	// Helper functions to prepare/unpack
	// data for storage
	std::vector<std::vector<char>> const get_for_storage();
	void load_from_storage(std::vector<std::vector<char>>);
};

// Simple console menu to navigate the song
// class and add songs
class song_interface
{
private:
	song_collection my_collection{};

	// Used to catch bad user input
	std::array<unsigned int, 4> valid_input{ 1, 2, 3, 4 };

	// Helper function to ensure proper formatting of
	// songs before display
	std::string adjust(std::string input, unsigned int size);

	// displays the provided songs in a formatted
	// manner. Takes a vector of song objects as
	// an argument to display
	void display_songs(std::vector<song> songs);

	// home screen where choices are made
	// returns a number based on the user's choice
	unsigned int home_menu();

	// Function to get song information from
	// user and saves to song collection
	void input_song();

	// Wrappers around display_songs to make
	// calling clearner
	void display_sorted();
	void display_unsorted();

public:
	// Program to run menu and catch exceptions
	// Exiting with 0 indicates no issues encountered
	// exiting with 1 indicates a crash.
	unsigned int run();
};