/*
 * Modern OpenGL closed-room lighting demo.
 *
 * Build on Linux:
 *   g++ modern_closed_room.cpp -o modern_room -lGLEW -lglfw -lGL
 *
 * This is intentionally separate from the existing GLUT fixed-pipeline spider
 * app. It demonstrates a GLSL 3.3 renderer with per-pixel Phong lighting,
 * Blinn-Phong specular, quadratic attenuation, and a two-pass shadow map.
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const unsigned int SCR_W = 1280;
const unsigned int SCR_H = 800;
const unsigned int SHADOW_W = 2048;
const unsigned int SHADOW_H = 2048;

struct Mesh {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLsizei count = 0;
};

std::string readFile(const char* path)
{
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open " << path << "\n";
        std::exit(1);
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

GLuint compileShader(GLenum type, const char* path)
{
    std::string source = readFile(path);
    const char* src = source.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint ok = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::cerr << "Shader compile failed for " << path << ":\n" << log << "\n";
        std::exit(1);
    }
    return shader;
}

GLuint linkProgram(const char* vertPath, const char* fragPath)
{
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertPath);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragPath);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint ok = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        std::cerr << "Program link failed:\n" << log << "\n";
        std::exit(1);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

void setMat4(GLuint program, const char* name, const glm::mat4& m)
{
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, glm::value_ptr(m));
}

void setVec3(GLuint program, const char* name, const glm::vec3& v)
{
    glUniform3fv(glGetUniformLocation(program, name), 1, glm::value_ptr(v));
}

void setFloat(GLuint program, const char* name, float v)
{
    glUniform1f(glGetUniformLocation(program, name), v);
}

void pushVertex(std::vector<float>& data, const glm::vec3& p, const glm::vec3& n)
{
    data.push_back(p.x); data.push_back(p.y); data.push_back(p.z);
    data.push_back(n.x); data.push_back(n.y); data.push_back(n.z);
}

Mesh createMesh(const std::vector<float>& data)
{
    Mesh mesh;
    mesh.count = static_cast<GLsizei>(data.size() / 6);

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return mesh;
}

Mesh createPlane(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, glm::vec3 normal)
{
    std::vector<float> data;
    normal = glm::normalize(normal);

    pushVertex(data, a, normal);
    pushVertex(data, b, normal);
    pushVertex(data, c, normal);
    pushVertex(data, a, normal);
    pushVertex(data, c, normal);
    pushVertex(data, d, normal);

    return createMesh(data);
}

Mesh createCube()
{
    const glm::vec3 p[8] = {
        {-0.5f,-0.5f,-0.5f}, { 0.5f,-0.5f,-0.5f}, { 0.5f, 0.5f,-0.5f}, {-0.5f, 0.5f,-0.5f},
        {-0.5f,-0.5f, 0.5f}, { 0.5f,-0.5f, 0.5f}, { 0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}
    };

    struct Face { int a,b,c,d; glm::vec3 n; };
    const Face faces[6] = {
        {0,3,2,1,{ 0, 0,-1}},
        {4,5,6,7,{ 0, 0, 1}},
        {0,4,7,3,{-1, 0, 0}},
        {1,2,6,5,{ 1, 0, 0}},
        {3,7,6,2,{ 0, 1, 0}},
        {0,1,5,4,{ 0,-1, 0}}
    };

    std::vector<float> data;
    for (const Face& f : faces) {
        pushVertex(data, p[f.a], f.n);
        pushVertex(data, p[f.b], f.n);
        pushVertex(data, p[f.c], f.n);
        pushVertex(data, p[f.a], f.n);
        pushVertex(data, p[f.c], f.n);
        pushVertex(data, p[f.d], f.n);
    }
    return createMesh(data);
}

void drawMesh(GLuint program, const Mesh& mesh, const glm::mat4& model, const glm::vec3& color, float shininess)
{
    setMat4(program, "model", model);
    setVec3(program, "objectColor", color);
    setFloat(program, "shininess", shininess);

    glBindVertexArray(mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, mesh.count);
    glBindVertexArray(0);
}

struct SceneMeshes {
    Mesh floor, ceiling, backWall, frontWall, leftWall, rightWall, cube;
};

SceneMeshes createScene()
{
    const float x = 5.0f;
    const float z = 5.0f;
    const float h = 9.0f;

    SceneMeshes s;
    // Normals point inward toward the center of the room.
    s.floor = createPlane({-x,0,-z}, { x,0,-z}, { x,0, z}, {-x,0, z}, {0, 1, 0});
    s.ceiling = createPlane({-x,h,-z}, {-x,h, z}, { x,h, z}, { x,h,-z}, {0,-1, 0});
    s.backWall = createPlane({-x,0,-z}, {-x,h,-z}, { x,h,-z}, { x,0,-z}, {0, 0, 1});
    s.frontWall = createPlane({-x,0, z}, { x,0, z}, { x,h, z}, {-x,h, z}, {0, 0,-1});
    s.leftWall = createPlane({-x,0,-z}, {-x,0, z}, {-x,h, z}, {-x,h,-z}, {1, 0, 0});
    s.rightWall = createPlane({ x,0,-z}, { x,h,-z}, { x,h, z}, { x,0, z}, {-1, 0, 0});
    s.cube = createCube();
    return s;
}

void drawScene(GLuint program, const SceneMeshes& s)
{
    glm::mat4 I(1.0f);

    drawMesh(program, s.floor, I, {0.72f, 0.60f, 0.42f}, 128.0f);
    drawMesh(program, s.ceiling, I, {0.82f, 0.80f, 0.74f}, 16.0f);
    drawMesh(program, s.backWall, I, {0.76f, 0.70f, 0.62f}, 24.0f);
    drawMesh(program, s.frontWall, I, {0.76f, 0.70f, 0.62f}, 24.0f);
    drawMesh(program, s.leftWall, I, {0.70f, 0.67f, 0.60f}, 24.0f);
    drawMesh(program, s.rightWall, I, {0.70f, 0.67f, 0.60f}, 24.0f);

    glm::mat4 m1 = glm::translate(I, glm::vec3(-1.7f, 0.65f, -1.4f));
    m1 = glm::scale(m1, glm::vec3(1.3f, 1.3f, 1.3f));
    drawMesh(program, s.cube, m1, {0.45f, 0.18f, 0.12f}, 32.0f);

    glm::mat4 m2 = glm::translate(I, glm::vec3(1.8f, 0.45f, 1.2f));
    m2 = glm::scale(m2, glm::vec3(0.9f, 0.9f, 0.9f));
    drawMesh(program, s.cube, m2, {0.15f, 0.26f, 0.42f}, 48.0f);
}

int main()
{
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_W, SCR_H, "Closed Room Lighting - GLSL 3.3", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return 1;
    }

    glViewport(0, 0, SCR_W, SCR_H);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    GLuint mainProgram = linkProgram("shaders/main.vert", "shaders/main.frag");
    GLuint shadowProgram = linkProgram("shaders/shadow.vert", "shaders/shadow.frag");
    SceneMeshes scene = createScene();

    GLuint depthFBO = 0;
    GLuint depthMap = 0;
    glGenFramebuffers(1, &depthFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_W, SHADOW_H, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    const float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    const glm::vec3 lightPos(0.0f, 8.85f, 0.0f);
    // Camera is inside the closed room. If placed outside, the front wall
    // correctly blocks the view and the window looks like an empty surface.
    const glm::vec3 cameraPos(0.0f, 3.6f, 4.35f);

    glm::mat4 lightProjection = glm::perspective(glm::radians(96.0f), 1.0f, 0.1f, 18.0f);
    glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    glUseProgram(mainProgram);
    glUniform1i(glGetUniformLocation(mainProgram, "shadowMap"), 0);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Pass 1: render scene depth from the ceiling light's perspective.
        glViewport(0, 0, SHADOW_W, SHADOW_H);
        glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glUseProgram(shadowProgram);
        setMat4(shadowProgram, "lightSpaceMatrix", lightSpaceMatrix);
        drawScene(shadowProgram, scene);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Pass 2: main camera pass with Phong lighting and shadow lookup.
        glViewport(0, 0, SCR_W, SCR_H);
        glClearColor(0.015f, 0.014f, 0.012f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(55.0f), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, glm::vec3(0.0f, 2.4f, -0.8f), glm::vec3(0.0f, 1.0f, 0.0f));

        glUseProgram(mainProgram);
        setMat4(mainProgram, "projection", projection);
        setMat4(mainProgram, "view", view);
        setMat4(mainProgram, "lightSpaceMatrix", lightSpaceMatrix);
        setVec3(mainProgram, "lightPos", lightPos);
        setVec3(mainProgram, "viewPos", cameraPos);
        setVec3(mainProgram, "lightColor", {1.0f, 0.92f, 0.72f});
        setFloat(mainProgram, "constantAttenuation", 1.0f);
        setFloat(mainProgram, "linearAttenuation", 0.09f);
        setFloat(mainProgram, "quadraticAttenuation", 0.032f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        drawScene(mainProgram, scene);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
