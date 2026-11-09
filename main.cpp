#include <iostream>
#include "custom_mergeSort.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <unordered_map>
#include <algorithm>

using namespace std;
using namespace std::chrono;

struct movie_info {
    int movie_id;
    string title;
    string genres;
};

vector<string> readCSV(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;

    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(field);
    return fields;
}

void readRatings(vector<movie_ratings>& ratings) {
    ifstream file("../resources/ml-32m/ratings.csv");
    if (!file.is_open()) {
        cout << "Error opening ratings.csv" << endl;
        return;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        movie_ratings r;

        getline(ss, field, ',');
        r.userid = stoi(field);
        getline(ss, field, ',');
        r.movieid = stoi(field);
        getline(ss, field, ',');
        r.rate = stof(field);
        getline(ss, field, ',');
        r.timestamp = stoi(field);
        ratings.push_back(r);
    }
    file.close();
}

void readMovies(unordered_map<int, movie_info>& movies) {
    ifstream file("../resources/ml-32m/movies.csv");
    if (!file.is_open()) {
        cout << "Error opening movies.csv" << endl;
        return;
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        vector<string> fields = readCSV(line);
        movie_info m;
        m.movie_id = stoi(fields[0]);
        m.title = fields[1];
        m.genres = fields[2];
        movies[m.movie_id] = m;
    }
    file.close();
}

void printRating(movie_ratings rating, unordered_map<int, movie_info>& movies) {
    auto it = movies.find(rating.movieid);
    cout << endl;
    cout << "Movie: " << it->second.title << endl;
    cout << "ID: " << rating.movieid << endl;
    cout << "Rating: " << rating.rate << endl;
    cout << "User ID: " << rating.userid << endl;
    cout << "Rating timestamp: " << rating.timestamp << endl;
    //cout << "Genres: " << it->second.genres << endl;
}

int main() {
    // load data into memory
    vector<movie_ratings> ratings;
    unordered_map<int, movie_info> movies;
    ratings.reserve(32000204);
    readRatings(ratings);
    readMovies(movies);

    // diagnostic data
    cout << "Current working directory: " << filesystem::current_path() << endl;
    cout << "Loaded " << ratings.size() << " ratings." << endl;
    cout << "Loaded " << movies.size() << " movies." << endl;

    // keep track of sorts
    bool sorted = false;
    double mergetime = 0;
    double quicktime = 0;

    // main program loop
    while (true) {
        // option list
        int choice;
        cout << endl;
        cout << "CineSort v.1.0" << endl;
        cout << endl;
        cout << "Options:" << endl;
        cout << "1. Sort all ratings with merge sort!" << endl;
        cout << "2. Sort all ratings with QuickSort! (std::sort)" << endl;
        cout << "3. Read the worst ratings." << endl;
        cout << "4. Read the best ratings." << endl;
        cout << "5. Show sort performance difference." << endl;
        cout << "0. Quit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1) {
            // 1. Sort all ratings with merge sort!
            cout << endl;
            if (sorted == true) {
                cout << "Rereading ratings..." << endl;
                readRatings(ratings);
            }
            cout << "Sorting with merge sort..." << endl;
            auto start = high_resolution_clock::now();
            mergeSort(ratings, 0, ratings.size() - 1);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start);
            cout << "Sorted with merge sort (" << duration.count() << " ms)."<< endl;
            mergetime = duration.count();
            sorted = true;
        } else if (choice == 2) {
            // 2. Sort all ratings with QuickSort! (std::sort)
            cout << endl;
            if (sorted == true) {
                cout << "Rereading ratings..." << endl;
                readRatings(ratings);
            }
            cout << "Sorting with QuickSort..." << endl;
            auto start = high_resolution_clock::now();
            sort(ratings.begin(), ratings.end(),
                 [](const movie_ratings& a, const movie_ratings& b) {
                     return a.rate > b.rate;
                 }
            );
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start);
            cout << "Sorted with QuickSort (" << duration.count() << " ms)."<< endl;
            quicktime = duration.count();
            sorted = true;
        } else if (choice == 3) {
            // 3. Read the worst ratings.
            int n;
            cout << "Enter the number of ratings to view: ";
            cin >> n;
            for (int i = 0; i < n; i++) {
                printRating(ratings[ratings.size() - 1 - i], movies);
            }
        } else if (choice == 4) {
            // 4. Read the best ratings.
            int n;
            cout << "Enter the number of ratings to view: ";
            cin >> n;
            for (int i = 0; i < n; i++) {
                printRating(ratings[i], movies);
            }
        } else if (choice == 5) {
            // 5. Show sort performance difference.
            cout << endl;
            if (mergetime == 0 or quicktime == 0) {
                cout << "Please sort using both sorts first." << endl;
            } else {
                if (mergetime > quicktime) {
                    cout << "QuickSort faster by " << (mergetime - quicktime) << " ms." << endl;
                } else if (quicktime > mergetime) {
                    cout << "Merge sort faster by " << (quicktime - mergetime) << " ms." << endl;
                } else {
                    cout << "Both sorts took the same amount of time!" << endl;
                }
            }
        } else if (choice == 0) {
            // exit loop
            break;
        } else {
            // unknown option error
            cout << "Unknown option. Please try again." << endl;
        }
    }

    return 0;
}