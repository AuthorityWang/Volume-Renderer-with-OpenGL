#include <utils.h>
#include <mesh.h>
#include <shader.h>
#include <camera.h>
#include <sstream>
#include <fstream>
#include <string>
const int WIDTH = 1920;
const int HEIGHT = 1080;

//define a camera
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void keyboard_callback(GLFWwindow* window);

Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
// position of the light source
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

GLFWwindow *window;

int main() {
  WindowGuard windowGuard(window, WIDTH, HEIGHT, "CS171 hw1");
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  // I don's register keycallback, instead I call it directly below. (It is a bit inconvenient to register it...)
  glEnable(GL_DEPTH_TEST);

  // init a shader
  Shader glShader("src/vertexShader.glsl", "src/fragmentShader.glsl");
  Shader geoShader("src/vsnormal.glsl", "src/fsnormal.glsl", "src/gsnormal.glsl");
  // init a mesh
  Mesh mesh("assets/bunny.obj");

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // use shader before setting any attribution to shader
    glShader.use();
    
    // projection matrix
    glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
    glShader.setMat4("proj", proj);
    // model matrix
    glm::mat4 model = glm::mat4(1.0f);
    glShader.setMat4("model", model);
    // view matrix
    glm::mat4 view = camera.GetViewMatrix();
    glShader.setMat4("view", view);
    // set light to shader

    glShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
    glShader.setVec3("lightColor",  glm::vec3(0.5f, 0.3f, 0.3f));
    glShader.setVec3("lightPos", camera.Position);
    glShader.setVec3("direction", camera.Front);
    glShader.setVec3("viewPos", camera.Position);
    glShader.setFloat("cutOff", glm::cos(glm::radians(12.5f)));
    glShader.setFloat("outerCutOff", glm::cos(glm::radians(17.5f)));
    glShader.setMat4("proj", proj);

    mesh.draw();

    geoShader.use();
    geoShader.setMat4("proj", proj);
    geoShader.setMat4("model", model);
    geoShader.setMat4("view", view);

    mesh.draw();
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