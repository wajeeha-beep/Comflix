#pragma once

#include <iostream>
#include <string>
#include <utility>

class Movie {
public:
    int id{};
    std::string title;
    std::string genre;
    float rating{};
    int releaseYear{};

    Movie() = default;

    Movie(
        int id,
        std::string title,
        std::string genre,
        float rating,
        int releaseYear
    )
        : id(id),
          title(std::move(title)),
          genre(std::move(genre)),
          rating(rating),
          releaseYear(releaseYear) {}

    void display() const {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Title: " << title << std::endl;
        std::cout << "Genre: " << genre << std::endl;
        std::cout << "Rating: " << rating << std::endl;
        std::cout << "Year: " << releaseYear << std::endl;
    }
};