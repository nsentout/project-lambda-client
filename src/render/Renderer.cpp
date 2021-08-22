#include "Color.h"
#include "render/Triangle.hpp"
#include "render/Rectangle.hpp"
#include "render/Renderer.hpp"
#include "render/ShaderRegistry.hpp"

#include <chrono>

using namespace std;

Renderer *Renderer::m_instance = nullptr;

const Color PLAYER1_COLOR = {255, 0, 0, 255};
const Color PLAYER2_COLOR = {0, 255, 0, 255};
const Color PLAYER3_COLOR = {0, 0, 255, 255};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
    for (auto gameobject : m_gameobjects) {
        delete gameobject;
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

Renderer *Renderer::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new Renderer();
    }
    return m_instance;
}

int Renderer::init()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Project Lambda", NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    // glew: initialize
    // ------------------------------
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "Error when initializing glew: " << glewGetErrorString(err) << std::endl;
        exit(1);
    }

    // Uniforms
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

    Shader *basic_shader = ShaderRegistry::getInstance()->getNoTextureShader();
    basic_shader->use();
    int modelMatrixLocation = basic_shader->getUniformLocation("model");
    int viewMatrixLocation = basic_shader->getUniformLocation("view");
    int projectionMatrixLocation = basic_shader->getUniformLocation("projection");

    basic_shader->setUniformMatrix4fv(modelMatrixLocation, modelMatrix);
    basic_shader->setUniformMatrix4fv(viewMatrixLocation, viewMatrix);
    basic_shader->setUniformMatrix4fv(projectionMatrixLocation, projectionMatrix);

    GameObject *player = new Rectangle();
    player->init();
    m_gameobjects.push_back(player);

    return 0;
}

void Renderer::drawScene()
{
    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // render the triangle
    for (auto gameobject : m_gameobjects) {
        gameobject->draw();
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Renderer::clearPlayer(int player_index)
{
    m_gameobjects[player_index]->erase();
    m_gameobjects.erase(m_gameobjects.begin() + player_index);
}

void Renderer::clearAllPlayers()
{
    for (auto gameobject : m_gameobjects) {
        gameobject->erase();
    }
    m_gameobjects.clear();
}

void Renderer::updateRenderData(Position *positions, int nb_players, int current_player_index)
{
    if (positions != nullptr) {
        for (int i = 0; i < nb_players; i++) {
            // Add a new rectangle to the vector if there is a new player
            if (m_gameobjects.size() == i) {
                GameObject *player = new Rectangle();
                player->init();
                player->setXY(positions[i].x, positions[i].y);
                m_gameobjects.push_back(player);
            }
            else
                m_gameobjects[i]->setXY(positions[i].x, positions[i].y);

            Color player_color;
            switch (i)
            {
                case 0:
                    player_color = PLAYER1_COLOR;
                    break;
                case 1:
                    player_color = PLAYER2_COLOR;
                    break;
                case 2:
                    player_color = PLAYER3_COLOR;
                    break;
            }
            m_gameobjects[i]->setColor(player_color);
        }
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Renderer::processInputs(Client *player)
{
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(m_window, true);
        return;
    }
        
    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        std::cout << "pressed LEFT" << std::endl;
        player->moveLeft();
    }

    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        std::cout << "pressed RIGHT" << std::endl;
        player->moveRight();
    }

    if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        std::cout << "pressed UP" << std::endl;
        player->moveUp();
    }

    if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        std::cout << "pressed DOWN" << std::endl;
        player->moveDown();
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

GLFWwindow *Renderer::getWindow() const
{
    return m_window;
}