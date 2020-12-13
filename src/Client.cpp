#include <stdio.h>
#include <string.h>

#include "Client.hpp"

#include <iostream>
#include <sstream>

#define HOST_NAME "localhost"
#define CONNECTION_TIMEOUT 5000
#define NUMBER_CHANNELS 2

Client::Client() : me(nullptr), server(nullptr)
{
    createClient();
}

Client::~Client()
{
    this->disconnect();
}

int Client::createClient()
{
    /* Creates a client */
    me = enet_host_create(NULL /* create a client host */,
                              1 /* only allow 1 outgoing connection */,
                              NUMBER_CHANNELS /* allow up 2 channels to be used, 0 and 1 */,
                              0 /* assume any amount of incoming bandwidth */,
                              0 /* assume any amount of outgoing bandwidth */);
    if (me == NULL)
    {
        fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
        return -1;
    }

    return 1;
}

int Client::connectToServer(int server_port)
{
    /* Connects to a server (or host) */
    ENetAddress address;
    ENetEvent event;

    /* Connect to localhost:1234. */
    enet_address_set_host(&address, HOST_NAME);
    address.port = server_port;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    server = enet_host_connect(me, &address, NUMBER_CHANNELS, 0);
    if (server == NULL)
    {
        fprintf(stderr, "No available peers for initiating an ENet connection.\n");
        return -1;
    }
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(me, &event, CONNECTION_TIMEOUT) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        puts("Connection to localhost:1234 succeeded.");
        /* Store any relevant server information here. */
        event.peer->data = "SERVER";
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(server);
        puts("Connection to the server failed.");
        return -1;
    }
    return 1;
}

void Client::sendPacket(const char* packetContent)
{
    /* Create a reliable packet of size 7 containing "packet\0" */
    ENetPacket *packet = enet_packet_create(packetContent, strlen(packetContent) + 1, ENET_PACKET_FLAG_RELIABLE);
    
    /* Send the packet to the peer over channel id 0. */
    enet_peer_send(server, 0, packet);
    printf("Sending packet ('%s') to server.\n", packetContent);
}

void Client::sendDirectionToServer(lambda::Movement *movement)
{
    std::ostringstream serialized_movement;
    movement->SerializeToOstream(&serialized_movement);

    /* Create a reliable packet of size 7 containing "packet\0" */
    //ENetPacket *packet = enet_packet_create(serialized_movement, sizeof(serialized_movement), ENET_PACKET_FLAG_RELIABLE);
    const char* packetData = serialized_movement.str().c_str();
    ENetPacket *packet = enet_packet_create(packetData, sizeof(packetData), ENET_PACKET_FLAG_RELIABLE);

    /* Send the packet to the peer over channel id 0. */
    enet_peer_send(server, 0, packet);
    printf("Sending packet ('%d') to server.\n", movement->direction());
}

/**
 * Checks if a packet is in the waiting queue.
 * If there is, packetReceived will be assigned its value.
 * 
 * @return a integer greater than 0, if a packet is in the waiting queue, -1 otherwise @return
 *
 */
void Client::checkPacketBox(char **packetReceived)
{
    ENetEvent event;
    if (enet_host_service(me, &event, 0) > 0)
    {
        switch (event.type)
        {
        // The "peer" field contains the peer the packet was received from, "channelID" is the channel on
        // which the packet was sent, and "packet" is the packet that was sent.
        case ENET_EVENT_TYPE_RECEIVE:
            printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                   event.packet->dataLength,
                   event.packet->data,
                   event.peer->data,
                   event.channelID);

            *packetReceived = (char*) event.packet->userData;  
            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(event.packet);

            break;

        // Only the "peer" field of the event structure is valid for this event and contains the peer that disconnected
        case ENET_EVENT_TYPE_DISCONNECT:
            printf("%s disconnected.\n", event.peer->data);
            /* Reset the peer's client information. */
            event.peer->data = NULL;
        }
    }
}

void Client::disconnect()
{
    ENetEvent event;

    enet_peer_disconnect(server, 0);

    /* Allow up to 3 seconds for the disconnect to succeed
    * and drop any received packets.
    */
    while (enet_host_service(me, &event, 3000) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy(event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            puts("Disconnection succeeded.");
            return;
        }
    }
    /* We've arrived here, so the disconnect attempt didn't */
    /* succeed yet. Force the connection down.             */
    enet_peer_reset(server);

    enet_host_destroy(me);
}

ENetHost* Client::getHost()
{
    return me;
}
