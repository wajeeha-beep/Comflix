#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include "../models/Movie.h"

using namespace std;

class MovieRepository {
private:
    map<int, Movie> moviesById;

    // Helper: trim whitespace from a string
    string trim(const string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end   = s.find_last_not_of(" \t\r\n");
        return (start == string::npos) ? "" : s.substr(start, end - start + 1);
    }

public:

    // Add a movie to the repository
    void addMovie(const Movie& m) {
        moviesById[m.id] = m;
    }

    // Get a movie by ID (returns nullptr if not found)
    Movie* getMovie(int id) {
        auto it = moviesById.find(id);
        return (it != moviesById.end()) ? &it->second : nullptr;
    }

    // Get all movies as a vector
    vector<Movie> getAllMovies() {
        vector<Movie> result;
        result.reserve(moviesById.size());
        for (auto& pair : moviesById)
            result.push_back(pair.second);
        return result;
    }

    // Check if a movie exists
    bool exists(int id) {
        return moviesById.find(id) != moviesById.end();
    }

    // Display all movies
    void displayAll() {
        for (auto& pair : moviesById) {
            pair.second.display();
            cout << "------------------\n";
        }
    }

    // -------------------------------------------------------
    // CSV SAVE: writes movies to CSV file
    // Format: id,title,genre,rating,year
    // -------------------------------------------------------
    void saveToFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "[Error] Could not open " << filename << " for writing.\n";
            return;
        }

        // Header row
        file << "id,title,genre,rating,year\n";

        for (auto& pair : moviesById) {
            const Movie& m = pair.second;

            // Escape commas in title/genre by wrapping in quotes if needed
            string safeTitle = m.title;
            string safeGenre = m.genre;
            if (safeTitle.find(',') != string::npos)
                safeTitle = "\"" + safeTitle + "\"";
            if (safeGenre.find(',') != string::npos)
                safeGenre = "\"" + safeGenre + "\"";

            file << m.id << ","
                 << safeTitle << ","
                 << safeGenre << ","
                 << m.rating << ","
                 << m.releaseYear << "\n";
        }
    }

    // -------------------------------------------------------
    // CSV LOAD: reads movies from CSV file
    // Supports quoted fields (e.g. titles with commas)
    // -------------------------------------------------------
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) return;

        string line;
        bool firstLine = true;

        while (getline(file, line)) {
            // Skip header row
            if (firstLine) {
                firstLine = false;
                continue;
            }

            line = trim(line);
            if (line.empty()) continue;

            // Parse CSV row manually (handles quoted fields)
            vector<string> fields;
            string field;
            bool inQuotes = false;

            for (size_t i = 0; i < line.size(); ++i) {
                char c = line[i];
                if (c == '"') {
                    inQuotes = !inQuotes;
                } else if (c == ',' && !inQuotes) {
                    fields.push_back(trim(field));
                    field.clear();
                } else {
                    field += c;
                }
            }
            fields.push_back(trim(field)); // last field

            if (fields.size() < 5) continue; // skip malformed rows

            try {
                int   id     = stoi(fields[0]);
                string title = fields[1];
                string genre = fields[2];
                float rating = stof(fields[3]);
                int   year   = stoi(fields[4]);

                if (!exists(id)) {
                    addMovie(Movie(id, title, genre, rating, year));
                }
            } catch (...) {
                continue; // skip malformed row
            }
        }
    }
};
