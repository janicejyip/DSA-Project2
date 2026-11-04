//
// Created by janice on 11/4/2026.
//
#include <vector>
using namespace std;

#ifndef CUSTOM_MERGESORT_H
#define CUSTOM_MERGESORT_H

void mergeSort(vector<int>& ratings, int left, int right);
void merge(vector<int>& ratings, int left, int mid, int right);

#endif //CUSTOM_MERGESORT_H
