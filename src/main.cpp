//
// Created by Bisher Almasri on 2025-10-24.
//
#include "Core/Engine.hpp"
#include "Core/EngineConfig.hpp"
#include "Core/Math/Quaternion.hpp"
#include "Core/Math/Math.hpp"
#include "Core/Math/Vector3.hpp"

#include <iostream>

int main(int argc, char* argv[])
{
    EngineConfig config;
    
    std::cout << "Loading configuration from config.ini..." << std::endl;
    config.LoadFromFile("config.ini");
    
    std::cout << "Loaded config values:" << std::endl;
    std::cout << "  Window Title: " << config.GetWindowTitle() << std::endl;
    std::cout << "  Window Size: " << config.GetWindowWidth() << "x" << config.GetWindowHeight() << std::endl;
    std::cout << "  VSync: " << (config.IsVSyncEnabled() ? "enabled" : "disabled") << std::endl;
    std::cout << "  Max FPS: " << config.GetMaxFPS() << std::endl;

    Engine engine;
    if (!engine.Initialize(config))
    {
        std::cerr << "Failed to initialize engine" << std::endl;
        return -1;
    }

    std::cout << "Testing math utilities..." << std::endl;

    Vector3 pos(1.0f, 2.0f, 3.0f);
    Vector3 target(4.0f, 5.0f, 6.0f);
    Vector3 direction = (target - pos).Normalized();

    std::cout << "Position: " << pos << std::endl;
    std::cout << "Target: " << target << std::endl;
    std::cout << "Direction: " << direction << std::endl;
    std::cout << "Distance: " << pos.Distance(target) << std::endl;

    Quaternion rotation = Quaternion::FromEulerAngles(
        Math::ToRadians(45.0f),
        Math::ToRadians(30.0f),
        Math::ToRadians(60.0f)
    );

    Vector3 rotatedVector = rotation * Vector3::FORWARD();
    std::cout << "Rotated forward vector: " << rotatedVector << std::endl;

    int exitCode = engine.Run();

    if (!config.SaveToFile("config.ini"))
    {
        std::cerr << "Failed to save config ini" << std::endl;
        return -1;
    }
    return exitCode;
}