#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Position.h"
#include "Client.hpp"
#include "GameObject.hpp"

#include <iostream>
#include <vector>

class Renderer
{
private:
    static Renderer *m_instance;

    GLFWwindow *m_window;
    std::vector<GameObject*> m_gameobjects;

    Renderer();
    void clearPlayer(int player_index);
    void clearAllPlayers();

public:
    ~Renderer();
    static Renderer *getInstance();

    int init();
    void processInputs(Client *player);
    void updateRenderData(Position *positions, int length, int current_player_index);
    void drawScene();

    GLFWwindow* getWindow() const;
};

#endif