#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <enet/enet.h>

#include "../../proto/gamestate.pb.h"
#include "../../proto/playeraction.pb.h"

class Client
{
private:
    ENetHost *m_host;
    ENetPeer *m_server;
    ENetAddress *m_server_address;

    int m_id;
    int m_x;
    int m_y;
    bool m_is_moving;

    int createClient();
    void drawPlayersFromGamestate(lambda::GameState *gamestate) const;
    void checkPacketFormat(lambda::GameState *gamestate);
    void handlePacketReceipt(ENetEvent *net_event);
    lambda::GameState getGamestateFromPacket(ENetPacket *packet) const;
    const std::string getStringFromPlayerAction(lambda::PlayerAction *playeraction) const;

public:
    Client();
    virtual ~Client();

    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();

    int connectToServer(int server_port);
    void disconnect();

    void sendPositionToServer(int x, int y) const;
    void checkPacketBox();
};

#endif
