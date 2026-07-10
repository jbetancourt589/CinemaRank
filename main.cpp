#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include "Movie.h"
#include "CSVReader.h"
#include "MaxHeap.h"
#include "SplayTree.h"


using namespace std;

void printMovies(vector<Movie>& movies, string mode)
{
    for (int i = 0; i < (int)movies.size(); i++)
    {
        cout << i + 1 << ". " << movies[i].title;

        if (mode == "rating")
            cout << " (" << fixed << setprecision(1) << movies[i].vote_average << ")";
        else
            cout << " (" << movies[i].vote_count << " votes)";

        cout << "\n";
    }
}

void printComparison(vector<Movie>& heapResults, vector<Movie>& treeResults,
                     MaxHeap& heap, SplayTree& tree, string mode)
{
    int col = 50;
    string label = (mode == "rating") ? "Rating" : "Votes";

    cout << "\n";
    cout << left << setw(col) << ("--- Max Heap (" + label + ") ---")
         << "--- Splay Tree (" + label + ") ---\n";

    int n = max(heapResults.size(), treeResults.size());

    for (int i = 0; i < n; i++)
    {
        string heapLine = "";
        string treeLine = "";

        if (i < (int)heapResults.size())
        {
            heapLine = to_string(i + 1) + ". " + heapResults[i].title;
            if (mode == "rating")
                heapLine += " (" + to_string(heapResults[i].vote_average).substr(0, 3) + ")";
            else
                heapLine += " (" + to_string(heapResults[i].vote_count) + " votes)";
        }

        if (i < (int)treeResults.size())
        {
            treeLine = to_string(i + 1) + ". " + treeResults[i].title;
            if (mode == "rating")
                treeLine += " (" + to_string(treeResults[i].vote_average).substr(0, 3) + ")";
            else
                treeLine += " (" + to_string(treeResults[i].vote_count) + " votes)";
        }

        cout << left << setw(col) << heapLine << treeLine << "\n";
    }

    cout << "\n";
    cout << left << setw(col) << ("Build Time: " + to_string(heap.buildTime) + " ms")
         << "Build Time: " + to_string(tree.buildTime) + " ms\n";
    cout << left << setw(col) << ("Query Time: " + to_string(heap.queryTime) + " ms")
         << "Query Time: " + to_string(tree.queryTime) + " ms\n";
}

int main()
{
    vector<Movie> movies;

    MaxHeap ratingHeap("rating");
    MaxHeap votesHeap("votes");
    SplayTree ratingTree("rating");
    SplayTree votesTree("votes");

    bool loaded = false;
    int choice;

    while (true)
    {
        cout << "\n==============================\n";
        cout << "   CinemaRank - Movie Ranker\n";
        cout << "==============================\n";
        cout << "1. Load Dataset\n";
        cout << "2. Get Top-N by Rating (Max Heap)\n";
        cout << "3. Get Top-N by Rating (Splay Tree)\n";
        cout << "4. Get Top-N by Votes (Max Heap)\n";
        cout << "5. Get Top-N by Votes (Splay Tree)\n";
        cout << "6. Compare Both Structures (Rating)\n";
        cout << "7. Compare Both Structures (Votes)\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string path;
            cout << "Enter path to CSV (or press Enter for default): ";
            cin.ignore();
            getline(cin, path);

            if (path.empty())
                path = "data/TMDB_movie_dataset_v11.csv";

            movies = loadCSV(path);

            if (movies.empty())
            {
                cout << "No movies loaded. Check your file path.\n";
                continue;
            }

            cout << "Building structures...\n";

            ratingHeap = MaxHeap("rating");
            votesHeap  = MaxHeap("votes");
            ratingTree = SplayTree("rating");
            votesTree  = SplayTree("votes");

            ratingHeap.buildHeap(movies);
            cout << "Rating Max Heap built in " << ratingHeap.buildTime << " ms\n";

            votesHeap.buildHeap(movies);
            cout << "Votes Max Heap built in " << votesHeap.buildTime << " ms\n";

            ratingTree.buildTree(movies);
            cout << "Rating Splay Tree built in " << ratingTree.buildTime << " ms\n";

            votesTree.buildTree(movies);
            cout << "Votes Splay Tree built in " << votesTree.buildTime << " ms\n";

            loaded = true;
        }
        else if (choice == 2)
        {
            if (!loaded) { cout << "Load the dataset first!\n"; continue; }
            int n;
            cout << "Enter N: ";
            cin >> n;
            auto results = ratingHeap.getTopN(n);
            cout << "\nTop " << n << " by Rating (Max Heap):\n";
            printMovies(results, "rating");
            cout << "Query Time: " << ratingHeap.queryTime << " ms\n";
        }
        else if (choice == 3)
        {
            if (!loaded) { cout << "Load the dataset first!\n"; continue; }
            int n;
            cout << "Enter N: ";
            cin >> n;
            auto results = ratingTree.getTopN(n);
            cout << "\nTop " << n << " by Rating (Splay Tree):\n";
            printMovies(results, "rating");
            cout << "Query Time: " << ratingTree.queryTime << " ms\n";
        }
        else if (choice == 4)
        {
            if (!loaded) { cout << "Load the dataset first!\n"; continue; }
            int n;
            cout << "Enter N: ";
            cin >> n;
            auto results = votesHeap.getTopN(n);
            cout << "\nTop " << n << " by Votes (Max Heap):\n";
            printMovies(results, "votes");
            cout << "Query Time: " << votesHeap.queryTime << " ms\n";
        }
        else if (choice == 5)
        {
            if (!loaded) { cout << "Load the dataset first!\n"; continue; }
            int n;
            cout << "Enter N: ";
            cin >> n;
            auto results = votesTree.getTopN(n);
            cout << "\nTop " << n << " by Votes (Splay Tree):\n";
            printMovies(results, "votes");
            cout << "Query Time: " << votesTree.queryTime << " ms\n";
        }
        else if (choice == 6)
        {
            if (!loaded) { cout << "Load the dataset first!\n"; continue; }
            int n;
            cout << "Enter N: ";
            cin >> n;
            auto heapResults = ratingHeap.getTopN(n);
            auto treeResults = ratingTree.getTopN(n);
            printComparison(heapResults, treeResults, ratingHeap, ratingTree, "rating");
        }
        else if (choice == 7)
        {
            if (!loaded) { cout << "Load the dataset first!\n"; continue; }
            int n;
            cout << "Enter N: ";
            cin >> n;
            auto heapResults = votesHeap.getTopN(n);
            auto treeResults = votesTree.getTopN(n);
            printComparison(heapResults, treeResults, votesHeap, votesTree, "votes");
        }
        else if (choice == 8)
        {
            cout << "Bye!\n";
            break;
        }
        else
        {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}
