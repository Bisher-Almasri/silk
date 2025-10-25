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

bool Engine::Initialize(const EngineConfig& config)
{
    std::cout << "Initializing Voxel Game Engine..." << std::endl;

    m_config = std::make_unique<EngineConfig>(config);

    if (!InitializeWindow())
    {
        std::cerr << "Failed to initialize window" << std::endl;
        return false;
    }

    if (!InitializeGraphics())
    {
        std::cerr << "Failed to initialize graphics" << std::endl;
        return false;
    }

    if (!InitializeSystems())
    {
        std::cerr << "Failed to initialize engine systems" << std::endl;
        return false;
    }

    m_isRunning = true;
    m_lastFrameTime = std::chrono::high_resolution_clock::now();

    std::cout << "Engine initialized successfully" << std::endl;
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

    std::cout << "Engine loop ended" << std::endl;
    return 0;
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

bool Engine::InitializeWindow()
{
    glfwSetErrorCallback(OnGLFWError);

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_window = glfwCreateWindow(
        m_config->GetWindowWidth(),
        m_config->GetWindowHeight(),
        m_config->GetWindowTitle().c_str(),
        nullptr,
        nullptr
        );

    if (!m_window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, OnWindowResize);

    glfwSwapInterval(m_config->IsVSyncEnabled() ? 1 : 0);

    return true;
}

bool Engine::InitializeGraphics()
{
    // Initialize GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;

    return true;
}

void Engine::UpdateTiming()
{
    const auto currentTime = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - m_lastFrameTime);
    m_deltaTime = duration.count() / 1000000.0f; // Convert to seconds
    m_lastFrameTime = currentTime;
    m_frameTimeSamples[m_frameTimeSampleIndex] = m_deltaTime;
    m_frameTimeSampleIndex = (m_frameTimeSampleIndex + 1) % FRAME_RATE_SAMPLE_COUNT;

    float averageFrameTime = 0.0f;
    for (const float m_frameTimeSample : m_frameTimeSamples)
    {
        averageFrameTime += m_frameTimeSample;
    }
    averageFrameTime /= FRAME_RATE_SAMPLE_COUNT;

    m_frameRate = averageFrameTime > 0.0f ? 1.0f / averageFrameTime : 0.0f;
}

void Engine::Update()
{
    // Todo
}

void Engine::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Engine::InitializeSystems()
{
    
    std::cout << "Initializing engine systems..." << std::endl;

    std::cout << "Engine systems initialized successfully" << std::endl;
    return true;
}


void Engine::OnWindowResize(GLFWwindow* window, const int width, const int height)
{
    if (const auto* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window)); engine && engine->m_config)
    {
        engine->m_config->SetWindowSize(width, height);
        glViewport(0, 0, width, height);
    }
}

void Engine::OnGLFWError(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}