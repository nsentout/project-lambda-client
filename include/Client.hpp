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

    int m_player_id;

    int m_x;
    int m_y;

    int createClient();
    void firstDraw(lambda::GameState *gamestate) const;
    void drawFirstGamestateReceived(ENetEvent *net_event);
    lambda::GameState getGamestateFromPacket(ENetEvent *net_event) const;
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

    void sendPacket(const char *packetContent) const;
    void sendPositionToServer(int x, int y);
    void checkPacketBox(/*char *packetReceived*/);

    int getPlayerId() const;
};

#endif
