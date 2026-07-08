#pragma once
#include <string>

using namespace std;

struct Movie
{
    string title;
    float vote_average;
    int vote_count;
    string genres;

    Movie(string t, float v, int vc, string g)
    {
        title = t;
        vote_average = v;
        vote_count = vc;
        genres = g;
    }
};