#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Parses one CSV field, handles both quoted and unquoted fields
string parseField(stringstream& ss)
{
    string field;
    if (ss.peek() == '"')
    {
        ss.ignore(); 
        string chunk;
        while (getline(ss, chunk, '"'))
        {
            field += chunk;
            if (ss.peek() == '"') 
            {
                field += '"';
                ss.ignore();
            }
            else break; // closing quote
        }
        if (ss.peek() == ',') ss.ignore(); // skip comma after closing quote
    }
    else
    {
        getline(ss, field, ',');
    }
    return field;
}

vector<Movie> loadCSV(const string& filename)
{
    vector<Movie> movies;
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "couldn't open file: " << filename << endl;
        return movies;
    }

    string line;
    getline(file, line); // skip header

    while (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty()) continue;

        stringstream ss(line);

        string id          = parseField(ss);
        string title       = parseField(ss);
        string voteAvgStr  = parseField(ss);
        string voteCountStr= parseField(ss);
        string status      = parseField(ss);
        string release_date= parseField(ss);
        string revenue     = parseField(ss);
        string runtime     = parseField(ss);
        string adult       = parseField(ss);
        string backdrop    = parseField(ss);
        string budget      = parseField(ss);
        string homepage    = parseField(ss);
        string imdb_id     = parseField(ss);
        string orig_lang   = parseField(ss);
        string orig_title  = parseField(ss);
        string overview    = parseField(ss);
        string popularity  = parseField(ss);
        string poster      = parseField(ss);
        string tagline     = parseField(ss);
        string genres      = parseField(ss);

        if (voteAvgStr.empty() || voteCountStr.empty()) continue;

        try
        {
            float voteAvg  = stof(voteAvgStr);
            int voteCount  = stoi(voteCountStr);

            if (voteCount <6 || title.empty()) continue; // we had to attempt various numbers of vote count to get 100k data points

            movies.push_back(Movie(title, voteAvg, voteCount, genres));
        }
        catch (...)
        {
            continue;
        }
    }

    cout << "Loaded " << movies.size() << " movies." << endl;
    return movies;
}