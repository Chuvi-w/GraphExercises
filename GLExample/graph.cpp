// g++ graph.cpp -lGL -lGLEW -lglfw
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <vector>

const std::size_t width = 800;
const std::size_t height = 600;

const auto vss = "#version 330\n"
                 "uniform mat4 u_transform;\n"
                 "layout(location = 0) in vec2 pos;\n"
                 "void main() {\n"
                 "    gl_Position = u_transform * vec4(pos, 0.0, 1.0);\n"
                 "}";

const auto fss = 
R"(
    #version 330
    uniform vec4 u_color;
    out vec4 color;
    void main() 
    {
        color = u_color;
    }
)";

float axis[] = {-1,  0, 1, 0,
                 0, -1, 0, 1};

const float graphFrom = -2.0f;
const float graphTo = 2.0f;

float func(float x) {
    return x / std::sqrt(1.0f + x * x);
}

int main() {
    glfwInit();
    auto window = glfwCreateWindow(width, height, "simple graph", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewInit();

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vss, nullptr);
    glCompileShader(vs);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fss, nullptr);
    glCompileShader(fs);
    unsigned int prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    unsigned int uTransform = glGetUniformLocation(prog, "u_transform");
    unsigned int uColor = glGetUniformLocation(prog, "u_color");

    unsigned int vaoAxis;
    glGenVertexArrays(1, &vaoAxis);
    glBindVertexArray(vaoAxis);
    unsigned int vboAxis;
    glGenBuffers(1, &vboAxis);
    glBindBuffer(GL_ARRAY_BUFFER, vboAxis);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axis), axis, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    std::vector<float> graph(width * 2);
    for (std::size_t i = 0; i < width; i++) {
        float x = graphFrom + (graphTo - graphFrom) * i / (width - 1);
        graph[i*2 + 0] = x;
        graph[i*2 + 1] = func(x);
    }
    unsigned int vaoGraph;
    glGenVertexArrays(1, &vaoGraph);
    glBindVertexArray(vaoGraph);
    unsigned int vboGraph;
    glGenBuffers(1, &vboGraph);
    glBindBuffer(GL_ARRAY_BUFFER, vboGraph);
    glBufferData(GL_ARRAY_BUFFER, graph.size() * sizeof(float), graph.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    auto projection = glm::ortho(-1.0f, 1.0f, -1.0f * height / width, 1.0f * height / width);
    auto model = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f / (graphTo - graphFrom), 2.0f / (graphTo - graphFrom), 1.0f));

    while (!glfwWindowShouldClose(window)) 
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(prog);
        glBindVertexArray(vaoAxis);
        glUniformMatrix4fv(uTransform, 1, GL_FALSE, &projection[0][0]);
        glLineWidth(1.0f);
        glUniform4f(uColor, 0.5f, 0.5f, 0.5f, 1.0f);
        glDrawArrays(GL_LINES, 0, 4);
        glBindVertexArray(vaoGraph);
        glUniformMatrix4fv(uTransform, 1, GL_FALSE, &(projection * model)[0][0]);
        glLineWidth(3.0f);
        glUniform4f(uColor, 0.0f, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINE_STRIP, 0, width);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &vboGraph);
    glDeleteVertexArrays(1, &vaoGraph);
    glDeleteBuffers(1, &vboAxis);
    glDeleteVertexArrays(1, &vaoAxis);
    glDeleteProgram(prog);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}