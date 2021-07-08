#ifndef RENDERER_HPP
#define RENDERER_HPP

//#include <SDL2/SDL.h>
#include <SDL.h>

#include "Position.hpp"

#include <iostream>
#include <vector>

class Renderer
{
private:
    static Renderer *m_instance;

    SDL_Renderer *m_renderer;
    SDL_Window *m_window;
    std::vector<SDL_Rect> m_rectangles;

    Renderer();

public:
    ~Renderer();
    static Renderer *getInstance();

    void init();
    void drawPlayers(Position *positions, int length);
    void drawPlayer(int player_num, int x, int y);
    void updateRect(int rect_index, int new_x, int new_y, SDL_Color *color);
};

#endif