#include "Vector2D.h"

Vector2D Vector2D::GetRandomVector2D() {
    double angle = my_random::RandomInRange(0.0, 2 * M_PI);
    double x = std::cos(angle);
    double y = std::sin(angle);
    return Vector2D(x, y);
}

Vector2D Vector2D::operator+(const Vector2D& rhs) const {
    return Vector2D(x + rhs.x, y + rhs.y);
}

Vector2D Vector2D::operator-() const {
    return Vector2D(-x, -y);
}

Vector2D Vector2D::operator-(const Vector2D& rhs) const {
    return *this + -rhs;
}
Vector2D Vector2D::operator*(const double rhs) const {
    return Vector2D(x * rhs, y * rhs);
}

Vector2D Vector2D::operator/(const double rhs) const {
    return *this * (1.0 / rhs);
}

double Vector2D::DotProduct(const Vector2D& rhs) const {
    return x * rhs.x + y * rhs.y;
}

double Vector2D::Length() const {
    return sqrt(x * x + y * y);
}

Vector2D Vector2D::Normalized() const {
    if (IsZeroLength()) {
        return Vector2D(0.0, 0.0);
    }
    return *this / Length();
}

Vector2D& Vector2D::Normalize() {
    *this = Normalized();
    return *this;
}

bool Vector2D::IsZeroLength() const {
    return (std::abs(x - 0.0) < Eps && std::abs(y - 0.0) < Eps);
}

double DotProduct(const Vector2D& lhs, const Vector2D& rhs) {
    return lhs.DotProduct(rhs);
};
