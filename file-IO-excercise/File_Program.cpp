#include "Song.h"
#include "FileHandler.h"
#include <iostream>

int main()
{
	// Create simple song collection
	song_collection my_collection{};
	song new_song{ "New Song", "Good Artist", "Pop" };
	song new_song2{ "A New Song", "A Good Artist", "Pop" };
	song new_song3{ "An old Song", "A bad Artist", "Pop" };
	my_collection.add_song(new_song);
	my_collection.add_song(new_song2);
	my_collection.add_song(new_song3);

	// Save file and read file
	file_handler handler{};
	handler.save_to_text_file(my_collection.get_for_storage(), "song_text.txt");
	my_collection.load_from_storage(handler.read_from_text_file("song_text.txt"));

	// Now in binary
	handler.save_to_bin_file(my_collection.get_for_storage(), "song_text.dat");
	my_collection.load_from_storage(handler.read_from_bin_file("song_text.dat"));
	
	std::vector sorted = my_collection.get_sorted();
	for (auto iter = sorted.begin(); iter != sorted.end(); ++iter)
	{
		song temp = *iter;
		std::cout << temp.get_artist() << '\n';
	}
}