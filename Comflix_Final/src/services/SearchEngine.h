#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "../models/Movie.h"
#include "../dsa/Trie.h"

using namespace std;

class SearchEngine {
private:
    unordered_map<string, Movie> movieMap;
    Trie trie;

    string toLower(string s) {
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

public:

    void addMovie(Movie m) {
        string key = toLower(m.title);
        movieMap[key] = m;
        trie.insert(m.title);
    }

    Movie* searchByTitle(const string& title) {
        string query = toLower(title);
        if (movieMap.find(query) != movieMap.end())
            return &movieMap[query];
        for (auto& pair : movieMap)
            if (pair.first.find(query) != string::npos)
                return &pair.second;
        return nullptr;
    }

    // Search all matches — multiple results
    void displayMovie(const string& title) {
        string query = toLower(title);
        vector<Movie*> results;
        for (auto& pair : movieMap)
            if (pair.first.find(query) != string::npos)
                results.push_back(&pair.second);

        if (results.empty()) {
            cout << "Movie not found!\n";
        } else {
            cout << results.size() << " movie(s) found:\n";
            for (auto* m : results) {
                m->display();
                cout << "------------------\n";
            }
        }
    }

    void showSuggestions(const string& query) {
        cout << "Suggestions:\n";
        string lowerQuery = toLower(query);
        vector<string> shown;

        auto trieResults = trie.autoComplete(query);
        for (auto& title : trieResults) {
            cout << "- " << title << endl;
            shown.push_back(toLower(title));
        }

        for (auto& pair : movieMap) {
            if (pair.first.find(lowerQuery) != string::npos) {
                bool alreadyShown = false;
                for (auto& s : shown)
                    if (s == pair.first) { alreadyShown = true; break; }
                if (!alreadyShown)
                    cout << "- " << pair.second.title << endl;
            }
        }
    }

    // Genre search
    void searchByGenre(const string& genre) {
        string g = toLower(genre);
        bool found = false;
        for (auto& pair : movieMap) {
            if (toLower(pair.second.genre) == g) {
                pair.second.display();
                cout << "------------------\n";
                found = true;
            }
        }
        if (!found) cout << "No movies found in this genre!\n";
    }

    // Unique genre list
    vector<string> getGenres() {
        vector<string> genres;
        for (auto& pair : movieMap) {
            string g = pair.second.genre;
            // normalize: first letter capital, rest small
            if (!g.empty()) {
                g[0] = toupper(g[0]);
                for (int i = 1; i < (int)g.size(); i++)
                    g[i] = tolower(g[i]);
            }
            bool found = false;
            for (auto& x : genres)
                if (x == g) { found = true; break; }
            if (!found) genres.push_back(g);
        }
        return genres;
    }

    // Rating filter
    void filterByRating(float minRating) {
        bool found = false;
        for (auto& pair : movieMap) {
            if (pair.second.rating >= minRating) {
                pair.second.display();
                cout << "------------------\n";
                found = true;
            }
        }
        if (!found) cout << "No movies found above this rating!\n";
    }

    void clear() {
        movieMap.clear();
    }
};
