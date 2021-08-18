#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <iostream>

#include "Client.hpp"
#include "render/Renderer.hpp"

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

    Renderer *renderer = Renderer::getInstance();

    int framerate = 0;
    auto timer_first_frame_per_second = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds;

    while (!glfwWindowShouldClose(renderer->getWindow()))
    {
        player.checkPacketBox();

        // input
        // -----
        renderer->processInputs(&player);

        // render
        // ------
        renderer->drawScene();

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

    delete renderer;

    return EXIT_SUCCESS;
}
