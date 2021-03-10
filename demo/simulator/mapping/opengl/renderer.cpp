// Copyright (c) 2020 Michael Koesel and respective contributors
// SPDX-License-Identifier: MIT
// See accompanying LICENSE file for detailed information

#include "mapping/opengl/renderer.h"
#include <iostream>
#include <cmath>

namespace
{
void generateCircleSegmentVertices(std::vector<Vertex>& vertices, float fov, float radius, float cx, float cy)
{
    vertices.emplace_back(Vertex(glm::vec2(cx, cy), glm::vec2(0.0f, 0.0f)));

    float halfFov = fov / 2;
    float startAngle = 90 - halfFov;
    float endAngle = 90 + halfFov;

    for (int angle = startAngle; angle <= endAngle; angle++)
    {
        float angle_radians = angle * M_PI / 180.0f;

        float x_val = cos(angle_radians);
        float y_val = sin(angle_radians);

        float x = radius * x_val;
        float y = radius * y_val;

        vertices.emplace_back(Vertex(glm::vec2(cx + x, cy + y), glm::vec2((angle - startAngle) / fov, 1.0f)));
    }
}
}  // namespace

Renderer::Renderer(int grid_size, float fov, float grid_range, float max_range) : grid_size(grid_size)
{
    std::cout << "R1" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    std::cout << "R2" << std::endl;
    _window = glfwCreateWindow(grid_size, grid_size, "GPU Occupancy Grid Map", nullptr, nullptr);
    //window.reset(glfwCreateWindow(grid_size, grid_size, "GPU Occupancy Grid Map", nullptr, nullptr));
    std::cout << "R2.2" << std::endl;
    glfwMakeContextCurrent(_window);
    std::cout << "R3" << std::endl;
    glewExperimental = GL_TRUE;
    std::cout << "R3.2" << std::endl;
    glewInit();
    std::cout << "R4" << std::endl;
    std::vector<Vertex> vertices;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    // center vehicle in the middle
    float range = 2.0f * (max_range / grid_range);
    // generateCircleSegmentVertices(vertices, fov, range, 0.0f, 0.0f);
    generateCircleSegmentVertices(vertices, fov, range, 0.0f, -1.0f);
    std::cout << "R5" << std::endl;
    polygon = std::make_unique<Polygon>(vertices.data(), vertices.size());
    std::cout << "R5.2" << std::endl;
    shader = std::make_unique<Shader>();
    std::cout << "R5.3" << std::endl;
    framebuffer = std::make_shared<Framebuffer>(grid_size, grid_size);
    std::cout << "R6" << std::endl;
}

Renderer::~Renderer()
{
    glfwTerminate();
}

void Renderer::renderToTexture(Texture& polar_texture)
{
    glViewport(0, 0, grid_size, grid_size);
    framebuffer->bind();

    // red=occ, green=free
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_CULL_FACE);

    polar_texture.bind(0);
    polar_texture.generateMipMap();

    shader->use();
    polygon->draw();

    framebuffer->unbind();
}
