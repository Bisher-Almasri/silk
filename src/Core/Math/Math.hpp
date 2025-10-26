//
// Created by Bisher Almasri on 2025-10-25.
//
#pragma once
#include "Vector3.hpp"

#include <algorithm>

namespace Math
{
constexpr float PI = 3.14159265358979323846f;
constexpr float TWO_PI = 2.0f * PI;
constexpr float HALF_PI = PI * 0.5f;
constexpr float DEG_TO_RAD = PI / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / PI;
constexpr float EPSILON = 1e-6f;

inline float ToRadians(float degree)
{
    return degree * DEG_TO_RAD;
}

inline float ToDegrees(float radian)
{
    return radian * DEG_TO_RAD;
}

inline float Clamp(float value, float min, float max)
{
    return std::max(min, std::min(value, max));
}

inline float Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

inline float InverseLerp(float a, float b, float value)
{
    if (std::abs(a - b) < EPSILON)
        return 0.0f;
    return (value - a) / (b - a);
}

inline float Smoothstep(float edge0, float edge1, float x)
{
    float t = Clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

inline bool IsNearlyEqual(float a, float b, float epsilon = EPSILON)
{
    return std::abs(a - b) < epsilon;
}

inline bool IsNearlyZero(float value, float epsilon = EPSILON)
{
    return std::abs(value) < epsilon;
}

inline float Sign(float value)
{
    return (value > 0.0f) ? 1.0f : (value < 0.0f) ? -1.0f : 0.0f;
}

inline int FloorToInt(float value)
{
    return static_cast<int>(std::floor(value));
}

inline int CeilToInt(float value)
{
    return static_cast<int>(std::ceil(value));
}

inline int RoundToInt(float value)
{
    return static_cast<int>(std::round(value));
}

inline Vector3 Min(const Vector3& a, const Vector3& b)
{
    return Vector3{std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}

inline Vector3 Max(const Vector3& a, const Vector3& b)
{
    return Vector3{std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}

inline Vector3 Clamp(const Vector3& value, const Vector3& min, const Vector3& max)
{
    return Vector3{Clamp(value.x, min.x, max.x), Clamp(value.y, min.y, max.y),
                   Clamp(value.z, min.z, max.z)};
}

inline Vector3 Abs(const Vector3& value)
{
    return Vector3{std::abs(value.x), std::abs(value.y), std::abs(value.z)};
}

inline Vector3 Floor(const Vector3& value)
{
    return Vector3{std::floor(value.x), std::floor(value.y), std::floor(value.z)};
}

inline Vector3 Ceil(const Vector3& value)
{
    return Vector3{std::ceil(value.x), std::ceil(value.y), std::ceil(value.z)};
}

inline Vector3 Round(const Vector3& value)
{
    return Vector3{std::round(value.x), std::round(value.y), std::round(value.z)};
}

} // namespace Math