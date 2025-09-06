#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

struct Shader {
    unsigned int ID;

    Shader() : ID(0) {}

    Shader(const std::string &vertexPath, const std::string &fragmentPath) {
        std::string vCode = loadFile(vertexPath);
        std::string fCode = loadFile(fragmentPath);

        const char* vShaderCode = vCode.c_str();
        const char* fShaderCode = fCode.c_str();

        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);

        int success;
        char infoLog[512];
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::cerr << "Erreur compilation vertex shader: " << infoLog << std::endl;
        }

        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            std::cerr << "Erreur compilation fragment shader: " << infoLog << std::endl;
        }

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);

        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, nullptr, infoLog);
            std::cerr << "Erreur link shader program: " << infoLog << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    std::string loadFile(const std::string &path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Erreur : impossible d'ouvrir " << path << std::endl;
            exit(-1);
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void use() {
        glUseProgram(ID);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};