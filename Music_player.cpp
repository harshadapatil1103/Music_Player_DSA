#include <iostream>
#include <unordered_map>
#include <string>
#include <random>


using namespace std;

// Song class to represent each song
class Song {
private:
    string id;
    string title;
    string artist;
    string genre;

public:
    Song(string _id, string _title, string _artist, string _genre)
        : id(_id), title(_title), artist(_artist), genre(_genre) {}

    string getId() const { return id; }
    string getTitle() const { return title; }
    string getArtist() const { return artist; }
    string getGenre() const { return genre; }
};

// User Class
class User {
private:
    string username;
    unordered_map<string, int> listeningHistory; // Maps song ID to number of times listened

public:
    User(string _username) : username(_username) {}

    string getUsername() const { return username; } // Provide access to the username

    void addToHistory(const string& songId) {
        if (listeningHistory.find(songId) != listeningHistory.end()) {
            listeningHistory[songId]++;
        } else {
            listeningHistory[songId] = 1;
        }
    }

    void displayListeningHistory() const {
        cout << "Listening history for user " << username << ":" << endl;
        for (const auto& entry : listeningHistory) {
            cout << "Song ID: " << entry.first << ", Listened: " << entry.second << " times" << endl;
        }
    }
};

// MusicPlayer class to manage the music player
class MusicPlayer {
private:
    struct Node {
        Song data;
        Node* prev;
        Node* next;

        Node(Song _data) : data(_data), prev(nullptr), next(nullptr) {}
    };

    unordered_map<string, Node*> songMap;
    Node* head;
    Node* tail;
    Node* currentSongNode;
    vector<User> users;
    User* currentUser;
    bool isPlaying;

public:
    MusicPlayer() : head(nullptr), tail(nullptr), currentSongNode(nullptr), isPlaying(false) {}

    void addUser(const string& username) {
        users.push_back(User(username));
        currentUser = &users.back(); // Set the current user to the last added user
    }

    void addSong(const Song& song) {
        Node* newNode = new Node(song);
        songMap[song.getId()] = newNode;
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void playSong(const string& songId) {
        if (songMap.find(songId) == songMap.end()) {
            cout << "Song not found." << endl;
            return;
        }
        currentSongNode = songMap[songId];

        currentUser->addToHistory(songId);

        cout << "Playing: " << songMap[songId]->data.getTitle() << endl;
        isPlaying = true;
        if (isPlaying) {
            cout << "Enter 'p' to pause ";
            char ch;
            cin >> ch;
            if (ch == 'p') {
                isPlaying = false;
                cout << "Song Paused..." << endl;
            }
        }
        if (!isPlaying) {
            cout << "Enter 'r' to resume ";
            char ch;
            cin >> ch;
            if (ch == 'r') {
                isPlaying = true;
                cout << "Song Resumed..." << endl;
            }
        }
    }

    void playRandom() {
        if (songMap.empty()) {
            cout << "No songs in the playlist." << endl;
            return;
        }

        // Get a random iterator
        auto it = next(songMap.begin(), rand() % songMap.size());
        currentSongNode = it->second;
        currentUser->addToHistory(currentSongNode->data.getId());
        cout << "Playing random song: " << currentSongNode->data.getTitle() << endl;
        isPlaying = true;
    }

    void skipBackward() {
        if (!currentSongNode) {
            cout << "No song is selected." << endl;
            return;
        }

        if (currentSongNode->prev) {
            currentSongNode = currentSongNode->prev;
            currentUser->addToHistory(currentSongNode->data.getId());
            cout << "Playing previous song: " << currentSongNode->data.getTitle() << endl;
            isPlaying = true;
        } else {
            cout << "Already at the beginning of the playlist." << endl;
        }
    }

    void skipForward() {
        if (!currentSongNode) {
            cout << "No song is selected." << endl;
            return;
        }

        if (currentSongNode->next) {
            currentSongNode = currentSongNode->next;
            currentUser->addToHistory(currentSongNode->data.getId());
            cout << "Playing next song: " << currentSongNode->data.getTitle() << endl;
            isPlaying = true;
        } else {
            cout << "Already at the end of the playlist." << endl;
        }
    }

    void adjustVolume(int volumeLevel) {
        cout << "Volume adjusted to level: " << volumeLevel << endl;
    }

    void displayPlaylist() {
        cout << "\n=== Playlist ===" << endl;
        for (auto it = songMap.begin(); it != songMap.end(); ++it) {
            cout << it->first << " : " << it->second->data.getTitle() << endl;
        }
        cout << "================" << endl;
    }

    void displayListeningHistory() const {
        if (currentUser) {
            currentUser->displayListeningHistory();
        } else {
            cout << "No user selected." << endl;
        }
    }

    void setUser(const string& username) {
        for (auto& user : users) {
            if (user.getUsername() == username) {
                currentUser = &user;
                return;
            }
        }
        cout << "User not found." << endl;
    }
};

int main() {
    MusicPlayer player;

    string username;
    string songId;
    string title, artist, genre, id;

    while (true) {
        int choice;
        cout << "+-----------------------------------+" << endl;
        cout << "|        Music Player Menu          |" << endl;
        cout << "+-----------------------------------+" << endl;
        cout << "| 1. Add User                       |" << endl;
        cout << "| 2. Select User                    |" << endl;
        cout << "| 3. Add Song                       |" << endl;
        cout << "| 4. Play Song                      |" << endl;
        cout << "| 5. Play Random Song               |" << endl;
        cout << "| 6. Display Listening History      |" << endl;
        cout << "| 7. Play Previous Song             |" << endl;
        cout << "| 8. Play Next Song                 |" << endl;
        cout << "| 9. Adjust Volume                  |" << endl;
        cout << "| 10. Display Playlist              |" << endl;
        cout << "| 11. Exit                          |" << endl;
        cout << "+-----------------------------------+" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter username: ";
            cin >> username;
            player.addUser(username);
            break;
        case 2:
            cout << "Enter username: ";
            cin >> username;
            player.setUser(username);
            break;
        case 3:
            cout << "Enter song details:\n";
            cout << "ID: ";
            cin >> id;
            cout << "Title: ";
            cin >> title;
            cout << "Artist: ";
            cin >> artist;
            cout << "Genre: ";
            cin >> genre;
            player.addSong(Song(id, title, artist, genre));
            break;
        case 4:
            cout << "Enter song ID: ";
            cin >> songId;
            player.playSong(songId);
            break;
        case 5:
            player.playRandom();
            break;
        case 6:
            player.displayListeningHistory();
            break;
        case 7:
            player.skipBackward();
            break;
        case 8:
            player.skipForward();
            break;
        case 9:
            int volume;
            cout << "Enter Volume to set: ";
            cin >> volume;
            player.adjustVolume(volume);
            break;
        case 10:
            player.displayPlaylist();
            break;
        case 11:
            cout << "Exiting program.\n";
            return 0;
        default:
            cout << "Invalid choice. Please enter a valid option.\n";
        }

    }

    return 0;
}