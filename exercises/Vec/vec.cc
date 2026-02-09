#include "vec.h"
#include <cmath>
#include <iostream>

vec operator+(const vec& a, const vec& b) {
    return vec(
        a.x + b.x,
        a.y + b.y,
        a.z + b.z
    );
}

vec operator-(const vec& a, const vec& b) {
    return vec(
        a.x - b.x,
        a.y - b.y,
        a.z - b.z
    );
}

vec operator*(const vec& a, double b) {
    return vec(
        b*a.x ,
        b*a.y,
        b*a.z
    );
}

vec operator*(double b, const vec& a) {
    return vec(
        b*a.x ,
        b*a.y,
        b*a.z
    );
}

vec operator/(const vec& a, double b) {
    return vec(        a.x/b ,
        a.y/b,
        a.z/b
    );
}

double operator*(const vec& a, const vec& b){
	return a.x*b.x + a.y*b.y+a.z*b.z;
}

std::ostream& operator<<(std::ostream& os, const vec& v) {
    os << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
    return os;
}

/* =========================
   approx comparison
   ========================= */

static bool approx(double a, double b, double acc, double eps) {
    if (std::fabs(a - b) < acc) return true;
    if (std::fabs(a - b) < std::max(std::fabs(a), std::fabs(b)) * eps) return true;
    return false;
}

bool approx(const vec& a, const vec& b, double acc, double eps) {
    if (!approx(a.x, b.x, acc, eps)) return false;
    if (!approx(a.y, b.y, acc, eps)) return false;
    if (!approx(a.z, b.z, acc, eps)) return false;
    return true;
}
vec& vec::operator+=(const vec& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

vec& vec::operator-=(const vec& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

vec& vec::operator*=(double a) {
    x *= a;
    y *= a;
    z *= a;
    return *this;
}

vec& vec::operator/=(double a) {
    x /= a;
    y /= a;
    z /= a;
    return *this;
}
