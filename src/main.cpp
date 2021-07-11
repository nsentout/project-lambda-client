#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <iostream>

#include "Client.hpp"
#include "Renderer.hpp"

#define FPS_CAP 60
#define POLL_INTERVAL_MS (1000.0 / FPS_CAP)
#define DEFAULT_IP "localhost"
#define DEFAULT_PORT 1234

int main(int argc, char **argv)
{
    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }

    const char* server_ip;
    int server_port;
    if (argc > 1)
        server_ip = argv[1];
    else
        server_ip = DEFAULT_IP;

    if (argc > 2)
        server_port = atoi(argv[2]);
    else
        server_port = DEFAULT_PORT;

    Client player;
    int server_response = player.connectToServer(server_ip, server_port);
    if (server_response < 0)
    {
        return EXIT_FAILURE;
    }

    SDL_Event event;
    bool quit = false;
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    int framerate = 0;
    auto timer_first_frame_per_second = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    while (!quit)
    {
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
                }

                if (keyPressed == SDLK_RIGHT)
                {
                    std::cout << "pressed RIGHT" << std::endl;
                    player.moveRight();
                }

                if (keyPressed == SDLK_UP)
                {
                    std::cout << "pressed UP" << std::endl;
                    player.moveUp();
                }

                if (keyPressed == SDLK_DOWN)
                {
                    std::cout << "pressed DOWN" << std::endl;
                    player.moveDown();
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

        usleep(POLL_INTERVAL_MS * 1000);    //TODO ? : soustraitre au temps du sleep le temps passé à traiter le paquet et les inputs ?
        framerate++;

        elapsed_seconds = std::chrono::system_clock::now() - timer_first_frame_per_second;
        if (elapsed_seconds.count() >= 1.0)
        {
            timer_first_frame_per_second = std::chrono::system_clock::now();
            std::cout << "framerate: " << framerate << std::endl;
            framerate = 0;
        }
    }

    enet_deinitialize();

    return EXIT_SUCCESS;
}
