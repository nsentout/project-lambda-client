#include <stdio.h>
#include <string.h>

#include "Client.hpp"
#include "Renderer.hpp"

#include <sstream>

#define HOST_NAME "localhost"
//#define CONNECTION_TIMEOUT 5000
#define CONNECTION_TIMEOUT 100000
#define NUMBER_CHANNELS 2
#define SPEED 100

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

int Client::connectToServer(int server_port)
{
    /* Connects to a server (or host) */
    m_server_address = new ENetAddress();
    ENetEvent event;

    /* Connect to localhost:1234. */
    enet_address_set_host(m_server_address, "localhost");
    m_server_address->port = server_port;

    /* Initiate the connection, allocating the two channels 0 and 1. */
    m_server = enet_host_connect(m_host, m_server_address, NUMBER_CHANNELS, 0);
    if (m_server == NULL)
    {
        fprintf(stderr, "No available peers for initiating an ENet connection.\n");
        return -1;
    }
    /* Wait up to 5 seconds for the connection attempt to succeed. */
    if (enet_host_service(m_host, &event, CONNECTION_TIMEOUT) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        puts("Attempt to connecting to localhost:1234 succeeded. Waiting for server response ...");
        /* Store any relevant server information here. */
        event.peer->data = (void *)"SERVER";
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
    int response = enet_host_service(m_host, &event, CONNECTION_TIMEOUT);

    // Server sent game state and the player's positions
    if (response > 0)
    {
        drawFirstGamestateReceived(&event);
    }
    else
    {
        puts("Server did not respond... Good bye");

        return -1;
    }

    return 1;
}

void Client::drawFirstGamestateReceived(ENetEvent *net_event)
{
    puts("Connection to localhost:1234 accepted.");

    printf("A packet of length %u was received from %d on channel %u.\n",
           net_event->packet->dataLength,
           net_event->peer->address.host,
           net_event->channelID);

    lambda::GameState gamestate_received = getGamestateFromPacket(net_event);
    printPacketDescription(&gamestate_received);

    if (gamestate_received.nb_players() < 1)
    {
        printf("Received malformed package.\n");
        disconnect();
        exit(-1);
    }

    m_player_id = gamestate_received.nb_players() - 1;

    auto player_data = gamestate_received.players_data(m_player_id);
    m_x = player_data.x();
    m_y = player_data.y();

    Renderer::getInstance()->init();
    firstDraw(&gamestate_received);
}

void Client::firstDraw(lambda::GameState *gamestate) const
{
    int nb_players = gamestate->nb_players();
    Position *positions = new Position[nb_players]();
    for (int i = 0; i < nb_players; i++)
    {
        positions[i] = {gamestate->players_data(i).x(), gamestate->players_data(i).y()};
    }

    Renderer::getInstance()->drawPlayers(positions, nb_players);
    delete positions;
}

lambda::GameState Client::getGamestateFromPacket(ENetEvent *net_event) const
{
    lambda::GameState gamestate;
    std::istringstream unserialized_gamestate(reinterpret_cast<char const *>(net_event->packet->data));
    gamestate.ParseFromIstream(&unserialized_gamestate);
    return gamestate;
}

const std::string Client::getStringFromPlayerAction(lambda::PlayerAction *playeraction) const
{
    std::string serialized_playeraction;
    playeraction->SerializeToString(&serialized_playeraction);
    return serialized_playeraction;
}


void Client::sendPositionToServer(int x, int y)
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
            printf("Someone connected\n");
            break;
        }

        // The "peer" field contains the peer the packet was received from, "channelID" is the channel on
        // which the packet was sent, and "packet" is the packet that was sent.
        case ENET_EVENT_TYPE_RECEIVE:
        {
            printf("A packet of length %u was received from %d on channel %u.\n",
                   net_event.packet->dataLength,
                   net_event.peer->address.host,
                   net_event.channelID);

            lambda::GameState gamestate = getGamestateFromPacket(&net_event);

            puts("Client received following packet :\n");
            printPacketDescription(&gamestate);

            if (gamestate.nb_players() < 1)
            {
                printf("Received malformed package.\n");
                disconnect();
                exit(-1);
            }

            // Retrieve players positions and send them to the renderer
            lambda::PlayersData player_data;
            int nb_players = gamestate.nb_players();
            //Position positions[nb_players];
            Position *positions = new Position[nb_players]();
            for (int i = 0; i < nb_players; i++)
            {
                player_data = gamestate.players_data(i);
                positions[i] = {player_data.x(), player_data.y()};
            }
            Renderer::getInstance()->drawPlayers(positions, nb_players);

            /* Clean up the packet now that we're done using it. */
            //enet_packet_destroy(net_event.packet);
            delete positions;
        }
        }
    }
}

void Client::disconnect()
{
    ENetEvent event;

    enet_peer_disconnect(m_server, 0);

    /* Allow up to 3 seconds for the disconnect to succeed
    * and drop any received packets.
    */
    while (enet_host_service(m_host, &event, 3000) > 0)
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

int Client::getPlayerId() const
{
    return m_player_id;
}
