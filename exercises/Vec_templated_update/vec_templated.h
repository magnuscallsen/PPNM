#pragma once
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

template<typename T>
struct vec {
    T x, y, z;

    // constructors
    vec(T x, T y, T z) : x(x), y(y), z(z) {}
    vec() : vec(0, 0, 0) {}
    vec(const vec&) = default;
    vec(vec&&) = default;
    ~vec() = default;

    vec& operator=(const vec&) = default;
    vec& operator=(vec&&) = default;

    // compound operators
    vec& operator+=(const vec& v) {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    vec& operator-=(const vec& v) {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    vec& operator*=(T a) {
        x *= a; y *= a; z *= a;
        return *this;
    }

    vec& operator/=(T a) {
        x /= a; y /= a; z /= a;
        return *this;
    }

    // utilities
    void set(T a, T b, T c) {
        x = a; y = b; z = c;
    }

    void print(const std::string& s = "") const {
        std::cout << s << "{ " << x << ", " << y << ", " << z << " }\n";
    }

    // vector math
    T dot(const vec& v) const {
        return x*v.x + y*v.y + z*v.z;
    }

    vec cross(const vec& v) const {
        return vec(
            y*v.z - z*v.y,
            z*v.x - x*v.z,
            x*v.y - y*v.x
        );
    }

    // stream output
    friend std::ostream& operator<<(std::ostream& os, const vec& v) {
        os << "{ " << v.x << ", " << v.y << ", " << v.z << " }";
        return os;
    }
};

// ===== non-member operators =====

template<typename T>
inline vec<T> operator+(const vec<T>& a, const vec<T>& b) {
    return vec<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}
template<typename T>
inline vec<T> operator-(const vec<T>& a, const vec<T>& b) {
    return vec<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}
template<typename T>
inline vec<T> operator-(const vec<T>& a) {
    return vec<T>(-a.x, -a.y, -a.z);
}
template<typename T>
inline vec<T> operator*(const vec<T>& a, T b) {
    return vec<T>(a.x*b, a.y*b, a.z*b);
}
template<typename T>
inline vec<T> operator*(T b, const vec<T>& a) {
    return vec<T>(a.x*b, a.y*b, a.z*b);
}
template<typename T>
inline vec<T> operator/(const vec<T>& a, T b) {
    return vec<T>(a.x/b, a.y/b, a.z/b);
}

// dot product via operator*
template<typename T>
inline T operator*(const vec<T>& a, const vec<T>& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

// ===== approx =====

template<typename T>
inline bool approx(T a, T b, T acc, T eps) {
    if (std::fabs(a - b) < acc) return true;
    if (std::fabs(a - b) < std::max(std::fabs(a), std::fabs(b)) * eps) return true;
    return false;
}

template<typename T>
inline bool approx(const vec<T>& a, const vec<T>& b, T acc = T(1e-6), T eps = T(1e-6)) {
    return approx(a.x, b.x, acc, eps)
        && approx(a.y, b.y, acc, eps)
        && approx(a.z, b.z, acc, eps);
}
