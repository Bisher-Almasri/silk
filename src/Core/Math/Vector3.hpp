//
// Created by Bisher Almasri on 2025-10-25.
//
#pragma once
#include <string>
#include <cmath>
#include <iostream>

class Vector3
{
  public:
    float x, y, z;
    Vector3() : x(0.0f), y(0.0f), z(0.0f)
    {
    }

    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z)
    {
    }

    explicit Vector3(const float value) : x(value), y(value), z(value)
    {
    }

    Vector3(const Vector3& other) = default;
    Vector3& operator=(const Vector3& other) = default;

    Vector3 operator+(const Vector3& other) const
    {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vector3 operator-(const Vector3& other) const
    {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vector3 operator*(float scalar) const
    {
        return {x * scalar, y * scalar, z * scalar};
    }

    Vector3 operator/(float scalar) const
    {
        return {x / scalar, y / scalar, z / scalar};
    }

    Vector3 operator-() const
    {
        return {-x, -y, -z};
    }

    Vector3& operator+=(const Vector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3& operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    bool operator==(const Vector3& other) const
    {
        constexpr float epsilon = 1e-6f;
        return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon;
    }

    bool operator!=(const Vector3& other) const
    {
        return !(*this == other);
    }

    float& operator[](int index)
    {
        return (&x)[index];
    }

    const float& operator[](int index) const
    {
        return (&x)[index];
    }

    [[nodiscard]] float Length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    [[nodiscard]] float LengthSquared() const
    {
        return x * x + y * y + z * z;
    }

    [[nodiscard]] Vector3 Normalized() const
    {
        const float length = Length();
        constexpr float epsilon = 1e-6f;
        if (length > epsilon)
        {
            return *this / length;
        }
        return {0.0f, 0.0f, 0.0f};
    }

    void Normalize()
    {
        *this = Normalized();
    }

    [[nodiscard]] float Dot(const Vector3& other) const
    {
        return x * other.x + y * other.y + z * other.z;
    }

    [[nodiscard]] float Distance(const Vector3& other) const
    {
        return (*this - other).Length();
    }

    [[nodiscard]] float DistanceSquared(const Vector3& other) const
    {
        return (*this - other).LengthSquared();
    }

    [[nodiscard]] Vector3 Cross(const Vector3& other) const
    {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x,
        };
    }

    [[nodiscard]] Vector3 Lerp(const Vector3& other, float t) const
    {
        return *this + (other - *this) * t;
    }

    [[nodiscard]] Vector3 Reflect(const Vector3& normal) const
    {
        return *this - normal * (2.0f * Dot(normal));
    }

    static Vector3 ZERO()
    {
        return {0.0f, 0.0f, 0.0f};
    }

    static Vector3 ONE()
    {
        return {1.0f, 1.0f, 1.0f};
    }

    static Vector3 UP()
    {
        return {0.0f, 1.0f, 0.0f};
    }

    static Vector3 DOWN()
    {
        return {0.0f, -1.0f, 0.0f};
    }

    static Vector3 LEFT()
    {
        return {-1.0f, 0.0f, 0.0f};
    }

    static Vector3 RIGHT()
    {
        return {1.0f, 0.0f, 0.0f};
    }

    static Vector3 FORWARD()
    {
        return {0.0f, 0.0f, -1.0f};
    }

    static Vector3 BACK()
    {
        return {0.0f, 0.0f, 1.0f};
    }

    static float Dot(const Vector3& a, const Vector3& b)
    {
        return a.Dot(b);
    }

    static Vector3 Cross(const Vector3& a, const Vector3& b)
    {
        return a.Cross(b);
    }

    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
    {
        return a.Lerp(b, t);
    }

    static float Distance(const Vector3& a, const Vector3& b)
    {
        return a.Distance(b);
    }

    [[nodiscard]] std::string ToString() const
    {
        return "Vector3(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
               std::to_string(z) + ")";
    }

    [[nodiscard]] float X() const
    {
        return x;
    }

    [[nodiscard]] float Y() const
    {
        return y;
    }

    [[nodiscard]] float Z() const
    {
        return z;
    }
};

inline Vector3 operator*(float scalar, const Vector3& vector)
{
    return vector * scalar;
}

inline std::ostream& operator<<(std::ostream& os, const Vector3& vector)
{
    os << vector.ToString();
    return os;
}