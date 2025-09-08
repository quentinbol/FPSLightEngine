#pragma once
#include <glad/glad.h>
#include <stdlib.h>
#include <vector>

struct Mesh {
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
    size_t indexCount;
    glm::vec3 minBounds;
    glm::vec3 maxBounds;

    Mesh() : VAO(0), VBO(0), EBO(0), indexCount(0), minBounds(0), maxBounds(0) {}
    Mesh(unsigned int vao, unsigned int vbo, unsigned int ebo, size_t count,
         const glm::vec3& minB, const glm::vec3& maxB)
        : VAO(vao), VBO(vbo), EBO(ebo), indexCount(count), minBounds(minB), maxBounds(maxB) {}

    void draw() const {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    glm::vec3 getHalfExtents() const {
        return (maxBounds - minBounds) * 0.5f;
    }

   static Mesh makeCube() {
    float vertices[] = {
        -0.5f, -0.5f,  0.5f,  0, 0, 1,  0, 0,
         0.5f, -0.5f,  0.5f,  0, 0, 1,  1, 0,
         0.5f,  0.5f,  0.5f,  0, 0, 1,  1, 1,
        -0.5f,  0.5f,  0.5f,  0, 0, 1,  0, 1,

        -0.5f, -0.5f, -0.5f,  0, 0, -1, 0, 0,
         0.5f, -0.5f, -0.5f,  0, 0, -1, 1, 0,
         0.5f,  0.5f, -0.5f,  0, 0, -1, 1, 1,
        -0.5f,  0.5f, -0.5f,  0, 0, -1, 0, 1,

        -0.5f, -0.5f, -0.5f, -1, 0, 0,  0, 0,
        -0.5f,  0.5f, -0.5f, -1, 0, 0,  1, 0,
        -0.5f,  0.5f,  0.5f, -1, 0, 0,  1, 1,
        -0.5f, -0.5f,  0.5f, -1, 0, 0,  0, 1,

         0.5f, -0.5f, -0.5f,  1, 0, 0,  0, 0,
         0.5f,  0.5f, -0.5f,  1, 0, 0,  1, 0,
         0.5f,  0.5f,  0.5f,  1, 0, 0,  1, 1,
         0.5f, -0.5f,  0.5f,  1, 0, 0,  0, 1,

        -0.5f, -0.5f, -0.5f,  0, -1, 0, 0, 0,
         0.5f, -0.5f, -0.5f,  0, -1, 0, 1, 0,
         0.5f, -0.5f,  0.5f,  0, -1, 0, 1, 1,
        -0.5f, -0.5f,  0.5f,  0, -1, 0, 0, 1,

        -0.5f,  0.5f, -0.5f,  0, 1, 0, 0, 0,
         0.5f,  0.5f, -0.5f,  0, 1, 0, 1, 0,
         0.5f,  0.5f,  0.5f,  0, 1, 0, 1, 1,
        -0.5f,  0.5f,  0.5f,  0, 1, 0, 0, 1
    };

    unsigned int indices[] = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        8,9,10, 10,11,8,
        12,13,14, 14,15,12,
        16,17,18, 18,19,16, 
        20,21,22, 22,23,20
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    return Mesh(VAO, VBO, EBO, sizeof(indices) / sizeof(unsigned int),
                glm::vec3(-0.5f,-0.5f,-0.5f),
                glm::vec3(0.5f,0.5f,0.5f));
}


    static Mesh makePlane(int cols, int rows) {
        std::vector<float> vertices;
        std::vector<unsigned int> indices;

        for (int z = 0; z <= rows; z++) {
            for (int x = 0; x <= cols; x++) {
                float xpos = (float)x - cols / 2.0f;
                float zpos = (float)z - rows / 2.0f;
                float ypos = 0.0f;

                vertices.push_back(xpos);
                vertices.push_back(ypos);
                vertices.push_back(zpos);

                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
                vertices.push_back(0.0f);

                vertices.push_back((float)x / cols);
                vertices.push_back((float)z / rows);
            }
        }

        for (int z = 0; z < rows; z++) {
            for (int x = 0; x < cols; x++) {
                int start = z * (cols + 1) + x;

                indices.push_back(start);
                indices.push_back(start + 1);
                indices.push_back(start + cols + 1);

                indices.push_back(start + 1);
                indices.push_back(start + cols + 2);
                indices.push_back(start + cols + 1);
            }
        }

        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        return Mesh(VAO, VBO, EBO, indices.size(),
            glm::vec3(-cols / 2.0f, 0.0f, -rows / 2.0f),
            glm::vec3(cols / 2.0f, 0.0f, rows / 2.0f));
    }

    static Mesh makePyramid() {
        float vertices[] = {
        
            -0.5f, 0.0f, -0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
            0.5f, 0.0f, -0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
            0.5f, 0.0f,  0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
            -0.5f, 0.0f,  0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,

            -0.5f, 0.0f, -0.5f,   0.0f, 0.707f, -0.707f,  0.0f, 0.0f,
            0.5f, 0.0f, -0.5f,   0.0f, 0.707f, -0.707f,  1.0f, 0.0f,
            0.0f, 1.0f,  0.0f,   0.0f, 0.707f, -0.707f,  0.5f, 1.0f,

            0.5f, 0.0f, -0.5f,   0.707f, 0.707f, 0.0f,  0.0f, 0.0f,
            0.5f, 0.0f,  0.5f,   0.707f, 0.707f, 0.0f,  1.0f, 0.0f,
            0.0f, 1.0f,  0.0f,   0.707f, 0.707f, 0.0f,  0.5f, 1.0f,

            0.5f, 0.0f,  0.5f,   0.0f, 0.707f, 0.707f,  0.0f, 0.0f,
            -0.5f, 0.0f,  0.5f,   0.0f, 0.707f, 0.707f,  1.0f, 0.0f,
            0.0f, 1.0f,  0.0f,   0.0f, 0.707f, 0.707f,  0.5f, 1.0f,

            -0.5f, 0.0f,  0.5f,  -0.707f, 0.707f, 0.0f,  0.0f, 0.0f,
            -0.5f, 0.0f, -0.5f,  -0.707f, 0.707f, 0.0f,  1.0f, 0.0f,
            0.0f, 1.0f,  0.0f,  -0.707f, 0.707f, 0.0f,  0.5f, 1.0f
        };


        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6,   
            7, 8, 9,
            10,11,12,
            13,14,15
        };


        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

        return Mesh(VAO, VBO, EBO, sizeof(indices)/sizeof(unsigned int),
                    glm::vec3(-0.5f, 0.0f, -0.5f),
                    glm::vec3(0.5f, 1.0f, 0.5f));
    }
};