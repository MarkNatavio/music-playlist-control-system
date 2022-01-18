/*
Group Names: Progga Chowdhury, Mark Natavio, Billy Davila
Class : CSC 21200-BC
Professor: Akshar Patel
*/

#include<iostream>
#include<vector>
#include<string>

#include<unistd.h>
#include<stdlib.h>

#include<locale>
#include<algorithm>
#include<cassert>

using namespace std;

class Song { // class definition for a Song object
    public:
        Song(string my_title = "The Principia", string my_artist_name = "I.N.", string my_duration_time = "1.6180") {
            title = my_title;
            artist_name = my_artist_name;
            duration_time = my_duration_time;
        }
        string get_title(){ return title;} // return the title of the song.
        string get_artist_name(){ return artist_name;}  // return the artist name of the song.
        string get_duration_time(){ return duration_time;} // return the duration time of the song.
        
        void display_Song() { // print out all the data about the song such as title, artist, and duration time.
            cout << title << " by " << artist_name << "\t" << duration_time << endl;
        }
        
        void conversion_to_capital() { // convert the Title and the Artist's name into full capital letter
            std::locale loc;
            string temp = "";
            string tempo;
            for(std::string::size_type i = 0; i < title.length(); ++i) {
                   tempo = std::toupper(title[i], loc);
                   temp += tempo;
            }
            title = temp;
            
            temp = "";
            for(std::string::size_type i = 0; i < artist_name.length(); ++i){
                   tempo = std::toupper(artist_name[i], loc);
                   temp += tempo;
            }
            artist_name = temp;
        }
        
    private:
        string title; // attribute of the song for the title.
        string artist_name; // attribute of the song for the artist's name.
        string duration_time;  // attribute of the song for the duration time.
};


class Play_list { // class similar to the dynamic Bags
    public:
        typedef std::size_t size_type;
        // n*(n+1)/2
        static const size_type DEFAULT_CAPACITY = ((8*9)/2); // default size of the play list is 36 songs.
        
        // Constructor, copy constructor, and destructor
        Play_list(string my_play_list_title = "???", size_type initial_capacity = DEFAULT_CAPACITY);
        Play_list(const Play_list& source);
        ~Play_list();
        
        // Modification member functions
        void reserve(size_type new_capacity);
        void insert_song(const Song& entry);
        void operator +=(const Play_list& addend);
        void operator =(const Play_list& source);
        
        void show_play_list() { // show full play list so far.
            for(int i = 0; i < used; ++i){
                data[i].display_Song();
            }
        }
        
        string return_title_play_list(){ return play_list_title;} // return the play list title.
        
        bool check_repeated_song(Song entry){ // to avoid duplicate songs, check if the title and artist are the same
            bool answer1 = false, answer2 = false;
            int pos = 0;
            for (int i = 0; i < used; ++i){
                if (entry.get_title().compare(data[i].get_title()) == 0){
                    answer1 = true; // the title is the same
                    pos = i;  // save the position so we can check if it's the same artist
                }
            }
            
            if (entry.get_artist_name().compare(data[pos].get_artist_name()) == 0){
                answer2 = true; // the artist is the same
            }
            return (answer1 && answer2); // if the artist and the title is the same we have duplicates
        }
        
        // CONSTANT MEMBER FUNCTIONS
        size_type size_of_the_play_list() const { return used;} // return the amount used so far in the playlist.
        
        bool comparator(string a, string b) {  // compare if one string is bigger than the other
            // helper function for sorting the play list
            return a > b;
        }
        
        void sort_play_list() { // sort the play list in alphabetical order by using the naive method bubblesort
            Song temp;
            for (int i = 0; i < used; ++i){
                for(int j = 0; j < used -i-1; ++j){
                    if(comparator(data[j].get_title(), data[j+1].get_title())){
                        temp = data[j+1];
                        data[j+1] = data[j];
                        data[j] = temp;
                    }
                }
            }
        }
        
        void shuffle() { // shuffle the play list to randomize song selection
            Song temp;
            int randomindex = 0;
            for( int i = 0; i < used; ++i){
                randomindex = rand()%used;
                temp = data[i];
                data[i] = data[randomindex];
                data[randomindex] = temp;
                
            }
            
        }
        
        void erase_song(string target_name_song, string target_name_artist){ // erase songs at the wish of user
            std::locale loc;
            string temp = "";
            string tempo;
            
            for(std::string::size_type i = 0; i < target_name_song.length(); ++i) { // we will access the song name first, asking the user to input
                   tempo = std::toupper(target_name_song[i], loc);
                   temp += tempo;
            }
            target_name_song = temp;
            temp = "";
            for(std::string::size_type i = 0; i < target_name_artist.length(); ++i){ // later we will ask acecss the artist name
                   tempo = std::toupper( target_name_artist[i], loc);
                   temp += tempo;
            }
            target_name_artist = temp;
            size_type index = 0;
            
            while((index < used) && (data[index].get_title() != target_name_song))
                    ++index;
            
            if( data[index].get_artist_name() != target_name_artist) return;
            
            
            if(index == used) return;
            
            
            for(int i = index; i < used; ++i) {
                  data[i] = data[i+1]; // just override the song.
            }
            --used;
        }
        
    private:
        size_type used; // attribute of the amount used so far from the capacity of the array.
        size_type capacity; // attribute to store the capacity of the dynamic array.
        Song* data;  // attribute to store a dynamic array of Song.
        string play_list_title; // attribute to store the title of the play list.
};

Play_list::Play_list( string my_play_list_title, size_type initial_capacity){
    play_list_title = my_play_list_title;
    data = new Song[initial_capacity];
    capacity = initial_capacity;
    used = 0;
}

Play_list::Play_list(const Play_list& source){
    data = new Song[source.capacity];
    capacity = source.capacity;
    used = source.used;
    play_list_title = source.play_list_title;
    copy(source.data, source.data + used, data);
}

Play_list::~Play_list(){
    delete[] data; // return the data to the heap.
}

void Play_list::reserve(size_type new_capacity){ // get more capacity memory
    Song* larger_array;
    if(new_capacity == capacity)
            return;
    
    if(new_capacity < used){
        new_capacity = used;
    }
    
    larger_array = new Song[new_capacity];
    copy(data, data + used, larger_array);
    delete[] data;
    data = larger_array;
    capacity = new_capacity;
}

void Play_list::insert_song(const Song& entry){
    if(!(check_repeated_song(entry))){
        if(used == capacity)
                reserve(used+1);
        data[used] = entry;
        ++used;
    }
}

void Play_list::operator +=(const Play_list& addend){ // allow us to append a play list with another playlist
    if (used + addend.used > capacity)
            reserve(used + addend.used);
        
    copy(addend.data, addend.data + addend.used, data + used);
    used += addend.used;

}

void Play_list::operator =(const Play_list& source){ // assignment operator of the playlist
    Song *new_data;

    // Check for possible self-assignment:
    if (this == &source)
            return;

    // If needed, allocate an array with a different size:
    if (capacity != source.capacity){
        new_data = new Song[source.capacity];
        delete [] data;
        data = new_data;
        capacity = source.capacity;
    }

    // Copy the data from the source array:
    used = source.used;
    copy(source.data, source.data + used, data);
}

int main(){
    string my_play_list_title;
    cout << "Enter the title of your play list: ";
    getline(cin, my_play_list_title);
    
    Play_list my_play_list(my_play_list_title);
    Song my_song;

    string choice = "1729";
    string my_song_title, my_song_duration, my_song_artist = ""; // for case 1 (add song)
    string temporary_song, temporary_artist = ""; // for case 5 (delete song)
    
    sleep(1);
    system("CLS");
    
    // Let's build an option menu by using a while loop and a switch option
    while(choice != "6"){
        //system("CLS");
        cout << "             *** Menu ***\n";
        cout << "----------------------------------------\n";
        cout << " 1. Add a new song to the play list.\n";
        cout << "----------------------------------------\n";
        cout << " 2. Show me the current play list.\n";
        cout << "----------------------------------------\n";
        cout << " 3. Sort play list in alphabetic order.\n";
        cout << "----------------------------------------\n";
        cout << " 4. Shuffle the play list.\n";
        cout << "----------------------------------------\n";
        cout << " 5. Delete Song.\n";
        cout << "----------------------------------------\n";
        cout << " 6. Exit\n";
        cin >> choice; // give user the choice to pick from 1-6 menu options
        
        // add a song
        if (choice == "1") {
            sleep(1);
            system("CLS");
            cin.clear();
            fflush(stdin);
            cout << "We are adding a new song to your play list\n";
            cout << "Please, enter the title of the song:\n";
            while (my_song_title == ""){
                getline(cin, my_song_title); // let user input the song name they want to add
            }
            cout << "Enter the song's artist:\n";
            while (my_song_artist == ""){
                getline(cin, my_song_artist); // let user innput the artist of the song
            }
            cout << "Enter the duration of the song:\n";
            while (my_song_duration == ""){
                getline(cin, my_song_duration); // let user input the duration of the particular song
            }
            my_song = Song(my_song_title, my_song_artist, my_song_duration);
            my_song.conversion_to_capital(); // convert all inputs to capital case
            my_play_list.insert_song(my_song);
            sleep(1);
            // Reset values
            my_song_title = "";
            my_song_duration = "";
            my_song_artist = "";
        }
        // Show current playlist
        else if (choice == "2"){
            sleep(1);
            system("CLS");
            cout << "Current Play list:\n";
            
            cout << " *** " << my_play_list.return_title_play_list() << " *** \n"; // highlighting selected playlist for user
            
            my_play_list.show_play_list(); // display playlist contents; show song name and artist name, followed by duration
            sleep(2);
            
            cout << endl << endl;
        }
        // Sort playlist alphabetically
        else if (choice == "3"){
            system("CLS");
            my_play_list.sort_play_list();
            my_play_list.show_play_list();
            sleep(2);
            cout << endl << endl;
        }
        // Shuffle songs in the playlist
        else if (choice == "4"){
            system("CLS");
            my_play_list.shuffle();
            my_play_list.show_play_list();
            sleep(2);
            cout << endl << endl;
        }
        // Delete song from playlist
        else if (choice == "5"){
            sleep(1);
            system("CLS");
            cin.clear();
            fflush(stdin);

            cout << "Which song?\n";
            while (temporary_song == ""){
                getline(cin, temporary_song ); // ask for song to be erased
            }
            cout << "Which artist?\n";
            while (temporary_artist == ""){
                getline(cin, temporary_artist); // ask for artist of song to be erased
            }
            my_play_list.erase_song( temporary_song, temporary_artist);
            cout << "Erasing....\n";
            cout << "Done, this is the new play list:\n"; // display new playlist with deleted item for user
            my_play_list.show_play_list();
            // reset values
            temporary_song = "";
            temporary_artist = "";

        }
        // Exit menu
        else if (choice == "6"){
            sleep(1);
            cout << "Exiting.....\n";
        }
        // Error message
        else {
            sleep(1);
            system("CLS");
            cout << "Error, input is not an available choice.\nOptions: 1, 2, 3, 4, 5, or 6.\n"; // only integer inputs available
        }
    }
    return 0;
}
