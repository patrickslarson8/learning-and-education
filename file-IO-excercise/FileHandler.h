#pragma once
#include <cstddef>
#include <vector>
#include <string>
#include <fstream>
#include <exception>

// Takes an vector of vector of chars and a string representing
// the file location. This allows the using code
// to predictably break up the data to be written into
// chunks that are easier to re-compose into the desired
// data object for use. Individual bytes can be converted
// to chars for more flexible usage, but this must be
// done prior to passing to file_handler.
// The text and binary version use different methods
// to delimit the individual vectors. See comments
// before each function below.
// 
// Argument format:
// std::vector<std::vector<char>> vector_in
// std::string file_location_in
//
// Example data format:
// Using this to store the string "I love objects"
// could look like this.
// 1. First decide how to decompose the string. We will
// break it up into "I" and "love" and "objects".
// 
// 2. Convert these to byte arrays (not performed in this file)
// sub_vector_1 = "I"
// sub_vector_2 = "love"
// sub_vector_3 = "objects"
// 
// 3. Load these into the master array
// vector_in[0] = sub_vector_1
// etc...
//
// 4. Pass vector_in to file_save with location
//
// Assumptions: Quoted strings and newline characters
// are not used when saving/reading from a text file.
// Individual vectors are not massive in length as their
// size information is limited to one integer. There is not
// a set length as this is compiler specific.
//
// Will throw an ios_base::failure if unable to open
// the file passed in

class file_handler
{
private:
public:
	// Used for saving textual objects
	// assumes that none of the data passed in is a newline character
	// to save data that uses newlines, use the save to binary file function
	void save_to_text_file(std::vector<std::vector<char>> data_in, std::string file_name);
	std::vector<std::vector<char>> read_from_text_file(std::string file_name);
	
	// Binary files delimit using the size of the vectors
	// which assumes your vectors size fits into an integer
	void save_to_bin_file(std::vector<std::vector<char>> data_in, std::string file_name);
	std::vector<std::vector<char>> read_from_bin_file(std::string file_name);
};