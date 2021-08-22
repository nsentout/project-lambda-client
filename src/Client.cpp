#include <stdio.h>
#include <string.h>

#include "Client.hpp"
#include "render/Renderer.hpp"

#include <sstream>

//#define CONNECTION_TIMEOUT 5000
#define CONNECTION_TIMEOUT 100000
#define NUMBER_CHANNELS 2
#define SPEED 25

Client::Client() : m_host(nullptr), m_server(nullptr)
{
    createClient();
}

Client::~Client()
{
    this->disconnect();
    delete m_server_address;
}

void printPacketDescription(const lambda::GameState *gamestate)
{
    printf("\tPacket description:\n");
    printf("\tNb players: %d\n", gamestate->nb_players());
    for (int i = 0; i < gamestate->nb_players(); i++)
    {
        printf("\tPlayer %d: (%d,%d)\n", i + 1, gamestate->players_data(i).x(), gamestate->players_data(i).y());
    }
}

int Client::createClient()
{
    /* Creates a client */
    m_host = enet_host_create(NULL /* create a client host */,
                              1 /* only allow 1 outgoing connection */,
                              NUMBER_CHANNELS /* allow up 2 channels to be used, 0 and 1 */,
                              0 /* assume any amount of incoming bandwidth */,
                              0 /* assume any amount of outgoing bandwidth */);
    if (m_host == NULL)
    {
        fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
        return -1;
    }

    return 1;
}

int Client::connectToServer(const char* server_ip, int server_port)
{
    // Set server address
    m_server_address = new ENetAddress();
    enet_address_set_host(m_server_address, server_ip);
    m_server_address->port = server_port;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    m_server = enet_host_connect(m_host, m_server_address, NUMBER_CHANNELS, 0);
    if (m_server == NULL)
    {
        fprintf(stderr, "No available peers for initiating an ENet connection.\n");
        return -1;
    }

    /* Wait up to 5 seconds for the connection attempt to succeed. */
    ENetEvent net_event;
    if (enet_host_service(m_host, &net_event, CONNECTION_TIMEOUT) > 0 &&
        net_event.type == ENET_EVENT_TYPE_CONNECT)
    {
        printf("Attempt to connecting to %s:%d succeeded. Waiting for server response ...\n", server_ip, server_port);
        /* Store any relevant server information here. */
        net_event.peer->data = (void *)"SERVER";
    }
    else
    {
        /* Either the 5 seconds are up or a disconnect event was */
        /* received. Reset the peer in the event the 5 seconds   */
        /* had run out without any significant event.            */
        enet_peer_reset(m_server);
        puts("Connection to the server failed.");
        return -1;
    }

    enet_host_flush(m_host); // Why does this have to be here for the server to receive the connection attempt ?

    //int response = enet_host_service(m_host, &event, 1000);
    int server_response = enet_host_service(m_host, &net_event, CONNECTION_TIMEOUT);

    // Server sent game state and the player's positions
    if (server_response > 0)
    {
        printf("Connection to %s:%d accepted.\n", server_ip, server_port);

        // Update the player's position
        lambda::GameState received_gamestate = getGamestateFromPacket(net_event.packet);
        int player_index = received_gamestate.nb_players() - 1;
        auto player_data = received_gamestate.players_data(player_index);
        m_id = player_data.id();
        m_x = player_data.x();
        m_y = player_data.y();

        handlePacketReceipt(&net_event);
    }
    else
    {
        puts("Server did not respond... Good bye");

        return -1;
    }

    return 1;
}

/**
 * Checks if a packet is in the waiting queue.
 */
void Client::checkPacketBox()
{
    ENetEvent net_event;
    while (enet_host_service(m_host, &net_event, 0) > 0)
    {
        switch (net_event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
        {
            printf("Someone connected (why ?)\n");
            break;
        }

        // The "peer" field contains the peer the packet was received from, "channelID" is the channel on
        // which the packet was sent, and "packet" is the packet that was sent.
        case ENET_EVENT_TYPE_RECEIVE:
        {
            handlePacketReceipt(&net_event);

            /* Clean up the packet now that we're done using it. */
            enet_packet_destroy(net_event.packet);
        }
        }
    }
}

void Client::handlePacketReceipt(ENetEvent *net_event)
{
    printf("A packet of length %lu was received from %d on channel %u.\n",
           net_event->packet->dataLength,
           net_event->peer->address.host,
           net_event->channelID);

    lambda::GameState received_gamestate = getGamestateFromPacket(net_event->packet);
    printPacketDescription(&received_gamestate);

    checkPacketFormat(&received_gamestate);

    updateRenderData(&received_gamestate);
}

void Client::updateRenderData(lambda::GameState *gamestate) const
{
    if (gamestate->has_player_disconnected_id()) {
        Renderer::getInstance()->clearPlayer(gamestate->player_disconnected_id() - 1);  // minus 1 because the server sends the index + 1
    }

    // Retrieve players positions and send them to the renderer
    lambda::PlayersData player_data;
    int nb_players = gamestate->nb_players();
    int player_index = -1;

    Position positions[nb_players];
    for (int i = 0; i < nb_players; i++)
    {
        if (m_id == gamestate->players_data(i).id())
            player_index = i;

        player_data = gamestate->players_data(i);
        positions[i] = {player_data.x(), player_data.y()};
    }
    Renderer::getInstance()->updateRenderData(positions, nb_players, player_index);
}

void Client::disconnect()
{
    ENetEvent net_event;

    enet_peer_disconnect(m_server, 0);

    /* Allow up to 3 seconds for the disconnect to succeed
    * and drop any received packets.
    */
    while (enet_host_service(m_host, &net_event, 3000) > 0)
    {
        switch (net_event.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            enet_packet_destroy(net_event.packet);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            puts("Disconnection succeeded.");
            return;
        }
    }
    /* We've arrived here, so the disconnect attempt didn't */
    /* succeed yet. Force the connection down.             */
    enet_peer_reset(m_server);

    enet_host_destroy(m_host);
}

void Client::moveUp()
{
    m_y -= SPEED;
    sendPositionToServer(m_x, m_y);
}

void Client::moveDown()
{
    m_y += SPEED;
    sendPositionToServer(m_x, m_y);
}

void Client::moveRight()
{
    m_x += SPEED;
    sendPositionToServer(m_x, m_y);
}

void Client::moveLeft()
{
    m_x -= SPEED;
    sendPositionToServer(m_x, m_y);
}

void Client::sendPositionToServer(int x, int y) const
{
    lambda::PlayerAction playerAction;
    playerAction.set_new_x(x);
    playerAction.set_new_y(y);

    std::string packet_data = getStringFromPlayerAction(&playerAction);
    ENetPacket *packet = enet_packet_create(packet_data.data(), packet_data.size(), ENET_PACKET_FLAG_RELIABLE);

    // Send the packet to the peer over channel id 0.
    enet_peer_send(m_server, 0, packet);
    printf("Sending packet '(%d, %d)' to server.\n", playerAction.new_x(), playerAction.new_y());
}

/**
 * Disconnect the client and close the window if the packet received is malformed
 */
void Client::checkPacketFormat(lambda::GameState *gamestate)
{
    if (gamestate->nb_players() < 1)
    {
        printf("Received malformed packet.\n");
        disconnect();
        exit(-1);
    }
}

lambda::GameState Client::getGamestateFromPacket(ENetPacket *packet) const
{
    lambda::GameState gamestate;
    gamestate.ParseFromArray(packet->data, packet->dataLength);
    return gamestate;
}

const std::string Client::getStringFromPlayerAction(lambda::PlayerAction *playeraction) const
{
    std::string serialized_playeraction;
    playeraction->SerializeToString(&serialized_playeraction);
    return serialized_playeraction;
}
