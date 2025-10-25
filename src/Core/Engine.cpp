//
// Created by Bisher Almasri on 2025-10-24.
//

#include "Engine.hpp"

#include <iostream>
#include <ostream>

Engine::Engine()
    : m_window(nullptr)
    , m_isRunning(false)
    , m_deltaTime(0.0f)
    , m_frameRate(0.0f)
    , m_frameTimeSampleIndex(0)
{
    std::fill(std::begin(m_frameTimeSamples), std::end(m_frameTimeSamples), 0.0f);
}

Engine::~Engine()
{
    Shutdown();
}

bool Engine::Initialize()
{
    std::cout << "Starting Engine " << std::endl;

    if (!InitializeWindow())
    {
        std::cerr << "Failed to initialize Window" << std::endl;
        return false;
    }

    if (!InitializeGraphics())
    {
        std::cerr << "Failed to initialize Graphics" << std::endl;
        return false;
    }

    if (!InitializeSystems())
    {
        std::cerr << "Failed to initialize Systems" << std::endl;
        return false;
    }

    m_isRunning = true;
    m_lastFrameTime = std::chrono::high_resolution_clock::now();

    std::cout << "Engine is running" << std::endl;
    return true;
}

int Engine::Run()
{
    if (!m_isRunning)
    {
        std::cerr << "Engine is not initialized" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(m_window))
    {
        UpdateTiming();

        glfwPollEvents();

        Update();

        Render();

        glfwSwapBuffers(m_window);
    }
}

void Engine::Shutdown()
{
    if (!m_isRunning)
        return;

    std::cout << "Shutting down Engine " << std::endl;
    m_isRunning = false;

    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
    std::cout << "Engine shutdown complete" << std::endl;
}

