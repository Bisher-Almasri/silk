//
// Created by Bisher Almasri on 2025-10-24.
//
#pragma once
#include <GLFW/glfw3.h>
#include <chrono>

/**
 * Core engine class that manages initialization, main loop coordination,
 * and system lifecycle management for the voxel game engine.
 */
class Engine
{
  public:
    Engine();
    ~Engine();

    /**
     * Initialize the engine with the given configuration
     * @return true if initialization was successful
     */
    bool Initialize();

    /**
     * Run the main engine loop
     * @return Exit code (0 for success)
     */
    int Run();

    /**
     * Shutdown the engine and cleanup resources
     */
    void Shutdown();

    /**
     * Get the current frame delta time in seconds
     */
    [[nodiscard]] float GetDeltaTime() const
    {
        return m_deltaTime;
    }

    /**
     * Get the current frame rate
     */
    [[nodiscard]] float GetFrameRate() const
    {
        return m_frameRate;
    }

    /**
     * Get the GLFW window handle
     */
    [[nodiscard]] GLFWwindow* GetWindow() const
    {
        return m_window;
    }

    /**
     * Check if the engine is running
     */
    [[nodiscard]] bool IsRunning() const
    {
        return m_isRunning;
    }

  private:
    GLFWwindow* m_window;
    bool m_isRunning;

    std::chrono::high_resolution_clock::time_point m_lastFrameTime;
    float m_deltaTime;
    float m_frameRate;

    static constexpr int FRAME_RATE_SAMPLE_COUNT = 60;
    float m_frameTimeSamples[FRAME_RATE_SAMPLE_COUNT]{};
    int m_frameTimeSampleIndex;

    /**
     * Initialize GLFW and create the main window
     */
    bool InitializeWindow();

    /**
     * Initialize OpenGL context and settings
     */
    bool InitializeGraphics();

    /**
     * Initialize all engine systems
     */
    bool InitializeSystems();

    /**
     * Update engine timing information
     */
    void UpdateTiming();

    /**
     * Update all engine systems
     */
    void Update();

    /**
     * Render the current frame
     */
    void Render();

    /**
     * Handle window resize events
     */
    static void OnWindowResize(GLFWwindow* window, int width, int height);

    /**
     * Handle GLFW error callbacks
     */
    static void OnGLFWError(int error, const char* description);
};