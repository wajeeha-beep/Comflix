#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include "Node.h"

using namespace std;

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;
    unordered_set<int> movieIds; // O(1) duplicate check

public:
    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
    }

    // Move existing node to front — O(n) traverse, O(1) move
    void moveToFront(int id) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == id) {
                if (temp == head) return; // already at front
                // detach node
                if (temp->prev) temp->prev->next = temp->next;
                if (temp->next) temp->next->prev = temp->prev;
                if (temp == tail) tail = temp->prev;
                // attach at head
                temp->prev = nullptr;
                temp->next = head;
                head->prev = temp;
                head = temp;
                return;
            }
            temp = temp->next;
        }
    }

    // Add movie — duplicate check O(1), move to front if exists
    bool add(Movie m) {
        if (movieIds.count(m.id)) {
            moveToFront(m.id);
            return false;
        }
        movieIds.insert(m.id);
        Node* newNode = new Node(m);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        return true;9
    }

    // O(1) check
    bool contains(int id) {
        return movieIds.count(id) > 0;
    }

    void display() {
        if (!head) { cout << "History is empty!\n"; return; }
        Node* temp = head;
        while (temp) {
            temp->data.display();
            cout << "------------------\n";
            temp = temp->next;
        }
    }

    vector<int> toVector() {
        vector<int> ids;
        Node* temp = head;
        while (temp) {
            ids.push_back(temp->data.id);
            temp = temp->next;
        }
        return ids;
    }

    ~DoublyLinkedList() {
        Node* temp = head;
        while (temp) {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
    }

    Node* getHead() { return head; }
};
