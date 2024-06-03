#pragma once
#include "vec2.hpp"
#include <cmath>
#include <algorithm>

using namespace std;

//Basics
template <typename T> void swap(std::vector<T>* vec, int indexA, int indexB);
int partition(vector<double>* vec, int startIndex, int endIndex);
int partitionVec2(vector<vec2>* points, int startIndex, int endIndex, const char& parameter);
void quickSortStep(vector<double>* vec, int startIndex, int endIndex);
void quickSortStepVec2(vector<vec2>* vec, int startIndex, int endIndex, const char& parameter);
void quickSort(vector<double>* vec);
void merge(vector<double>* vec, vector<double>* leftVec, vector<double>* rightVec);
void mergeSort(vector<double>* vec);
void quickSortVec2x(vector<vec2>* points);
void quickSortVec2y(vector<vec2>* points);

//Jarvis
vector<vec2> jarvis(vector<vec2>* points);
double orientation(const vec2& p1, const vec2& p2, const vec2& p3);
double dist(const vec2& p1, const vec2& p2);

//MergeHull
vector<vec2> subproblem_jar(vector<vec2>* points);
int quad(vec2 p);
bool compare(vec2 p1, vec2 q1);
int check_ori(vec2 a, vec2 b, vec2 c);
vector<vec2> mergeHullMerger(vector<vec2> a, vector<vec2> b);
vector<vec2> mergeHullDivide(vector<vec2> a);
vector<vec2> mergeHull(vector<vec2> points);