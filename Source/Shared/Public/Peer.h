#ifndef PEER_H_
#define PEER_H_

#include "SharedDefs.h"
#include <SDL2/SDL_net.h>
#include <memory>
#include <array>

namespace AM
{

/**
 * Represents a network peer for communication.
 */
class Peer
{
public:
    /** Largest message we'll accept. Kept at 1450 for now to try to avoid IP fragmentation.
     *  Can rethink if we need larger. */
    static constexpr unsigned int MAX_MESSAGE_SIZE = 1450;

    /**
     * Initiates a TCP connection that the other side can then accept.
     * (e.g. the client connecting to the server)
     */
    static std::unique_ptr<Peer> initiate(std::string serverIP, unsigned int serverPort);

    /**
     * Constructor for when you only need 1 peer (client connecting to server, anyone
     * connecting to chat server.)
     * Constructs a socket set for this peer to use.
     */
    Peer(TCPsocket inSocket);

    /**
     * Constructor for when you need a set of peers (server connecting to clients).
     * Adds the socket to the given set.
     */
    Peer(TCPsocket inSocket, std::shared_ptr<SDLNet_SocketSet> inSet);

    ~Peer();

    bool isConnected();

    /**
     * Sends a message to this Peer.
     * Will error if the message size is larger than a Uint16 can hold.
     * @return false if the send failed, else true.
     */
    bool sendMessage(BinaryBufferSharedPtr message);

    /**
     * @param checkSockets  If true, will call CheckSockets() before checking
     *                      SocketReady(). Set this to false if you're going to call
     *                      CheckSockets() yourself.
     * @return A message if one is ready, else nullptr.
     */
    BinaryBufferSharedPtr receiveMessage(bool checkSockets);

    /**
     * Waits for a message to be ready.
     * @return A message.
     */
    BinaryBufferSharedPtr receiveMessageWait();

private:
    std::shared_ptr<SDLNet_SocketSet> set;
    TCPsocket socket;

    bool peerIsConnected;

    std::array<Uint8, MAX_MESSAGE_SIZE> messageBuffer;
};

} /* End namespace AM */

#endif /* End PEER_H_ */
