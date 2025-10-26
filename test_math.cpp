#include "src/Core/Math/Quaternion.hpp"
#include "src/Core/Math/Vector3.hpp"

#include <iostream>

int main()
{
    // Test Vector3
    Vector3 v1(1.0f, 2.0f, 3.0f);
    Vector3 v2(4.0f, 5.0f, 6.0f);
    Vector3 v3 = v1 + v2;

    std::cout << "Vector3 test: " << v3.ToString() << std::endl;
    std::cout << "Vector3 length: " << v3.Length() << std::endl;

    // Test Quaternion
    Quaternion q1 = Quaternion::Identity();
    Quaternion q2 = Quaternion::FromAxisAngle(Vector3::UP(), M_PI / 4.0f);

    std::cout << "Quaternion identity: " << q1.ToString() << std::endl;
    std::cout << "Quaternion from axis-angle: " << q2.ToString() << std::endl;

    // Test Euler angles
    Vector3 euler(0.1f, 0.2f, 0.3f);
    Quaternion q3 = Quaternion::FromEulerAngles(euler);
    Vector3 backToEuler = q3.ToEulerAngles();

    std::cout << "Original Euler: " << euler.ToString() << std::endl;
    std::cout << "Back to Euler: " << backToEuler.ToString() << std::endl;

    return 0;
}