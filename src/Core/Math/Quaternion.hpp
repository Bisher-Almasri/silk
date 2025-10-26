//
// Created by Bisher Almasri on 2025-10-25.
//
#pragma once
#include <cmath>
#include <ostream>
#include <string>
#include "Vector3.hpp"

class Quaternion
{
  public:
    float x{}, y{}, z{}, w{};

    Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
    Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    Quaternion(const Vector3& axis, float angle) { SetFromAxisAngle(axis, angle); }

    Quaternion(const Quaternion& other) = default;
    Quaternion& operator=(const Quaternion& other) = default;

    Quaternion operator+(const Quaternion& other) const
    {
        return {x + other.x, y + other.y, z + other.z, w + other.w};
    }

    Quaternion operator-(const Quaternion& other) const
    {
        return {x - other.x, y - other.y, z - other.z, w - other.w};
    }

    Quaternion operator*(const Quaternion& other) const
    {
        return {
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        };
    }

    Quaternion operator*(float scalar) const
    {
        return {x * scalar, y * scalar, z * scalar, w * scalar};
    }

    Quaternion operator/(float scalar) const
    {
        return {x / scalar, y / scalar, z / scalar, w / scalar};
    }

    Quaternion& operator+=(const Quaternion& other)
    {
        x += other.x; y += other.y; z += other.z; w += other.w;
        return *this;
    }

    Quaternion& operator-=(const Quaternion& other)
    {
        x -= other.x; y -= other.y; z -= other.z; w -= other.w;
        return *this;
    }

    Quaternion& operator*=(float scalar)
    {
        x *= scalar; y *= scalar; z *= scalar; w *= scalar;
        return *this;
    }

    Quaternion& operator*=(const Quaternion& other)
    {
        *this = *this * other;
        return *this;
    }

    bool operator==(const Quaternion& other) const
    {
        constexpr float epsilon = 1e-6f;
        return std::fabs(x - other.x) < epsilon &&
               std::fabs(y - other.y) < epsilon &&
               std::fabs(z - other.z) < epsilon &&
               std::fabs(w - other.w) < epsilon;
    }

    bool operator!=(const Quaternion& other) const { return !(*this == other); }

    float& operator[](int index) { return (&x)[index]; }
    const float& operator[](int index) const { return (&x)[index]; }

    [[nodiscard]] float Length() const { return std::sqrt(x*x + y*y + z*z + w*w); }
    [[nodiscard]] float LengthSquared() const { return x*x + y*y + z*z + w*w; }

    [[nodiscard]] Quaternion Normalized() const
    {
        float len = Length();
        if (len > 0.0f) return *this * (1.0f / len);
        return Identity();
    }

    void Normalize() { *this = Normalized(); }

    [[nodiscard]] Quaternion Conjugate() const { return {-x, -y, -z, w}; }

    [[nodiscard]] Quaternion Inverse() const
    {
        if (float ls = LengthSquared(); ls > 0.0f) return Conjugate() * (1.0f / ls);
        return Identity();
    }

    [[nodiscard]] float Dot(const Quaternion& other) const
    {
        return x*other.x + y*other.y + z*other.z + w*other.w;
    }

    void SetFromAxisAngle(const Vector3& axis, float angle)
    {
        const float half = angle * 0.5f;
        const float s = std::sin(half);
        const Vector3 n = axis.Normalized();
        x = n.x * s;
        y = n.y * s;
        z = n.z * s;
        w = std::cos(half);
    }

    void ToAxisAngle(Vector3& axis, float& angle) const
    {
        Quaternion n = Normalized();
        angle = 2.0f * std::acos(n.w);

        const float s = std::sqrt(std::max(0.0f, 1.0f - n.w * n.w));
        if (s > 1e-6f)
        {
            axis.x = n.x / s;
            axis.y = n.y / s;
            axis.z = n.z / s;
        }
        else
        {
            axis = Vector3::UP();
        }
    }

    [[nodiscard]] Vector3 ToEulerAngles() const
    {
        Vector3 angles;

        // roll (x-axis)
        float sinr_cosp = 2.0f * (w * x + y * z);
        float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
        angles.x = std::atan2(sinr_cosp, cosr_cosp);

        // pitch (y-axis)
        float sinp = 2.0f * (w * y - z * x);
        if (std::fabs(sinp) >= 1.0f)
            angles.y = std::copysign(static_cast<float>(M_PI) / 2.0f, sinp);
        else
            angles.y = std::asin(sinp);

        // yaw (z-axis)
        float siny_cosp = 2.0f * (w * z + x * y);
        float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
        angles.z = std::atan2(siny_cosp, cosy_cosp);

        return angles;
    }

    void SetFromEulerAngles(float roll, float pitch, float yaw)
    {
        const float cr = std::cos(roll * 0.5f);
        const float sr = std::sin(roll * 0.5f);
        const float cp = std::cos(pitch * 0.5f);
        const float sp = std::sin(pitch * 0.5f);
        const float cy = std::cos(yaw * 0.5f);
        const float sy = std::sin(yaw * 0.5f);

        w = cr * cp * cy + sr * sp * sy;
        x = sr * cp * cy - cr * sp * sy;
        y = cr * sp * cy + sr * cp * sy;
        z = cr * cp * sy - sr * sp * cy;
    }

    void SetFromEulerAngles(const Vector3& eulerAngles)
    {
        SetFromEulerAngles(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    }

    [[nodiscard]] Quaternion Slerp(const Quaternion& other, float t) const
    {
        float dot = Dot(other);
        Quaternion target = other;

        if (dot < 0.0f)
        {
            target = {-other.x, -other.y, -other.z, -other.w};
            dot = -dot;
        }

        if (dot > 0.9995f)
        {
            const Quaternion result = *this + (target - *this) * t;
            return result.Normalized();
        }

        float theta0 = std::acos(dot);
        float theta  = theta0 * t;
        float sinT   = std::sin(theta);
        float sinT0  = std::sin(theta0);

        float s0 = std::cos(theta) - dot * sinT / sinT0;
        float s1 = sinT / sinT0;

        return (*this * s0) + (target * s1);
    }

    static Quaternion Identity() { return {0.0f, 0.0f, 0.0f, 1.0f}; }

    static Quaternion FromAxisAngle(const Vector3& axis, float angle)
    {
        Quaternion q;
        q.SetFromAxisAngle(axis, angle);
        return q;
    }

    static Quaternion FromEulerAngles(float roll, float pitch, float yaw)
    {
        Quaternion q;
        q.SetFromEulerAngles(roll, pitch, yaw);
        return q;
    }

    static Quaternion FromEulerAngles(const Vector3& eulerAngles)
    {
        return FromEulerAngles(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    }

    static Quaternion LookRotation(const Vector3& forward, const Vector3& up = Vector3::UP())
    {
        Vector3 f = forward.Normalized();
        Vector3 u = up.Normalized();
        Vector3 r = u.Cross(f).Normalized();
        u = f.Cross(r);

        float trace = r.x + u.y + f.z;
        Quaternion q;

        if (trace > 0.0f)
        {
            float s = std::sqrt(trace + 1.0f) * 2.0f;
            q.w = 0.25f * s;
            q.x = (u.z - f.y) / s;
            q.y = (f.x - r.z) / s;
            q.z = (r.y - u.x) / s;
        }
        else if (r.x > u.y && r.x > f.z)
        {
            float s = std::sqrt(1.0f + r.x - u.y - f.z) * 2.0f;
            q.w = (u.z - f.y) / s;
            q.x = 0.25f * s;
            q.y = (u.x + r.y) / s;
            q.z = (f.x + r.z) / s;
        }
        else if (u.y > f.z)
        {
            float s = std::sqrt(1.0f + u.y - r.x - f.z) * 2.0f;
            q.w = (f.x - r.z) / s;
            q.x = (u.x + r.y) / s;
            q.y = 0.25f * s;
            q.z = (f.y + u.z) / s;
        }
        else
        {
            float s = std::sqrt(1.0f + f.z - r.x - u.y) * 2.0f;
            q.w = (r.y - u.x) / s;
            q.x = (f.x + r.z) / s;
            q.y = (f.y + u.z) / s;
            q.z = 0.25f * s;
        }

        return q;
    }

    static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t)
    {
        return a.Slerp(b, t);
    }

    [[nodiscard]] std::string ToString() const
    {
        return "Quaternion(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
               std::to_string(z) + ", " + std::to_string(w) + ")";
    }
};

inline Quaternion operator*(float scalar, const Quaternion& quaternion)
{
    return quaternion * scalar;
}

inline Vector3 operator*(const Quaternion& q, const Vector3& v)
{
    Vector3 qv{q.x, q.y, q.z};
    Vector3 t = 2.0f * qv.Cross(v);
    return v + (q.w * t) + qv.Cross(t);
}

inline std::ostream& operator<<(std::ostream& os, const Quaternion& quaternion)
{
    os << quaternion.ToString();
    return os;
}
