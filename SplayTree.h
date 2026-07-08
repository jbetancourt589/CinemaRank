#pragma once
#include <vector>
#include <string>
#include <chrono>
#include "Movie.h"

using namespace std;
using namespace chrono;

class SplayTree
{
public:

    struct Node
    {
        Movie movie;
        Node* left;
        Node* right;

        Node(const Movie& m) : movie(m)
        {
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root = nullptr;
    long long buildTime = 0;
    mutable long long queryTime = 0;
    string mode;

    SplayTree(string m = "rating")
    {
        mode = m;
        root = nullptr;
    }

    ~SplayTree()
    {
        deleteTree(root);
    }

    float getKey(const Movie& m)
    {
        if (mode == "rating")
            return m.vote_average;
        return (float) m.vote_count;
    }

    Node* rotateRight(Node* node)
    {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        return newRoot;
    }

    Node* rotateLeft(Node* node)
    {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        return newRoot;
    }

    Node* splay(Node* node, float k)
    {
        if (node == nullptr || getKey(node->movie) == k)
            return node;

        if (k < getKey(node->movie))
        {
            if (node->left == nullptr) return node;

            if (k < getKey(node->left->movie))
            {
                node->left->left = splay(node->left->left, k);
                node = rotateRight(node);
            }
            else if (k > getKey(node->left->movie))
            {
                node->left->right = splay(node->left->right, k);
                if (node->left->right != nullptr)
                    node->left = rotateLeft(node->left);
            }

            if (node->left == nullptr) return node;
            return rotateRight(node);
        }
        else
        {
            if (node->right == nullptr) return node;

            if (k < getKey(node->right->movie))
            {
                node->right->left = splay(node->right->left, k);
                if (node->right->left != nullptr)
                    node->right = rotateRight(node->right);
            }
            else if (k > getKey(node->right->movie))
            {
                node->right->right = splay(node->right->right, k);
                node = rotateLeft(node);
            }

            if (node->right == nullptr) return node;
            return rotateLeft(node);
        }
    }

    void insert(const Movie& movie)
    {
        if (root == nullptr)
        {
            root = new Node(movie);
            return;
        }

        float k = getKey(movie);
        root = splay(root, k);

        Node* newNode = new Node(movie);

        if (k < getKey(root->movie))
        {
            newNode->right = root;
            newNode->left = root->left;
            root->left = nullptr;
            root = newNode;
        }
        else
        {
            newNode->left = root;
            newNode->right = root->right;
            root->right = nullptr;
            root = newNode;
        }
    }

    void buildTree(const vector<Movie>& movies)
    {
        auto start = high_resolution_clock::now();

        for (auto& m : movies)
            insert(m);

        auto end = high_resolution_clock::now();
        buildTime = duration_cast<milliseconds>(end - start).count();
    }

    void getTopNHelper(Node* node, vector<Movie>& result, int n)
    {
        if (node == nullptr || (int)result.size() >= n)
            return;

        getTopNHelper(node->right, result, n);

        if ((int)result.size() < n)
            result.push_back(node->movie);

        getTopNHelper(node->left, result, n);
    }

    vector<Movie> getTopN(int n)
    {
        auto start = high_resolution_clock::now();

        vector<Movie> result;
        getTopNHelper(root, result, n);

        auto end = high_resolution_clock::now();
        queryTime = duration_cast<milliseconds>(end - start).count();

        return result;
    }

    void deleteTree(Node* node)
    {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
};