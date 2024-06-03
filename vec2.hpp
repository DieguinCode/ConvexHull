#pragma once
#include <vector>
#include <stdexcept>
#include <iso646.h>
#include <algorithm>
#include <iostream>

using namespace std;

class vec2 {
public:
	// presets
	static const vec2 ZERO;
	static const vec2 UP;
	static const vec2 DOWN;

	// variables
	double x, y;

	// constructor
	vec2();
	vec2(double x_in, double y_in);
	vec2(const vec2& v); //copy

	// operators
	//		with other vectors
	friend vec2 operator+(const vec2& lhs, const vec2& rhs);
	friend vec2 operator-(const vec2& lhs, const vec2& rhs);
	friend bool operator==(const vec2& lhs, const vec2& rhs);
	//		with scalars
	friend vec2 operator*(const double& scalar, const vec2& v);
	friend vec2 operator*(const vec2& v, const double& scalar);
	friend vec2 operator/(const vec2& v, const double& scalar);
	//		printing
	friend std::ostream& operator<<(std::ostream& os, vec2 const& v);

	// functions
	double magSquared(); // faster
	double mag();
	double dot(const vec2& other);
	double cross(const vec2& other);

	vec2 normalized();
	vec2 floored();
	vec2 clampedPositive();
	vec2 project(vec2& base);
	vec2 reflect(vec2& normal);
};