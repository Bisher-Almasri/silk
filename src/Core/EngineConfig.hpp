//
// Created by Bisher Almasri on 2025-10-24.
//
#pragma once
#include <variant>
#include <string>
#include <unordered_map>

/**
 * Engine configuration system that manages all engine settings
 * and provides a centralized way to access configuration values.
 */
class EngineConfig
{
public:
    using ConfigValue = std::variant<bool, int, float, std::string>;

    EngineConfig();
    ~EngineConfig();

    [[nodiscard]] int GetWindowWidth() const { return m_windowWidth; }
    [[nodiscard]] int GetWindowHeight() const { return m_windowHeight; }
    [[nodiscard]] const std::string& GetWindowTitle() const { return m_windowTitle; }
    [[nodiscard]] bool IsFullscreen() const { return m_windowFullScreen; }
    [[nodiscard]] bool IsVSyncEnabled() const { return m_vsync; }

    void SetWindowSize(int width, int height);
    void SetWindowTitle(const std::string& title);
    void SetFullscreen(bool fullscreen);
    void SetVSync(bool vsync);

    [[nodiscard]] int GetMaxFPS() const { return m_maxFPS; }
    [[nodiscard]] float GetFieldOfView() const { return m_fieldOfView; }
    [[nodiscard]] float GetNearPlane() const { return m_nearPlane; }
    [[nodiscard]] float GetFarPlane() const { return m_farPlane; }
    [[nodiscard]] int GetRenderDistance() const { return m_renderDistance; }

    void SetMaxFPS(int fps);
    void SetFieldOfView(float fov);
    void SetNearPlane(float nearPlane);
    void SetFarPlane(float farPlane);
    void SetRenderDistance(int distance);

    [[nodiscard]] float GetMouseSensitivity() const { return m_mouseSensitivity; }
    [[nodiscard]] float GetMovementSpeed() const { return m_movementSpeed; }

    void SetMouseSensitivity(float sensitivity);
    void SetMovementSpeed(float speed);

    void SetValue(const std::string& key, const ConfigValue& value);
    [[nodiscard]] ConfigValue GetValue(const std::string& key, const ConfigValue& defaultValue = ConfigValue{}) const;

    template<typename T>
    T GetValueAs(const std::string& key, const T& defaultValue = T{}) const
    {
        if (const auto it = m_configValues.find(key); it != m_configValues.end())
        {
            if (std::holds_alternative<T>(it->second))
            {
                return std::get<T>(it->second);
            }
        }
        return defaultValue;
    }

    bool LoadFromFile(const std::string& filename);
    bool SaveToFile(const std::string& filename) const;

    void ResetToDefaults();

private:
    int m_windowWidth{};
    int m_windowHeight{};
    std::string m_windowTitle;
    bool m_windowFullScreen{};
    bool m_vsync{};

    int m_maxFPS{};
    float m_fieldOfView{};
    float m_nearPlane{};
    float m_farPlane{};
    int m_renderDistance{};

    float m_mouseSensitivity{};
    float m_movementSpeed{};

    std::unordered_map<std::string, ConfigValue> m_configValues;

    /**
     * Initialize default configuration values
     */
    void InitializeDefaults();

    /**
     * Sync member variables with config map values
     */
    void SyncMemberVariables();
};