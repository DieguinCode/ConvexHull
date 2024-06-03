#include "vec2.hpp"

using namespace std;

#define PI 3.14159265

//Default Constructor
vec2::vec2() {
	this->x = 0;
	this->y = 0;
}

// constructor
vec2::vec2(double x_in, double y_in) {
	x = x_in; y = y_in;
}
// copy constructor
vec2::vec2(const vec2& v) {
	x = v.x; y = v.y;
}

// presets
vec2 const vec2::ZERO = vec2(0, 0);
vec2 const vec2::UP = vec2(0, -1);
vec2 const vec2::DOWN = vec2(0, 1);

// operators
//		with other vectors
vec2 operator+(const vec2& lhs, const vec2& rhs) {
	return vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}
vec2 operator-(const vec2& lhs, const vec2& rhs) {
	return vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}
bool operator==(const vec2& lhs, const vec2& rhs) {
	if ((lhs.x == rhs.x) and (lhs.y == rhs.y)) {
		return true;
	}
	else {
		return false;
	}
}

// operators
//		with scalars
vec2 operator*(const double& scalar, const vec2& v) {
	return vec2(v.x * scalar, v.y * scalar);
}
vec2 operator*(const vec2& v, const double& scalar) {
	return vec2(v.x * scalar, v.y * scalar);
}
vec2 operator/(const vec2& v, const double& scalar) {
	return vec2(v.x / scalar, v.y / scalar);
}
// operators
//		for printing
std::ostream& operator<<(std::ostream& os, vec2 const& v) {
	return os << "vec2(" << v.x << ", " << v.y << ")";
}

// functions
double vec2::magSquared() {
	return this->x * this->x + this->y * this->y;
}
double vec2::mag() {
	return sqrt(magSquared());
}
double vec2::dot(const vec2& other) {
	return this->x * other.x + this->y * other.y;
}
double vec2::cross(const vec2& other) {
	return (this->x * other.y) - (this->y * other.x);
}

vec2 vec2::normalized() {		// corrigir bugs de -0
	return (*this / this->mag()) + vec2::ZERO;
}
vec2 vec2::floored() {
	return vec2(floor(this->x), floor(this->y));
}
vec2 vec2::clampedPositive() {
	double max_x_0 = x < 0.0 ? 0.0 : x;
	double max_y_0 = y < 0.0 ? 0.0 : y;
	return vec2(max_x_0, max_y_0);
}
vec2 vec2::project(vec2& base) {
	return base * (this->dot(base) / base.magSquared());
}
vec2 vec2::reflect(vec2& normal) {
	return *this - (normal * (this->dot(normal)) * 2);
}