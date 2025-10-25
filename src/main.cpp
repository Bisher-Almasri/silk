//
// Created by Bisher Almasri on 2025-10-24.
//
#include "Core/Engine.hpp"
#include "Core/EngineConfig.hpp"
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

    int exitCode = engine.Run();

    if (!config.SaveToFile("config.ini"))
    {
        std::cerr << "Failed to save config ini" << std::endl;
        return -1;
    }
    return exitCode;
}