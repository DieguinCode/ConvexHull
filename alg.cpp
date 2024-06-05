#include "alg.hpp"
#include <iostream>

using namespace std;

#define PI 3.14159265

vec2 mid;

template <typename T> void swap(vector<T>* vec, int indexA, int indexB) {
    if (indexA == indexB) return;
    T temp = vec->at(indexA);
    vec->at(indexA) = vec->at(indexB);
    vec->at(indexB) = temp;
}

int partition(vector<double>* vec, int startIndex, int endIndex) {
    int pivot = vec->at(endIndex);
    int i = startIndex - 1;
    for (int j = startIndex; j < endIndex; j++) {
        if (vec->at(j) < pivot) {
            i += 1;
            swap(vec, i, j);
        }
    }
    i += 1;
    swap(vec, i, endIndex);
    return i;
}


void quickSortStep(vector<double>* vec, int startIndex, int endIndex) {
    if (endIndex <= startIndex) return; // recursion f0 case

    int pivotIndex = partition(vec, startIndex, endIndex);
    quickSortStep(vec, startIndex, pivotIndex - 1);
    quickSortStep(vec, pivotIndex + 1, endIndex);
}


void quickSort(vector<double>* vec) {
    quickSortStep(vec, 0, vec->size() - 1);
}


void merge(vector<double>* vec, vector<double>* leftVec, vector<double>* rightVec) {
    vector<double> vecCopy;
    int size = vec->size();
    int leftSize = size / 2;
    int rightSize = size - leftSize;
    int l = 0;
    int r = 0;
    int step = 0;
    while (l < leftSize && r < rightSize) {
        if (leftVec->at(l) < rightVec->at(r)) {
            vec->at(step) = leftVec->at(l);
            l += 1;
        }
        else {
            vec->at(step) = rightVec->at(r);
            r += 1;
        }
        step++;
    }
    while (l < leftSize) {
        vec->at(step) = leftVec->at(l);
        l += 1;
        step++;
    }
    while (r < rightSize) {
        vec->at(step) = rightVec->at(r);
        r += 1;
        step++;
    }
}


void mergeSort(vector<double>* vec) {
    int subVecSize = vec->size();
    if (subVecSize < 2) return; //already sorted

    //two subvectors split in half
    int half = subVecSize / 2; // int division = floor

    vector<double> leftSubVec;
    for (int i = 0; i <= half - 1; i++) {
        leftSubVec.push_back(vec->at(i));
    }
    vector<double> rightSubVec;
    for (int i = half; i < subVecSize; i++) {
        rightSubVec.push_back(vec->at(i));
    }
    mergeSort(&leftSubVec);
    mergeSort(&rightSubVec);
    merge(vec, &leftSubVec, &rightSubVec);
}


int partitionVec2(vector<vec2>* points, int startIndex, int endIndex, const char& parameter) {
    if (parameter == 'x') {
        double pivot = points->at(endIndex).x;
        int i = startIndex - 1;
        for (int j = startIndex; j < endIndex; j++) {
            if (points->at(j).x < pivot) {
                i += 1;
                swap(points, i, j);
            }
        }
        i += 1;
        swap(points, i, endIndex);
        return i;
    }
    else if (parameter == 'y') {
        double pivot = points->at(endIndex).y;
        int i = startIndex - 1;
        for (int j = startIndex; j < endIndex; j++) {
            if (points->at(j).y < pivot) {
                i += 1;
                swap(points, i, j);
            }
        }
        i += 1;
        swap(points, i, endIndex);
        return i;
    }
    else {
        runtime_error("Invalid Parameter!");
    }
}

void quickSortStepVec2(vector<vec2>* vec, int startIndex, int endIndex, const char& parameter) {
    if ((parameter == 'x') or (parameter == 'y')) {
        if (endIndex <= startIndex) return; // recursion f0 case

        int pivotIndex = partitionVec2(vec, startIndex, endIndex, parameter);
        quickSortStepVec2(vec, startIndex, pivotIndex - 1, parameter);
        quickSortStepVec2(vec, pivotIndex + 1, endIndex, parameter);
    }
    else {
        runtime_error("Invalid Parameter!");
    }
}

void quickSortVec2x(vector<vec2>* points) {
    quickSortStepVec2(points, 0, points->size() - 1, 'x');
}

void quickSortVec2y(vector<vec2>* points) {
    quickSortStepVec2(points, 0, points->size() - 1, 'y');
}


double orientation(const vec2& p1, const vec2& p2, const vec2& p3) {
    //Compare the slope between two known points. So, in the end of the day, we can know the orientation.
    double res = (p3.y - p2.y) * (p2.x - p1.x) - (p2.y - p1.y) * (p3.x - p2.x);
    if (res > 0) {
        //CCW
        return 1;
    }
    else if (res < 0) {
        //CW
        return -1;
    }
    else {
        //Colinear
        return 0;
    }
}

double dist(const vec2& p1, const vec2& p2) {
    return sqrt(pow(p2.y - p1.y, 2) + pow(p2.x - p1.x, 2));
}

vector<vec2> jarvis(vector<vec2>* points) {

    vector<vec2> res{};

    //First Step -> Find P0
    quickSortVec2x(points);

    /*Pathological Case : If there are extreme points with the same X value,
    we will take the point with the highest Y value among these */
    int on_hull = 0;
    if (points->at(0).x == points->at(1).x) {
        for (int i = 1; i < (points->size() - 1); i++) {
            if (points->at(i).x != points->at(i + 1).x) {
                //Index 'i' is the last position which contains a candidate point.
                double y_max = points->at(0).y;
                for (int j = 1; j <= i; j++) {
                    if (points->at(j).y > y_max) {
                        y_max = points->at(j).y;
                        on_hull = j;
                    }
                }
                break;
            }
        }
    }

    //Now, we have p0!


    //Now, let's go to the magic!
    bool cond = true;

    while (cond) {
        res.push_back(points->at(on_hull));
        vec2 next_point = points->at(0); //The fist point in the set
        vec2 point_in_the_hull = points->at(on_hull);
        for (int i = 0; i < points->size(); i++) {
            /* 1- point_in_the_hull is the last point appended in the convex hull, and we are sure
            that this point belongs to the convex hull subset.
               2- next_point is the next candidate to be appended. We must check the orientation!
               3- points->at(i) is used to run the set of points */
            double o = orientation(point_in_the_hull, next_point, points->at(i));
            /*We update the next_point when the orientation is CCW(o == 1).But also, when the actual
            next_point is the last one we added to the convex hull, and sure, we shouldn't add a point
            which is already in the result subset.
            And, sure, if we have 3 collinear points, we must take the far way point to minimize
            the result subset-> The point in that moment of the loop is the new candidate*/
            if ((next_point == point_in_the_hull) or (o == 1) or (o == 0 and
                dist(point_in_the_hull, points->at(i)) >= dist(point_in_the_hull, next_point))) {
                next_point = points->at(i);
                on_hull = i;
            }
        }
        //Now, we have the right candidate.

        //Loop Conditional
        if (points->at(on_hull) == res.at(0)) {
            cond = false;
        }
    }

    res.push_back(res.at(0));
    return res;
}

//MergeHull

/*To determine the quadrant
(+, +) -> Fist Quadrant
(-, +) -> Second Quadrant
(-, -) -> Third Quadrant
(+, -) -> Forth Quadrant */
int quad(vec2 p) {
    if (p.x >= 0 && p.y >= 0)
        return 1;
    if (p.x <= 0 && p.y >= 0)
        return 2;
    if (p.x <= 0 && p.y <= 0)
        return 3;
    return 4;
}

//Used inside std::sort function -> Sorting in CCW order
bool compare(vec2 p1, vec2 q1) {
    vec2 p{ p1.x - mid.x, p1.y - mid.y };
    vec2 q{ q1.x - mid.x, q1.y - mid.y };

    int one = quad(p);
    int two = quad(q);

    if (one != two)
        return (one < two);
    return (p.y * q.x < q.y * p.x);
}

// Checks the orientation of the triplet (a, b, c)
// 0 -> a, b and c are collinear
// 1 -> Clockwise
// -1 -> Counterclockwise
int check_ori(vec2 a, vec2 b, vec2 c) {
    double res = (b.y - a.y) * (c.x - b.x) - (c.y - b.y) * (b.x - a.x);

    if (res == 0)
        return 0;
    if (res > 0)
        return 1;
    return -1;
}

vector<vec2> subproblem_jar(vector<vec2>* points) {
    vector<vec2> res{};

    cout << "Subproblem was called to the following subset: " << endl;

    for (size_t i = 0; i < points->size(); i++) {
        cout << points->at(i) << " ";
    }

    cout << endl;

    //First Step -> Find P0
    quickSortVec2x(points);

    /*Pathological Case : If there are extreme points with the same X value,
    we will take the point with the highest Y value among these */
    int on_hull = 0;
    if (points->at(0).x == points->at(1).x) {
        for (int i = 1; i < (points->size() - 1); i++) {
            if (points->at(i).x != points->at(i + 1).x) {
                //Index 'i' is the last position which contains a candidate point.
                double y_max = points->at(0).y;
                for (int j = 1; j <= i; j++) {
                    if (points->at(j).y > y_max) {
                        y_max = points->at(j).y;
                        on_hull = j;
                    }
                }
                break;
            }
        }
    }

    //Now, we have p0!


    //Now, let's go to the magic!
    bool cond = true;

    while (cond) {
        res.push_back(points->at(on_hull));
        vec2 next_point = points->at(0); //The fist point in the set
        vec2 point_in_the_hull = points->at(on_hull);
        for (int i = 0; i < points->size(); i++) {
            /* 1- point_in_the_hull is the last point appended in the convex hull, and we are sure
            that this point belongs to the convex hull subset.
               2- next_point is the next candidate to be appended. We must check the orientation!
               3- points->at(i) is used to run the set of points */
            double o = orientation(point_in_the_hull, next_point, points->at(i));
            /*We update the next_point when the orientation is CCW(o == 1).But also, when the actual
            next_point is the last one we added to the convex hull, and sure, we shouldn't add a point
            which is already in the result subset.
            And, sure, if we have 3 collinear points, we must take the far way point to minimize
            the result subset-> The point in that moment of the loop is the new candidate*/
            if ((next_point == point_in_the_hull) or (o == 1) or (o == 0 and
                dist(point_in_the_hull, points->at(i)) >= dist(point_in_the_hull, next_point))) {
                next_point = points->at(i);
                on_hull = i;
            }
        }
        //Now, we have the right candidate.

        //Loop Conditional
        if (points->at(on_hull) == res.at(0)) {
            cond = false;
        }
    }

    // Sorting the points in the anti-clockwise order
    mid = { 0, 0 };
    int n = res.size();
    for (int i = 0; i < n; i++) {
        mid.x += res[i].x;
        mid.y += res[i].y;
    }
    mid.x = mid.x / n;
    mid.y = mid.y / n;
    //Now we have the centroid!
    sort(res.begin(), res.end(), compare);

    cout << "Poligono Sub Problema Ordenado em sentido CCW:" << endl;

    for (size_t i = 0; i < res.size(); i++) {
        cout << res.at(i) << endl;
    }

    return res;
}

vector<vec2> mergeHullMerger(vector<vec2> a, vector<vec2> b) {
    // n1 -> number of points in polygon a
    // n2 -> number of points in polygon b
    int n1 = a.size(), n2 = b.size();

    int ia = 0, ib = 0;
    for (int i = 1; i < n1; i++)
        if (a[i].x > a[ia].x)
            ia = i;

    // ib -> leftmost point of b
    for (int i = 1; i < n2; i++)
        if (b[i].x < b[ib].x)
            ib = i;

    // finding the upper tangent
    int inda = ia, indb = ib;
    bool done = 0;
    while (!done) {
        done = 1;
        while (check_ori(b[indb], a[inda], a[(inda + 1) % n1]) > 0)
            inda = (inda + 1) % n1;

        while (check_ori(a[inda], b[indb], b[(n2 + indb - 1) % n2]) < 0) {
            indb = (n2 + indb - 1) % n2;
            done = 0;
        }
    }

    int uppera = inda, upperb = indb;

    cout << "Upper Tangent: " << "Up_Left: " << a.at(uppera) << " | Up_Right: " << b.at(upperb) << endl;

    inda = ia, indb = ib;
    done = 0;
    while (!done) { // finding the lower tangent
        done = 1;
        while (check_ori(a[inda], b[indb], b[(indb + 1) % n2]) > 0)
            indb = (indb + 1) % n2;

        while (check_ori(b[indb], a[inda], a[(n1 + inda - 1) % n1]) < 0) {
            inda = (n1 + inda - 1) % n1;
            done = 0;
        }
    }

    int lowera = inda, lowerb = indb;
    
    cout << "Lower Tangent: " << "Lower_Left: " << a.at(lowera) << " | Lower_Right: " << b.at(lowerb) << endl;
    
    vector<vec2> ret;

    // ret contains the convex hull after merging the two convex hulls
    // with the points sorted in anti-clockwise order
    int ind = uppera;
    ret.push_back(a[uppera]);
    while (ind != lowera) {
        ind = (ind + 1) % n1;
        ret.push_back(a[ind]);
    }

    ind = lowerb;
    ret.push_back(b[lowerb]);
    while (ind != upperb) {
        ind = (ind + 1) % n2;
        ret.push_back(b[ind]);
    }
    return ret;
}

// Returns the convex hull for the given set of points
vector<vec2> mergeHullDivide(vector<vec2> a) {
    // If the number of points is less than 6 then the
    // function uses the jarvis algorithm to find the
    // convex hull
    if (a.size() <= 5) {
        return subproblem_jar(&a);
    }

    // left contains the left half points
    // right contains the right half points
    vector<vec2> left, right;
    for (int i = 0; i < a.size() / 2; i++)
        left.push_back(a[i]);
    for (int i = a.size() / 2; i < a.size(); i++)
        right.push_back(a[i]);

    // convex hull for the left and right sets
    vector<vec2> left_hull = mergeHullDivide(left);
    vector<vec2> right_hull = mergeHullDivide(right);

    // merging the convex hulls
    return mergeHullMerger(left_hull, right_hull);
}

vector<vec2> mergeHull(vector<vec2> points) {

    //First, we need to sort according to the x-axis
    quickSortVec2x(&points);

    cout << "InputPoints have already sorted according to the x-asis:" << endl;

    for (size_t i = 0; i < points.size(); i++) {
        cout << points.at(i) << " ";
    }

    cout << endl;

    //Do the Magic!
    vector<vec2> res = mergeHullDivide(points);
    res.push_back(res.at(0));

    return res;
}