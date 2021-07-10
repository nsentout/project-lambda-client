#include "Renderer.hpp"

#include <chrono>

using namespace std;

Renderer *Renderer::m_instance = nullptr;
const SDL_Color PLAYER1_COLOR = { 255, 0, 0, 255};
const SDL_Color PLAYER2_COLOR = { 0, 255, 0, 255};
const SDL_Color PLAYER3_COLOR = { 0, 0, 255, 255};

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
}

Renderer *Renderer::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new Renderer();
    }
    return m_instance;
}

void Renderer::init()
{
    // Create window
    m_window = SDL_CreateWindow("Project lambda", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    m_renderer = nullptr;

    if (m_window != nullptr)
    {
        // Set the scaling interpolation algorithm for the renderer (1 = linear interpolation)
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

        // Create renderer for the window
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (m_renderer == nullptr)
        {
            cerr << "Renderer could not be created! SDL Error:" << SDL_GetError() << endl;
            SDL_DestroyWindow(m_window);
        }
    }
}

void Renderer::drawPlayers(Position *positions, int nb_players, int current_player_index, bool current_player_is_moving) 
{
    if (positions != nullptr) {
        m_current_player_index = current_player_index;

        if (current_player_is_moving) {
            drawCurrentPlayer(positions[current_player_index].getX(), positions[current_player_index].getY());
        }

        // Draw other players
        for (int i = 0; i < nb_players; i++) {
            if (i != current_player_index)
                drawPlayer(i, positions[i].getX(), positions[i].getY());
        }

        if (!current_player_is_moving) {
            drawCurrentPlayer(positions[current_player_index].getX(), positions[current_player_index].getY());
        }
    }
}

void Renderer::drawCurrentPlayer(int x, int y)
{
    drawPlayer(m_current_player_index, x, y);
}

void Renderer::drawPlayer(int player_num, int x, int y)
{
    SDL_Color player_color;
    switch (player_num)
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
        default:
            player_color = { 255, 255, 255, 255};
            break;
    }

    drawRect(player_num, x, y, &player_color);
}

void Renderer::drawRect(int rect_index, int new_x, int new_y, SDL_Color *color)
{
    // Clean the old position of the rect
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(m_renderer, &m_rectangles[rect_index]);

    // Add a new rectangle to the vector if there is a new player
    if (m_rectangles.size() == rect_index)
        m_rectangles.push_back({ new_x, new_y, 100, 100 });
    else
        m_rectangles[rect_index] = { new_x, new_y, 100, 100 };

    // Draw the new position of the rect
    SDL_SetRenderDrawColor(m_renderer, color->r, color->g, color->b, color->a);
    SDL_RenderFillRect(m_renderer, &m_rectangles[rect_index]);

    SDL_RenderPresent(m_renderer);
}
