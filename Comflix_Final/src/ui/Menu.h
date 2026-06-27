#pragma once

#include <iostream>
#include <string>
#include "../services/ComflixController.h"

using namespace std;

class Menu {
private:
    ComflixController& app;

    string pickMovieByName(const string& action) {
        cout << "\nType a few letters for suggestions (or Enter to skip): ";
        cin.ignore();
        string prefix;
        getline(cin, prefix);
        if (!prefix.empty()) app.suggest(prefix);
        cout << action << " (enter title): ";
        string name;
        getline(cin, name);
        return name;
    }

public:
    Menu(ComflixController& controller) : app(controller) {}

    void showMainMenu() {
        int choice;

        while (true) {
            cout << "\n==============================\n";
            cout << "         COMFLIX UI          \n";
            cout << "==============================\n";
            app.showCurrentUser();
            cout << "==============================\n";
            cout << "1.  Add Movie\n";
            cout << "2.  Search Movie\n";
            cout << "3.  Suggestions\n";
            cout << "4.  Watch Movie\n";
            cout << "5.  Add to Watch Later\n";
            cout << "6.  View History\n";
            cout << "7.  View Watch Later\n";
            cout << "8.  Trending\n";
            cout << "9.  Recommendations\n";
            cout << "10. Show All Movies\n";
            cout << "11. Filter by Rating\n";
            cout << "12. Switch User\n";
            cout << "13. Save & Exit\n";
            cout << "Enter choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid input!\n";
                continue;
            }

            // ── ADD MOVIE ──────────────────────────────────────
            if (choice == 1) {
                int id, year;
                float rating;
                string title, genre;

                while (true) {
                    cout << "ID (0 to cancel): ";
                    cin >> id;
                    if (id == 0) { cout << "Cancelled.\n"; break; }

                    if (app.movieExists(id)) {
                        cout << "ID " << id << " already exists!\n";
                        app.showMovieById(id);
                        cout << "Enter a new ID: ";
                    } else {
                        bool cancelled = false;

                        cout << "Title: ";
                        cin.ignore();
                        getline(cin, title);

                        app.showGenres();
                        cout << "Genre: ";
                        getline(cin, genre);

                        while (true) {
                            cout << "Rating (0-10, or -1 to cancel): ";
                            cin >> rating;
                            if (rating == -1) { cout << "Cancelled.\n"; cancelled = true; break; }
                            if (rating >= 0 && rating <= 10) break;
                            cout << "Invalid! Must be 0-10.\n";
                        }

                        if (!cancelled) {
                            cout << "Year: ";
                            cin >> year;
                            app.addMovie(Movie(id, title, genre, rating, year));
                            cout << "Movie added!\n";
                            app.saveData();
                        }
                        break;
                    }
                }
            }

            // ── SEARCH ─────────────────────────────────────────
            else if (choice == 2) {
                cout << "Search by:\n1. Title\n2. Genre\nChoice: ";
                int sc; cin >> sc;
                if (sc == 1) {
                    cout << "Title: "; cin.ignore();
                    string title; getline(cin, title);
                    app.search(title);
                } else if (sc == 2) {
                    app.showGenres();
                    cout << "Genre: "; cin.ignore();
                    string genre; getline(cin, genre);
                    app.searchByGenre(genre);
                }
            }

            // ── SUGGESTIONS ────────────────────────────────────
            else if (choice == 3) {
                cout << "Enter prefix: "; cin.ignore();
                string prefix; getline(cin, prefix);
                app.suggest(prefix);
            }

            // ── WATCH MOVIE ────────────────────────────────────
            else if (choice == 4) {
                string name = pickMovieByName("Watch movie");
                if (name.empty()) { cout << "Cancelled.\n"; continue; }
                Movie* m = app.getMovieByName(name);
                if (!m) { cout << "Movie not found!\n"; }
                else {
                    app.watchMovie(m->id);
                    app.saveData();
                }
            }

            // ── WATCH LATER ────────────────────────────────────
            else if (choice == 5) {
                string name = pickMovieByName("Add to Watch Later");
                if (name.empty()) { cout << "Cancelled.\n"; continue; }
                Movie* m = app.getMovieByName(name);
                if (!m) { cout << "Movie not found!\n"; }
                else {
                    if (app.isInWatchLater(m->id)) {
                        cout << "\"" << m->title << "\" is already in Watch Later!\n";
                    } else {
                        app.addToWatchLater(m->id);
                        cout << "Added to Watch Later: " << m->title << "\n";
                        app.saveData();
                    }
                }
            }

            else if (choice == 6)  { app.showHistory(); }
            else if (choice == 7)  { app.showWatchLater(); }
            else if (choice == 8)  { app.showTrending(); }

            // ── RECOMMENDATIONS ────────────────────────────────
            else if (choice == 9) {
                string name = pickMovieByName("Get recommendations for");
                if (name.empty()) { cout << "Cancelled.\n"; continue; }
                Movie* m = app.getMovieByName(name);
                if (!m) { cout << "Movie not found!\n"; }
                else { app.showRecommendations(m->id); }
            }

            else if (choice == 10) { app.showAllMovies(); }

            // ── FILTER BY RATING ───────────────────────────────
            else if (choice == 11) {
                float r;
                cout << "Show movies rated above: "; cin >> r;
                app.filterByRating(r);
            }

            else if (choice == 12) {
                int userId;
                cout << "User ID (1:Rimsha 2:Arham 3:Wajeeha 4:Mehwish): ";
                cin >> userId;
                app.setCurrentUser(userId);
            }

            else if (choice == 13) {
                app.saveData();
                cout << "Goodbye!\n";
                break;
            }

            else { cout << "Invalid choice!\n"; }
        }
    }
};
