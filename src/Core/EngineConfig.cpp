//
// Created by Bisher Almasri on 2025-10-24.
//

#include "EngineConfig.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

EngineConfig::EngineConfig()
{
    InitializeDefaults();
}

EngineConfig::~EngineConfig()
= default;


void EngineConfig::InitializeDefaults()
{
    m_windowWidth = 1280;
    m_windowHeight = 720;
    m_windowTitle = "Silk Game";
    m_windowFullScreen = false;
    m_vsync = true;

    m_maxFPS = 60;
    m_fieldOfView = 45.0f;
    m_nearPlane = 0.1f;
    m_farPlane = 1000.0f;
    m_renderDistance = 8;

    m_mouseSensitivity = 0.1f;
    m_movementSpeed = 2.5f;

    m_configValues["window.width"] = m_windowWidth;
    m_configValues["window.height"] = m_windowHeight;
    m_configValues["window.title"] = m_windowTitle;
    m_configValues["window.fullscreen"] = m_windowFullScreen;
    m_configValues["window.vsync"] = m_vsync;

    m_configValues["rendering.maxFPS"] = m_maxFPS;
    m_configValues["rendering.fieldOfView"] = m_fieldOfView;
    m_configValues["rendering.nearPlane"] = m_nearPlane;
    m_configValues["rendering.farPlane"] = m_farPlane;
    m_configValues["rendering.renderDistance"] = m_renderDistance;

    m_configValues["input.mouseSensitivity"] = m_mouseSensitivity;
    m_configValues["input.movementSpeed"] = m_movementSpeed;
}

void EngineConfig::SetWindowSize(int width, int height)
{
    m_windowWidth = width;
    m_windowHeight = height;
    m_configValues["window.width"] = width;
    m_configValues["window.height"] = height;
}

void EngineConfig::SetWindowTitle(const std::string& title)
{
    m_windowTitle = title;
    m_configValues["window.title"] = title;
}

void EngineConfig::SetFullscreen(bool fullscreen)
{
    m_windowFullScreen = fullscreen;
    m_configValues["window.fullscreen"] = fullscreen;
}

void EngineConfig::SetVSync(bool enabled)
{
    m_vsync = enabled;
    m_configValues["window.vsync"] = enabled;
}

void EngineConfig::SetMaxFPS(int fps)
{
    m_maxFPS = fps;
    m_configValues["rendering.maxFPS"] = fps;
}

void EngineConfig::SetFieldOfView(float fov)
{
    m_fieldOfView = fov;
    m_configValues["rendering.fieldOfView"] = fov;
}

void EngineConfig::SetNearPlane(float nearPlane)
{
    m_nearPlane = nearPlane;
    m_configValues["rendering.nearPlane"] = nearPlane;
}

void EngineConfig::SetFarPlane(float farPlane)
{
    m_farPlane = farPlane;
    m_configValues["rendering.farPlane"] = farPlane;
}

void EngineConfig::SetRenderDistance(int distance)
{
    m_renderDistance = distance;
    m_configValues["rendering.renderDistance"] = distance;
}

void EngineConfig::SetMouseSensitivity(float sensitivity)
{
    m_mouseSensitivity = sensitivity;
    m_configValues["input.mouseSensitivity"] = sensitivity;
}

void EngineConfig::SetMovementSpeed(float speed)
{
    m_movementSpeed = speed;
    m_configValues["input.movementSpeed"] = speed;
}

void EngineConfig::SetValue(const std::string& key, const ConfigValue& value)
{
    m_configValues[key] = value;
}

EngineConfig::ConfigValue EngineConfig::GetValue(const std::string& key, const ConfigValue& defaultValue) const
{
    if (const auto it = m_configValues.find(key); it != m_configValues.end())
    {
        return it->second;
    }
    return defaultValue;
}

bool EngineConfig::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open config file: " << filename << std::endl;
        return false;
    }

    auto trim = [](std::string& s) {
        s.erase(0, s.find_first_not_of(" \t\r\n"));
        s.erase(s.find_last_not_of(" \t\r\n") + 1);
    };

    auto startsWith = [](const std::string& s, const std::string& prefix) {
        return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
    };

    auto endsWith = [](const std::string& s, const std::string& suffix) {
        return s.size() >= suffix.size() &&
               s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
    };

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        ++lineNumber;
        trim(line);
        if (line.empty() || line[0] == '#' || startsWith(line, "//"))
            continue;

        size_t equalPos = line.find('=');
        if (equalPos == std::string::npos)
        {
            std::cerr << "Warning: Skipping invalid line " << lineNumber << ": " << line << "\n";
            continue;
        }

        std::string key = line.substr(0, equalPos);
        std::string valueStr = line.substr(equalPos + 1);
        trim(key);
        trim(valueStr);

        // remove optional quotes
        if ((startsWith(valueStr, "\"") && endsWith(valueStr, "\"")) ||
            (startsWith(valueStr, "'") && endsWith(valueStr, "'")))
        {
            valueStr = valueStr.substr(1, valueStr.size() - 2);
        }

        ConfigValue value;

        // try bool
        if (valueStr == "true" || valueStr == "false")
        {
            value = (valueStr == "true");
        }
        // try int
        else if (!valueStr.empty() &&
                 std::all_of(valueStr.begin(), valueStr.end(), [](unsigned char c) {
                     return std::isdigit(c) || c == '-' || c == '+'; }))
        {
            try { value = std::stoi(valueStr); }
            catch (...) { value = valueStr; }
        }
        // try float
        else if (valueStr.find('.') != std::string::npos)
        {
            try { value = std::stof(valueStr); }
            catch (...) { value = valueStr; }
        }
        else
        {
            value = valueStr; // fallback to string
        }

        m_configValues[key] = value;
    }

    SyncMemberVariables();

    std::cout << "Configuration loaded successfully from: " << filename << std::endl;
    return true;
}

bool EngineConfig::SaveToFile(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open config file " << filename << std::endl;
        return false;
    }

    file << "# AUTOGENERATED\n\n";

    for (const auto& [key, value] : m_configValues)
    {
        file << key << " = ";

        std::visit([&file](const auto& v) {
            if constexpr (std::is_same_v<std::decay_t<decltype(v)>, bool>)
            {
                file << (v ? "true" : "false");
            }
            else
            {
                file << v;
            }
        }, value);

        file << "\n";
    }

    std::cout << "Configuration saved to: " << filename << std::endl;
    return true;
}

void EngineConfig::SyncMemberVariables()
{
    m_windowWidth = GetValueAs<int>("window.width", m_windowWidth);
    m_windowHeight = GetValueAs<int>("window.height", m_windowHeight);
    m_windowTitle = GetValueAs<std::string>("window.title", m_windowTitle);
    m_windowFullScreen = GetValueAs<bool>("window.fullscreen", m_windowFullScreen);
    m_vsync = GetValueAs<bool>("window.vsync", m_vsync);

    m_maxFPS = GetValueAs<int>("rendering.maxFPS", m_maxFPS);
    m_fieldOfView = GetValueAs<float>("rendering.fieldOfView", m_fieldOfView);
    m_nearPlane = GetValueAs<float>("rendering.nearPlane", m_nearPlane);
    m_farPlane = GetValueAs<float>("rendering.farPlane", m_farPlane);
    m_renderDistance = GetValueAs<int>("rendering.renderDistance", m_renderDistance);

    m_mouseSensitivity = GetValueAs<float>("input.mouseSensitivity", m_mouseSensitivity);
    m_movementSpeed = GetValueAs<float>("input.movementSpeed", m_movementSpeed);
}

void EngineConfig::ResetToDefaults()
{
    m_configValues.clear();
    InitializeDefaults();
}