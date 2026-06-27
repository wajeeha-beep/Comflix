#pragma once

#include <string>
#include "../dsa/DoublyLinkedList.h"
#include "../dsa/Queue.h"

using namespace std;

class User {
public:
    int id;
    string name;

    DoublyLinkedList history;
    Queue watchLater;

    User() = default;

    User(int id, string name) {
        this->id = id;
        this->name = name;
    }
};