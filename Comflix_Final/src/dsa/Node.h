#pragma once
#include "../models/Movie.h"

struct Node {
    Movie data;
    Node* next;
    Node* prev;

    Node(Movie m) {
        data = m;
        next = nullptr;
        prev = nullptr;
    }
};