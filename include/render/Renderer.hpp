#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Position.h"
#include "Client.hpp"
#include "Triangle.hpp"

#include <iostream>
#include <vector>

class Renderer
{
private:
    static Renderer *m_instance;

    GLFWwindow *m_window;
    std::vector<Triangle*> m_triangles;

    Renderer();
    void drawPlayer(int player_num, int x, int y);
    void drawCurrentPlayer(int x, int y);
    void drawRect(int rect_index, int new_x, int new_y/*, SDL_Color *color*/);
    void clearAllRects();

public:
    ~Renderer();
    static Renderer *getInstance();

    int init();
    void processInputs(Client *player);
    void clearPlayer(int player_index);
    void drawPlayers(Position *positions, int length, int current_player_index);
    void drawScene();

    GLFWwindow* getWindow() const;
};

#endif