#include <stdio.h>
#include <stdlib.h>

#include <chrono>

#include "../proto/movement.pb.h"
#include "Client.hpp"

#define POLL_INTERVAL 0.1

int main(int argc, char **argv)
{
    Client client;
    int response = client.connectToServer(1234);
    if (response < 0) {
        return EXIT_FAILURE;
    }

    lambda::Movement m;
    m.set_direction(lambda::Movement_Direction_DOWN_LEFT);
    client.sendDirectionToServer(&m);

    ENetEvent event;
    auto start = std::chrono::system_clock::now();
    while (true)
    {
        std::chrono::duration<double> elapsed_seconds = std::chrono::system_clock::now() - start;
        if (elapsed_seconds.count() > POLL_INTERVAL)
        {
            start = std::chrono::system_clock::now();

            if (enet_host_service(client.getHost(), &event, 0) > 0)
            {
                switch (event.type)
                {
                // Only the "peer" field of the event structure is valid for this event
                case ENET_EVENT_TYPE_CONNECT:
                {
                    printf("A new client connected from %x:%u.\n",
                           event.peer->address.host,
                           event.peer->address.port);
                    /* Store any relevant client information here. */
                    event.peer->data = "Client dummy";
                    break;
                }

                // The "peer" field contains the peer the packet was received from, "channelID" is the channel on
                // which the packet was sent, and "packet" is the packet that was sent.
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    printf("A packet of length %u containing '%s' was received from %s on channel %u.\n",
                           event.packet->dataLength,
                           event.packet->data,
                           event.peer->data,
                           event.channelID);

                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy(event.packet);

                    break;
                }

                // Only the "peer" field of the event structure is valid for this event and contains the peer that disconnected
                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    printf("%s disconnected.\n", event.peer->data);
                    /* Reset the peer's client information. */
                    event.peer->data = NULL;
                }
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
