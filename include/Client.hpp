#ifndef CLIENT_H
#define CLIENT_H

#include <enet/enet.h>

#include "../proto/movement.pb.h"

class Client
{
private:
    ENetHost *me;
    ENetPeer *server;
    int createClient();

public:
    Client();
    virtual ~Client();

    int connectToServer(int server_port);
    void sendPacket(const char* packetContent);
    void sendDirectionToServer(lambda::Movement *movement);
    void checkPacketBox(char **packetReceived);
    void disconnect();

    ENetHost* getHost();
};

#endif
