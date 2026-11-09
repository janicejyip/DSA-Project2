//
// Created by janice on 11/4/2026.
//
#include <stdio.h>
#include <vector>
#include "custom_mergeSort.h"

using namespace std;

void mergeSort(vector<movie_ratings>& ratings, int left, int right) {
    //the job of this is to divide the vector
    //left and right are the indexes
    if (left < right) {
        //finds the middle
        int mid = left + (right - left) / 2;
        //then divide it by 2
        mergeSort(ratings, left, mid);
        mergeSort(ratings, mid + 1, right);
        //once everything is split we then use merge to combine the sorted halves
        merge(ratings, left, mid, right);
    }
}

void merge(vector<movie_ratings>& ratings, int left, int mid, int right) {
    // compares the first value of each half by creating a copy
    //then it puts the smaller value into the original vector
    //repeat until one half is empty
    //then with anyting else left over it will just copy over

    //calculate the size of each half
    int n1 = mid - left + 1;
    int n2 = right - mid;
    //create a temporary vector
    vector<movie_ratings>X(n1), Y(n2);
    //copy the left half into x and right half to y
    for (int i = 0; i < n1; i++) {
        X[i] = ratings[left + i];
    }
    for (int j = 0; j < n2; j++) {
        Y[j] = ratings[mid + 1 + j];
    }
    //each of these int will keep track the x and y position and k will track where to write the next value
    int i = 0;
    int j = 0;
    int k = left;
    //wile we haven't eached the end of x and we haven't reached the end of y
    while (i < n1 && j < n2) {
        //x is less than y than we place the x first
        //use rate from the struct because we want the actual numerical value
        if (X[i].rate >= Y[j].rate) {
            ratings[k] = X[i];
            i++;
        }
        //else if we have y is less than x we place y
        else {
            ratings[k] = Y[j];
            j++;
        }
        k++;
    }
    //once we run out of elements in x or y we can append the remaining elements
    //check to see if x still has some elements remaining
    while (i < n1) {
        ratings[k] = X[i];
        i++;
        k++;
    }
    while (j < n2) {
        ratings[k] = Y[j];
        j++;
        k++;
    }
}


