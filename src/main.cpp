#include <utils.h>
#include <shader.h>
#include <camera.h>
#include <functionloader.h>
#include <dataloader.h>
#include <sstream>
#include <fstream>
#include <string>
#include <memory>
const int WIDTH = 1920;
const int HEIGHT = 1080;
const float stepsize = 0.01f;

//define a camera
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void keyboard_callback(GLFWwindow* window);

Camera camera(glm::vec3(0.5f, 0.5f, 5.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;
// position of the light source
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

GLFWwindow *window;

int main() {
    WindowGuard windowGuard(window, WIDTH, HEIGHT, "CS171 project");
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    // init a shader
    Shader glShader("shader/VShader.vs", "shader/FShader.fs");

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

    unsigned int VAO, VBO, EBO;
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
    GLuint TfTexture;
    TfLoader loader("data/transferfunction/transfer_function.dat");
    TfTexture = loader.tfTexture;

    GLuint RawTexture;
    // if not 256x256x256, modify parameter in dataloader
    RawLoader rawloader("data/raw/head_256x256x256_uint8.raw");
    RawTexture = rawloader.rawTexture;

    GLuint BackTexture;
    glGenTextures(1, &BackTexture);
    glBindTexture(GL_TEXTURE_2D, BackTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // use shader before setting any attribution to shader
        glShader.use();
        
        // projection matrix
        glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH/(float)HEIGHT, 0.1f, 800.0f);
        glShader.setMat4("proj", proj);
        // model matrix
        glm::mat4 model = glm::mat4(1.0f);
        glShader.setMat4("model", model);
        // view matrix
        glm::mat4 view = camera.GetViewMatrix();
        glShader.setMat4("view", view);

        // glShader.setMat4("ViewMatrix", view);
        // glShader.setFloat("focal_length", 5.0f);
        // glShader.setFloat("aspect_ratio", (float)WIDTH / (float)HEIGHT);
        // glShader.setVec2("viewport_size", vec2(1.0, 1.0));
        // glShader.setVec3("ray_origin", vec3(0.5, 0.5, 5.0));
        // glShader.setVec3("upp_bnd", vec3(1.0, 1.0, 1.0));
        // glShader.setVec3("low_bnd", vec3(0.0, 0.0, 0.0));
        // glShader.setVec3("background_colour", vec3(1.0f, 1.0f, 1.0f));
        // glShader.setFloat("step_size", 0.01f);
        glShader.setFloat("StepSize", stepsize);
        glShader.setVec2("ScreenSize", vec2(WIDTH, HEIGHT));
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D, TfTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_3D, RawTexture);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, BackTexture);

        glShader.setInt("TfTexture", TfTexture);
        glShader.setInt("RawTexture", RawTexture);
        glShader.setInt("ExitPoints", BackTexture);

        // only draw the front visible faces to faster
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLuint *)NULL);
        glBindVertexArray(0);
        glDisable(GL_CULL_FACE);

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