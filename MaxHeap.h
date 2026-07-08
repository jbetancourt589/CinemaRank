#pragma once
#include <vector>
#include <string>
#include <chrono>
#include "Movie.h"

using namespace std;
using namespace chrono;

class MaxHeap
{
public:

    vector<Movie> heap;
    long long buildTime = 0;
    mutable long long queryTime = 0;
    string mode;

    MaxHeap(string m = "rating")
    {
        mode = m;
    }

    float getKey(const Movie& m)
    {
        if (mode == "rating")
            return m.vote_average;
        return (float) m.vote_count;
    }

    int parent(int i) { return (i - 1) / 2; }
    int leftChild(int i) { return 2 * i + 1; }
    int rightChild(int i) { return 2 * i + 2; }

    void heapifyUp(int i)
    {
        while (i > 0 && getKey(heap[i]) > getKey(heap[parent(i)]))
        {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

    void heapifyDown(int i)
    {
        int largest = i;
        int left = leftChild(i);
        int right = rightChild(i);

        if (left < (int)heap.size() && getKey(heap[left]) > getKey(heap[largest]))
            largest = left;

        if (right < (int)heap.size() && getKey(heap[right]) > getKey(heap[largest]))
            largest = right;

        if (largest != i)
        {
            swap(heap[i], heap[largest]);
            heapifyDown(largest);
        }
    }

    void insert(const Movie& movie)
    {
        heap.push_back(movie);
        heapifyUp(heap.size() - 1);
    }

    void buildHeap(const vector<Movie>& movies)
    {
        auto start = high_resolution_clock::now();

        for (auto& m : movies)
            insert(m);

        auto end = high_resolution_clock::now();
        buildTime = duration_cast<milliseconds>(end - start).count();
    }

    vector<Movie> getTopN(int n)
    {
        auto start = high_resolution_clock::now();

        MaxHeap temp = *this;
        vector<Movie> result;

        for (int i = 0; i < n && temp.heap.size() > 0; i++)
        {
            result.push_back(temp.heap[0]);
            temp.heap[0] = temp.heap[temp.heap.size() - 1];
            temp.heap.pop_back();
            temp.heapifyDown(0);
        }

        auto end = high_resolution_clock::now();
        queryTime = duration_cast<milliseconds>(end - start).count();

        return result;
    }
};