#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Program {
    private:
        GLuint programId;
    public:
        Program(const std::string vertShaderPath, const std::string fragmentShaderPath);
        void useProgram();
        void setVec3(const std::string uniformName, const glm::vec3 vec3);
        void setMat4x4(const std::string uniformName, const glm::mat4x4 mat4x4);
    private:
        std::string readShader(const std::string shaderPath);
};

Program::Program(const std::string vertShaderPath, const std::string fragmentShaderPath) {
    GLuint vertex_shader, fragment_shader;

    const std::string vertex_shader_str = readShader(vertShaderPath);
    const std::string fragment_shader_str = readShader(fragmentShaderPath);

    const char* vertex_shader_c_str = vertex_shader_str.c_str();
    const char* frag_shader_c_str = fragment_shader_str.c_str();

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_c_str, NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag_shader_c_str, NULL);
    glCompileShader(fragment_shader);

    this->programId = glCreateProgram();

    glAttachShader(this->programId, vertex_shader);
    glAttachShader(this->programId, fragment_shader);

    glLinkProgram(this->programId);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Program::useProgram(){
    glUseProgram(this->programId);
}

void Program::setVec3(const std::string uniformName, const glm::vec3 vec3) {
    const char* uniformNameCStr = uniformName.c_str();
    GLuint uniformLoc = glGetUniformLocation(this->programId, uniformNameCStr);
    glUniform3fv(uniformLoc, 1, glm::value_ptr(vec3));
}

void Program::setMat4x4(const std::string uniformName, const glm::mat4x4 mat4x4) {
    const char* uniformNameCStr = uniformName.c_str();
    GLuint uniformLoc = glGetUniformLocation(this->programId, uniformNameCStr);
    glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mat4x4));
}

std::string Program::readShader(const std::string shaderPath) {
    std::string shaderSource;
    std::ifstream shaderFile;
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        shaderFile.open(shaderPath.c_str());
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        shaderSource   = shaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    return shaderSource;
}
#endif