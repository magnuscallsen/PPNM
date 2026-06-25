#pragma once
#include <iostream>
#include <string>

struct vec {
    double x, y, z;

    vec(double x, double y, double z) : x(x), y(y), z(z) {}
    vec() : vec(0, 0, 0) {}
    vec(const vec&) = default;
    vec(vec&&) = default;
    ~vec() = default;

    vec& operator=(const vec&) = default;
    vec& operator=(vec&&) = default;

    vec& operator+=(const vec&);
    vec& operator-=(const vec&);
    vec& operator*=(double);
    vec& operator/=(double);

    void set(double a, double b, double c) { x = a; y = b; z = c; }
    void print(const std::string& s = "") const;

    // vector math
    double dot(const vec&) const;
    vec cross(const vec&) const;

    friend std::ostream& operator<<(std::ostream&, const vec&);
};

// non-member operators
vec operator-(const vec&);
vec operator-(const vec&, const vec&);
vec operator+(const vec&, const vec&);
vec operator*(const vec&, double);
vec operator*(double, const vec&);
vec operator/(const vec&, double);

// approximate equality
bool approx(const vec&, const vec&, double acc = 1e-6, double eps = 1e-6);
