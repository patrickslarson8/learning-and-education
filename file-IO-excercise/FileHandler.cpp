#include "FileHandler.h"

void file_handler::save_to_text_file(std::vector<std::vector<char>> data_in, std::string file_name)
{
    std::ofstream output_file(file_name, std::ios::out);

    // Throw exception if cannot open
    if (!output_file)
    {
        throw std::ios_base::failure("Unable to open specified file.");
    }

    // Iterate through to get sub_vectors
    for (auto iter = data_in.begin(); iter != data_in.end(); ++iter)
    {
        // iterate through subvectors and save to file
        std::vector sub_vec = *iter;
        for (auto sub_it = sub_vec.begin(); sub_it != sub_vec.end(); ++sub_it)
        {
            output_file << *sub_it;
        }
        
        // delimit each subvector with newline char
        output_file << '\n';
    }
    // clean up
    output_file.close();
}

std::vector<std::vector<char>> file_handler::read_from_text_file(std::string file_name)
{
    std::ifstream input_file(file_name, std::ios::in);

    // throw exception if can't open file
    if (!input_file) { throw std::ios_base::failure("Unable to open specified file."); }

    // Create temporary containers to build file
    // information in the memory
    std::vector<std::vector<char>> primary_vec{};
    std::vector<char> sub_vec{};

    while (!input_file.eof())
    {
        // Get next character
        char temp_char{};
        temp_char = input_file.get();

        // catch the newline breaks to break up vectors
        if (temp_char == '\n')
        {
            primary_vec.push_back(sub_vec);
            sub_vec.clear();
        }

        // Otherwise save char to sub vector
        else
        {
            sub_vec.push_back(temp_char);
        }
    }

    //After looping through all information then return
    return primary_vec;
}

void file_handler::save_to_bin_file(std::vector<std::vector<char>> data_in, std::string file_name)
{
    std::ofstream output_file(file_name, std::ios::out | std::ios::binary);

    if (!output_file)
    {
        throw std::ios_base::failure("Unable to open specified file.");
    }

    // save total number of vectors to make reading back easier
    int outer_size = data_in.size();
    output_file.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));

    // Iterate through to get sub_vectors
    for (auto iter = data_in.begin(); iter != data_in.end(); ++iter)
    {
        // iterate through subvectors and save to file
        std::vector sub_vec = *iter;
        int size = std::size(sub_vec);

        // store that vector size to tell file reader how much to read
        output_file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        // store that vector as a whole
        for (int i{ 0 }; i < size; i++)
        {
            char temp = sub_vec[i];
            output_file.write(reinterpret_cast<const char*>(&temp), sizeof(temp));
        }
        
    }
    // clean up
    output_file.close();
}

std::vector<std::vector<char>> file_handler::read_from_bin_file(std::string file_name)
{
    std::ifstream input_file(file_name, std::ios::in|std::ios::binary);

    if (!input_file) { throw std::ios_base::failure("Unable to open specified file."); }

    // Create temporary containers to store
    // information
    std::vector<std::vector<char>> primary_vec{};
    std::vector<char> sub_vec{};
    int outer_size{ 0 };
    int size{ 0 };

    // get total number of vectors to loop through
    input_file.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));

    for (int i{0}; i < outer_size; i++)
    {
        // get size to read
        input_file.read(reinterpret_cast<char*>(&size), sizeof(size));

        // read vector in based on size
        for (int i{ 0 }; i < size; i++)
        {
            char temp{};
            input_file.read(reinterpret_cast<char*>(&temp), sizeof(temp));
            sub_vec.push_back(temp);
        }

        // prepare for next vectors
        primary_vec.push_back(sub_vec);
        sub_vec.clear();
    }

    //After looping through all information then return
    return primary_vec;
}
