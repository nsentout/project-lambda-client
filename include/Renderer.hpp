#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL.h>

#include "Position.hpp"

#include <iostream>
#include <vector>

class Renderer
{
private:
    static Renderer *m_instance;

    /**
     * Current player index in the vector of drawn rectangles   //TODO: not used atm
     */
    int m_current_player_index;

    SDL_Renderer *m_renderer;
    SDL_Window *m_window;
    std::vector<SDL_Rect> m_rectangles;

    Renderer();
    void drawPlayer(int player_num, int x, int y);
    void drawCurrentPlayer(int x, int y);
    void drawRect(int rect_index, int new_x, int new_y, SDL_Color *color);
    void clearAllRects();

public:
    ~Renderer();
    static Renderer *getInstance();

    void init();
    void clearPlayer(int player_index);
    void drawPlayers(Position *positions, int length, int current_player_index);
};

#endif