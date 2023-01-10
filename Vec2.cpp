#include "Vec2.h"

Vec2::Vec2() : x(0), y(0) {

}

 Vec2::Vec2(float nx, float ny) : x(nx), y(ny) {

}

float Vec2::Length() const {
    return sqrtf(LengthSquare());
}

 float Vec2::LengthSquare() const {
    return Dot(*this);
}

 float Vec2::Dot(const Vec2& other) const {
    return x * other.x + y * other.y;
}

 float Vec2::Cross(const Vec2& other) const {
    return x * other.y - y * other.x;
}

float Vec2::DistanceFrom(const Vec2& other) const {
    return (other - *this).Length();
}

Vec2 Vec2::Normalized() const {
    if (Length() != 0)
        return *this / Length();
    else
        return *this;
}

 bool Vec2::isZero() const {
    return x == 0.0 && y == 0.0;
}

void Vec2::setZero() {
    x = 0.0;
    y = 0.0;
}

Vec2 Vec2::Rotation(float t) const {
    return{ x * cosf(t) - x * sinf(t) ,y * sinf(t) + y * cosf(t) };
}

 Vec2 Vec2::operator +() const {
    return *this;
}

 Vec2 Vec2::operator -() const {
    return{ -x, -y };
}

 Vec2 Vec2::operator +(const Vec2& other) const {
    return{ x + other.x, y + other.y };
}

Vec2 Vec2::operator -(const Vec2& other) const {
    return{ x - other.x, y - other.y };
}

Vec2 Vec2::operator *(float s) const {
    return{ x * s, y * s };
}

 Vec2 Vec2::operator /(float s) const {
    return{ x * 1 / s, y * 1 / s };
}

Vec2& Vec2::operator +=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vec2& Vec2::operator -=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2& Vec2::operator *=(float s) {
    x *= s;
    y *= s;
    return *this;
}

Vec2& Vec2::operator /=(float s) {
    x /= s;
    y /= s;
    return *this;
}

 Vec2 operator *(float s, const Vec2& v) {
    return{ s * v.x, s * v.y };
}