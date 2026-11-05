//
// Created by janice on 11/4/2026.
//
#include <vector>
using namespace std;

#ifndef CUSTOM_MERGESORT_H
#define CUSTOM_MERGESORT_H

struct movie_ratings {
    int userid;
    int movieid;
    double rate;
    int timestamp;
};


void mergeSort(vector<movie_ratings>& ratings, int left, int right);
void merge(vector<movie_ratings>& ratings, int left, int mid, int right);

#endif //CUSTOM_MERGESORT_H
