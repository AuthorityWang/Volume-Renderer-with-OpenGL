#include <utils.h>
#include <shader.h>
#include <camera.h>
#include <functionloader.h>
#include <dataloader.h>
#include <CalPoint.h>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>
const int WIDTH = 1920;
const int HEIGHT = 1080;
const float stepsize = 0.001f;
GLint Bufferindex;
GLint TfTexture;
GLint PosTexture;
GLint RawTexture;
unsigned int VAO;

//define a camera
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void keyboard_callback(GLFWwindow* window);

Camera camera(glm::vec3(0.5f, 0.5f, 5.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

GLFWwindow *window;

void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Shader glShader("shader/VShader.vs", "shader/FShader.fs");
    Shader glShaderDeBug("shader/shaderDebug.vs", "shader/shaderDebug.fs");
    glShader.use();
    // projection matrix
    glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 800.0f);
    // model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 0.5f, 0.5f));
    // view matrix
    glm::mat4 view = camera.GetViewMatrix();

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Bufferindex);
    glShaderDeBug.use();
    glShaderDeBug.setMat4("proj", proj);
    glShaderDeBug.setMat4("model", model);
    glShaderDeBug.setMat4("view", view);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);
    glBindVertexArray(0);
    glDisable(GL_CULL_FACE);
    glUseProgram(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glShader.use();
    glShader.setMat4("proj", proj);
    glShader.setMat4("model", model);
    glShader.setMat4("view", view);
    glShader.setFloat("StepSize", 0.01f);
    glShader.setVec2("ScreenSize", vec2(WIDTH, HEIGHT));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, TfTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, PosTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_3D, RawTexture);

    glShader.setInt("TransferFunc", 0);
    glShader.setInt("ExitPoints", 1);
    glShader.setInt("VolumeTex", 2);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);
    glBindVertexArray(0);
    glDisable(GL_CULL_FACE);
    glUseProgram(0);
}

int main() {
    WindowGuard windowGuard(window, WIDTH, HEIGHT, "CS171 project");
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    // init a shader
    // Shader glShader("shader/vertexShader.vs", "shader/fragmentShader.fs");
    Shader glShader("shader/VShader.vs", "shader/FShader.fs");
    Shader glShaderDeBug("shader/shaderDebug.vs", "shader/shaderDebug.fs");

    // first write a mesh without transfer function and rawdata, just a cube
    float vertices[24] = {
        0.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 1.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 1.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 1.0
    };
    int indices[36] = {
        1,5,7,7,3,1,
        0,2,6,6,4,0,
        0,1,3,3,2,0,
        7,5,4,4,6,7,
        2,3,7,7,6,2,
        1,0,4,4,5,1
    };

    unsigned int VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (float*) NULL);
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (float*) NULL);
    glBindVertexArray(0);

    // load transfer function as texture
    unsigned int TfTexture;
    TfLoader loader("data/transferfunction/transfer_function.dat");
    TfTexture = loader.tfTexture;

    unsigned int RawTexture;
    // if not 256x256x256, modify parameter in dataloader
    RawLoader rawloader("../../data/raw/bonsai_256x256x256_uint8.raw", 256, 256, 256, 1);
    RawTexture = rawloader.rawTexture;

    GLuint PosTexture, Bufferindex;
    PointGenerate raypos(WIDTH, HEIGHT);
    PosTexture = raypos.PointTexture;
    Bufferindex = raypos.Buffer;

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glShader.use();
        // projection matrix
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 800.0f);
        // model matrix
        glm::mat4 model = glm::mat4(1.0f);
        // view matrix
        glm::mat4 view = camera.GetViewMatrix();

        glBindFramebuffer(GL_FRAMEBUFFER, Bufferindex);
        glEnable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glShaderDeBug.use();
        glShaderDeBug.setMat4("proj", proj);
        glShaderDeBug.setMat4("model", model);
        glShaderDeBug.setMat4("view", view);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);
        glBindVertexArray(0);
        glDisable(GL_CULL_FACE);
        glUseProgram(0);

        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glShader.use();
        glShader.setMat4("proj", proj);
        glShader.setMat4("model", model);
        glShader.setMat4("view", view);
        glShader.setFloat("StepSize", 0.01f);
        glShader.setVec2("ScreenSize", vec2(WIDTH, HEIGHT));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D, TfTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, PosTexture);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_3D, RawTexture);

        glShader.setInt("TransferFunc", 0);
        glShader.setInt("ExitPoints", 1);
        glShader.setInt("VolumeTex", 2);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);
        glBindVertexArray(0);
        glDisable(GL_CULL_FACE);
        glUseProgram(0);
        keyboard_callback(window);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xposf = static_cast<float>(xpos);
    float yposf = static_cast<float>(ypos);

    if (firstMouse)
    {
        lastX = xposf;
        lastY = yposf;
        firstMouse = false;
    }

    float xoffset = xposf - lastX;
    float yoffset = lastY - yposf;
    // as y axis is from button to top, so it is reversed. 

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void keyboard_callback(GLFWwindow* window)
{
    // process to three axis keyboard input to make a move of camera in three axis
    // wasd and UP LEFT DOWN RIGHT are corresponding, space and c are used to make move in Y axis for camera
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(SPACE);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.ProcessKeyboard(C);
}