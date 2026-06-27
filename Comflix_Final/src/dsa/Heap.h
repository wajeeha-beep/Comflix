#pragma once
#include <iostream>
#include <vector>
#include "../models/Movie.h"
using namespace std;

class Heap {
private:
    vector<Movie> heap;

    void heapifyUp(int index) {
        if (index == 0) return;

        int parent = (index - 1) / 2;

        if (heap[index].rating > heap[parent].rating) {
            swap(heap[index], heap[parent]);
            heapifyUp(parent);
        }
    }

    void heapifyDown(int index) {
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < heap.size() && heap[left].rating > heap[largest].rating)
            largest = left;

        if (right < heap.size() && heap[right].rating > heap[largest].rating)
            largest = right;

        if (largest != index) {
            swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }

public:
    void insert(Movie m) {
        heap.push_back(m);
        heapifyUp(heap.size() - 1);
    }

    void showTrending() {
        cout << "TRENDING MOVIES:\n";

        if (heap.empty()) return;

        // make a copy so original heap is not destroyed
        vector<Movie> temp = heap;

        int count = 0;

        while (!temp.empty() && count < 10) {

            // get max manually
            int largest = 0;

            for (int i = 1; i < temp.size(); i++) {
                if (temp[i].rating > temp[largest].rating) {
                    largest = i;
                }
            }

            temp[largest].display();
            cout << "------------------\n";

            temp.erase(temp.begin() + largest);

            count++;
        }
    }

    Movie extractTop() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }

        Movie top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();

        heapifyDown(0);

        return top;
    }

    void clear() {
        heap.clear();
    }
};