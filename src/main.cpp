#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <iostream>

#include "Client.hpp"
#include "Renderer.hpp"

#define FPS_CAP 60
#define POLL_INTERVAL (1000.0 / FPS_CAP) / 1000.0
#define DEFAULT_PORT 1234

int main(int argc, char **argv)
{
    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    Client player;
    int response = player.connectToServer(DEFAULT_PORT);
    if (response < 0)
    {
        return EXIT_FAILURE;
    }

    SDL_Event event;
    bool quit = false;
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    //Renderer::getInstance()->init();

    auto start = std::chrono::system_clock::now();
    while (!quit)
    {
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start;
        if (elapsed_seconds.count() > POLL_INTERVAL)
        {
            start = std::chrono::system_clock::now();
            player.checkPacketBox();

            while (SDL_PollEvent(&event) != 0)
            {
                switch (event.type)
                {
                case SDL_KEYDOWN:
                {
                    SDL_Keycode keyPressed = event.key.keysym.sym;

                    if (keyPressed == SDLK_SPACE && !state[SDLK_SPACE])
                    {
                        std::cout << "pressed space" << std::endl;
                    }

                    if (keyPressed == SDLK_LEFT)
                    {
                        std::cout << "pressed LEFT" << std::endl;
                        player.moveLeft();
                        //movement.set_direction(lambda::GameState_Movement_Direction_LEFT);
                        //client.sendDirectionToServer(&movement);
                    }

                    if (keyPressed == SDLK_RIGHT)
                    {
                        std::cout << "pressed RIGHT" << std::endl;
                        player.moveRight();
                        //movement.set_direction(lambda::GameState_Movement_Direction_RIGHT);
                        //client.sendDirectionToServer(&movement);
                    }

                    if (keyPressed == SDLK_UP)
                    {
                        std::cout << "pressed UP" << std::endl;
                        player.moveUp();
                        //movement.set_direction(lambda::GameState_Movement_Direction_UP);
                        //client.sendDirectionToServer(&movement);
                    }

                    if (keyPressed == SDLK_DOWN)
                    {
                        std::cout << "pressed DOWN" << std::endl;
                        player.moveDown();
                        //movement.set_direction(lambda::GameState_Movement_Direction_DOWN);
                        //client.sendDirectionToServer(&movement);
                    }

                    if (keyPressed == SDLK_ESCAPE)
                    {
                        quit = true;
                    }
                    break;
                }

                case SDL_QUIT:
                {
                    quit = true;
                    break;
                }
                }
            }
        }
    }

    enet_deinitialize();

    return EXIT_SUCCESS;
}
