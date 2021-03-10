// Copyright (c) 2020 Michael Koesel and respective contributors
// SPDX-License-Identifier: MIT
// See accompanying LICENSE file for detailed information

#include "mapping/opengl/polygon.h"
#include <iostream>
#include <cstddef>

Polygon::Polygon(Vertex* vertices, size_t num_vertices)
{
    vertices_count = num_vertices;

    std::cout << "p1" << std::endl;

    glGenVertexArrays(1, &vao);  
    std::cout << "p1.2" << std::endl;

    glBindVertexArray(vao);

    std::cout << "p2" << std::endl;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);

    std::cout << "p3" << std::endl;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)nullptr);

    std::cout << "p4" << std::endl;

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coord));

    std::cout << "p5" << std::endl;

    glBindVertexArray(0);
}

Polygon::~Polygon()
{
    glDeleteVertexArrays(1, &vao);
}

void Polygon::draw()
{
    glBindVertexArray(vao);

    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices_count);

    glBindVertexArray(0);
}
