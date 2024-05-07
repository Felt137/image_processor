#ifndef CPP_HSE_VECTOR2D_H
#define CPP_HSE_VECTOR2D_H
#pragma once

#include <cmath>
#include <concepts>
#include <random>
#include "../Misc/Random.h"

class Vector2D {
public:
    double x, y;
    Vector2D() = default;
    ~Vector2D() = default;
    template <typename T>
    requires(std::is_floating_point_v<T> || std::is_integral_v<T>) Vector2D(T x, T y) : x(x), y(y) {
    }
    static Vector2D GetRandomVector2D();
    Vector2D operator+(const Vector2D& rhs) const;
    Vector2D operator-() const;
    Vector2D operator-(const Vector2D& rhs) const;
    Vector2D operator*(const double rhs) const;
    Vector2D operator/(const double rhs) const;
    double DotProduct(const Vector2D& rhs) const;
    double Length() const;
    Vector2D Normalized() const;
    Vector2D& Normalize();
    bool IsZeroLength() const;

private:
    constexpr static double Eps = 1e-8;
};

double DotProduct(const Vector2D& lhs, const Vector2D& rhs);
#endif  // CPP_HSE_VECTOR2D_H
