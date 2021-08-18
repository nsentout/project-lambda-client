#include "render/Renderer.hpp"
#include "Color.h"
#include "const.h"

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
    for (auto triangle : m_triangles) {
        delete triangle;
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

    Triangle *triangle = new Triangle();
    triangle->init();
    m_triangles.push_back(triangle);

    return 0;
}

void Renderer::drawScene()
{
    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // render the triangle
    for (auto triangle : m_triangles) {
        triangle->draw();
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Renderer::clearPlayer(int player_index)
{
    m_triangles.erase(m_triangles.begin() + player_index);
}

void Renderer::clearAllRects()
{

}

void Renderer::drawPlayers(Position *positions, int nb_players, int current_player_index)
{
    if (positions != nullptr) {
        clearAllRects();
        for (int i = 0; i < nb_players; i++) {
            // Add a new rectangle to the vector if there is a new player
            if (m_triangles.size() == i) {
                Triangle *triangle = new Triangle();
                triangle->init();
                triangle->setXY(positions[i].x, positions[i].y);
                m_triangles.push_back(triangle);
            }
            else
                m_triangles[i]->setXY(positions[i].x, positions[i].y);

            drawPlayer(i, positions[i].x, positions[i].y);
        }

    }
}

void Renderer::drawPlayer(int player_num, int x, int y)
{

}

void Renderer::drawRect(int rect_index, int new_x, int new_y /*, SDL_Color *color*/)
{

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