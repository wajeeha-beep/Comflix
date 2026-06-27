#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include "Node.h"

using namespace std;

class Queue {
private:
    Node* front;
    Node* rear;
    unordered_set<int> movieIds; // O(1) duplicate check

public:
    Queue() {
        front = nullptr;
        rear  = nullptr;
    }

    // Enqueue — O(1) duplicate check
    bool enqueue(Movie m) {
        if (movieIds.count(m.id)) {
            cout << "\"" << m.title << "\" is already in Watch Later!\n";
            return false;
        }
        movieIds.insert(m.id);
        Node* newNode = new Node(m);
        if (!rear) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            newNode->prev = rear;
            rear = newNode;
        }
        return true;
    }

    // O(1) check
    bool contains(int id) {
        return movieIds.count(id) > 0;
    }

    void dequeue() {
        if (!front) { cout << "Queue is empty!\n"; return; }
        movieIds.erase(front->data.id);
        Node* temp = front;
        front = front->next;
        if (front) front->prev = nullptr;
        else rear = nullptr;
        delete temp;
    }

    void display() {
        if (!front) { cout << "Watch Later is empty!\n"; return; }
        cout << "WATCH LATER QUEUE:\n\n";
        Node* temp = front;
        while (temp) {
            temp->data.display();
            cout << "------------------\n";
            temp = temp->next;
        }
    }

    vector<int> toVector() {
        vector<int> ids;
        Node* temp = front;
        while (temp) {
            ids.push_back(temp->data.id);
            temp = temp->next;
        }
        return ids;
    }

    bool isEmpty() { return front == nullptr; }

    ~Queue() { while (front) dequeue(); }
};
