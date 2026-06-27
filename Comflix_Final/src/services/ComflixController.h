#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../models/Movie.h"
#include "../models/User.h"
#include "../services/MovieRepository.h"
#include "../services/SearchEngine.h"
#include "../dsa/Graph.h"
#include "../dsa/Heap.h"

using namespace std;

class ComflixController {
private:
    MovieRepository repo;
    SearchEngine    searchEngine;
    Graph*          recommendationGraph;
    Heap            trendingHeap;

    unordered_map<int, User> users;
    int currentUserId = -1;

    vector<string> splitCSV(const string& line, char delim = ',') {
        vector<string> fields;
        string field;
        bool inQuotes = false;
        for (size_t i = 0; i < line.size(); ++i) {
            char c = line[i];
            if (c == '"') inQuotes = !inQuotes;
            else if (c == delim && !inQuotes) { fields.push_back(trim(field)); field.clear(); }
            else field += c;
        }
        fields.push_back(trim(field));
        return fields;
    }

    string trim(const string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end   = s.find_last_not_of(" \t\r\n");
        return (start == string::npos) ? "" : s.substr(start, end - start + 1);
    }

    vector<int> splitPipeIds(const string& s) {
        vector<int> ids;
        if (s.empty()) return ids;
        stringstream ss(s);
        string token;
        while (getline(ss, token, '|')) {
            token = trim(token);
            if (!token.empty()) {
                try { ids.push_back(stoi(token)); } catch (...) {}
            }
        }
        return ids;
    }

public:

    ComflixController(int size) {
        recommendationGraph = new Graph(size);
    }

    // ========================
    // USER SYSTEM
    // ========================
    void addUser(User u) { users[u.id] = u; }

    void setCurrentUser(int id) {
        if (users.find(id) != users.end()) currentUserId = id;
        else cout << "User not found!\n";
    }

    void showCurrentUser() {
        if (currentUserId == -1) { cout << "No user selected!\n"; return; }
        cout << "Current User: " << users[currentUserId].name << "\n";
    }

    // ========================
    // MOVIE SYSTEM
    // ========================
    void addMovie(Movie m) {
        repo.addMovie(m);
        searchEngine.addMovie(m);
        trendingHeap.insert(m);
    }

    bool movieExists(int id) { return repo.exists(id); }

    void showMovieById(int id) {
        Movie* m = repo.getMovie(id);
        if (m) m->display();
    }

    void showAllMovies() { repo.displayAll(); }

    void showGenres() {
        vector<string> genres = searchEngine.getGenres();
        cout << "\nAvailable Genres:\n";
        for (auto& g : genres) cout << "- " << g << "\n";
    }

    void filterByRating(float minRating) {
        searchEngine.filterByRating(minRating);
    }

    void searchByGenre(const string& genre) {
        searchEngine.searchByGenre(genre);
    }

    // ========================
    // WATCH SYSTEM
    // ========================
    void watchMovie(int movieId) {
        Movie* m = repo.getMovie(movieId);
        if (!m || currentUserId == -1) return;

        User& user = users[currentUserId];

        if (user.history.contains(m->id)) {
            cout << "\"" << m->title << "\" moved to top of history!\n";
        } else {
            cout << "Added to history: " << m->title << "\n";
        }
        user.history.add(*m);

        // Build recommendation edges with previously watched
        Node* temp = user.history.getHead();
        while (temp != nullptr) {
            if (temp->data.id != movieId)
                recommendationGraph->addEdge(movieId, temp->data.id);
            temp = temp->next;
        }
    }

    void addToWatchLater(int movieId) {
        Movie* m = repo.getMovie(movieId);
        if (m && currentUserId != -1)
            users[currentUserId].watchLater.enqueue(*m);
    }

    bool isInWatchLater(int id) {
        if (currentUserId == -1) return false;
        return users[currentUserId].watchLater.contains(id);
    }

    void showHistory() {
        if (currentUserId != -1) users[currentUserId].history.display();
    }

    void showWatchLater() {
        if (currentUserId != -1) users[currentUserId].watchLater.display();
    }

    // ========================
    // SEARCH SYSTEM
    // ========================
    void search(const string& title) { searchEngine.displayMovie(title); }
    void suggest(const string& prefix) { searchEngine.showSuggestions(prefix); }
    Movie* getMovieByName(const string& title) { return searchEngine.searchByTitle(title); }

    // ========================
    // TRENDING
    // ========================
    void showTrending() { trendingHeap.showTrending(); }

    // ========================
    // RECOMMENDATIONS (original from zip1)
    // ========================
    void showRecommendations(int movieId) {
        cout << "\nRECOMMENDATIONS:\n";
        vector<int> recs = recommendationGraph->getRecommendations(movieId);

        if (recs.empty()) {
            cout << "No recommendations yet. Watch more movies!\n";
            return;
        }

        for (int id : recs) {
            Movie* m = repo.getMovie(id);
            if (m) {
                m->display();
                cout << "------------------\n";
            }
        }
    }

    // ========================
    // SAVE (CSV) — zip1 paths kept
    // ========================
    void saveData() {
        // repo.saveToFile("D:/Comsats/Comflix_3.0/src/movies.csv");
        repo.saveToFile("movies.csv");

        {
            // ofstream file("D:/Comsats/Comflix_3.0/src/users.csv");
            ofstream file("users.csv");
            if (file.is_open()) {
                file << "user_id,name,history_ids,watchlater_ids\n";
                for (auto& pair : users) {
                    User& u = pair.second;
                    vector<int> hids = u.history.toVector();
                    string histStr;
                    for (size_t i = 0; i < hids.size(); ++i) {
                        if (i) histStr += '|';
                        histStr += to_string(hids[i]);
                    }
                    vector<int> wids = u.watchLater.toVector();
                    string watchStr;
                    for (size_t i = 0; i < wids.size(); ++i) {
                        if (i) watchStr += '|';
                        watchStr += to_string(wids[i]);
                    }
                    file << u.id << "," << u.name << "," << histStr << "," << watchStr << "\n";
                }
            }
        }

        {
            // ofstream file("D:/Comsats/Comflix_3.0/src/graph_edges.csv");
            
            ofstream file("graph_edges.csv");
            if (file.is_open()) {
                file << "from_id,to_id\n";
                vector<vector<int>> adj = recommendationGraph->getAdjList();
                vector<pair<int,int>> written;
                auto alreadyWritten = [&](int a, int b) {
                    for (auto& e : written)
                        if (e.first == a && e.second == b) return true;
                    return false;
                };
                for (int i = 0; i < (int)adj.size(); ++i) {
                    for (int j : adj[i]) {
                        int lo = min(i, j), hi = max(i, j);
                        if (!alreadyWritten(lo, hi)) {
                            file << lo << "," << hi << "\n";
                            written.push_back({lo, hi});
                        }
                    }
                }
            }
        }

        cout << "\n[Saved] movies.csv, users.csv, graph_edges.csv\n";
    }

    // ========================
    // LOAD (CSV) — zip1 paths kept
    // ========================
    void loadData() {
        // repo.loadFromFile("D:/Comsats/Comflix_3.0/src/movies.csv");
        repo.loadFromFile("movies.csv");

        searchEngine.clear();
        trendingHeap.clear();

        for (auto& m : repo.getAllMovies()) {
            searchEngine.addMovie(m);
            trendingHeap.insert(m);
        }

        {
            // ifstream file("D:/Comsats/Comflix_3.0/src/users.csv");
            
            ifstream file("users.csv");
            if (!file.is_open()) {
                cout << "\n[First run] No persisted user data yet.\n";
                return;
            }
            string line;
            bool firstLine = true;
            while (getline(file, line)) {
                if (firstLine) { firstLine = false; continue; }
                line = trim(line);
                if (line.empty()) continue;
                vector<string> fields = splitCSV(line);
                if (fields.size() < 4) continue;
                try {
                    int    userId   = stoi(fields[0]);
                    string name     = fields[1];
                    string histStr  = fields[2];
                    string watchStr = fields[3];
                    if (users.find(userId) == users.end()) continue;
                    User& u = users[userId];
                    u.name  = name;
                    for (int mid : splitPipeIds(histStr)) {
                        Movie* m = repo.getMovie(mid);
                        if (m) u.history.add(*m);
                    }
                    for (int mid : splitPipeIds(watchStr)) {
                        Movie* m = repo.getMovie(mid);
                        if (m) u.watchLater.enqueue(*m);
                    }
                } catch (...) { continue; }
            }
        }

        {
            // ifstream file("D:/Comsats/Comflix_3.0/src/graph_edges.csv");
            ifstream file("graph_edges.csv");
            if (file.is_open()) {
                string line;
                bool firstLine = true;
                while (getline(file, line)) {
                    if (firstLine) { firstLine = false; continue; }
                    line = trim(line);
                    if (line.empty()) continue;
                    vector<string> fields = splitCSV(line);
                    if (fields.size() < 2) continue;
                    try {
                        int from = stoi(fields[0]);
                        int to   = stoi(fields[1]);
                        recommendationGraph->addEdge(from, to);
                    } catch (...) { continue; }
                }
            }
        }

        cout << "\n[Loaded] movies.csv, users.csv, graph_edges.csv\n";
    }

    ~ComflixController() { delete recommendationGraph; }
};
