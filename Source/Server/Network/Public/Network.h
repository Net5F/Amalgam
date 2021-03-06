#pragma once

#include "NetworkDefs.h"
#include "ServerNetworkDefs.h"
#include "ClientHandler.h"
#include "MessageSorter.h"
#include "ClientInput.h"
#include "readerwriterqueue.h"
#include <memory>
#include <cstddef>
#include <unordered_map>
#include <shared_mutex>
#include <queue>

namespace AM
{
class Acceptor;
class Peer;

namespace Server
{
/**
 * Provides Network functionality in the format that the Game wants.
 */
class Network
{
public:
    static constexpr unsigned int SERVER_PORT = 41499;

    Network();

    /**
     * Sends any queued messages over the network.
     */
    void tick();

    /**
     * Queues a message to be sent the next time sendWaitingMessages is called.
     * @throws std::out_of_range if id is not in the clients map.
     *
     * @param networkID  The client to send the message to.
     * @param message  The message to send.
     * @param messageTick  Optional, for messages that are associated with a
     *                     tick number. Used to update the client's
     *                     latestSentSimTick.
     */
    void send(NetworkID networkID, const BinaryBufferSharedPtr& message,
              Uint32 messageTick = 0);

    /**
     * Deserializes and routes received client messages.
     *
     * When a message with a tick number is received, updates the associated
     * client's tick diff data.
     *
     * @param receiveQueue  A queue with messages to process.
     */
    void processReceivedMessages(std::queue<ClientMessage>& receiveQueue);

    /** Forwards to the inputMessageSorter's startReceive. */
    std::queue<std::unique_ptr<ClientInput>>&
        startReceiveInputMessages(Uint32 tickNum);

    /** Forward to the inputMessageSorter's endReceive. */
    void endReceiveInputMessages();

    /** Initialize the tick timer. */
    void initTimer();

    // Returning non-const refs because they need to be modified. Be careful not
    // to attempt to re-assign the obtained ref (can't re-seat a reference once
    // bound).
    ClientMap& getClientMap();
    std::shared_mutex& getClientMapMutex();

    moodycamel::ReaderWriterQueue<NetworkID>& getConnectEventQueue();
    moodycamel::ReaderWriterQueue<NetworkID>& getDisconnectEventQueue();
    moodycamel::ReaderWriterQueue<NetworkID>& getMessageDropEventQueue();

    /** Used for passing us a pointer to the Game's currentTick. */
    void registerCurrentTickPtr(const std::atomic<Uint32>* inCurrentTickPtr);

    /**
     * Allocates and fills a dynamic buffer with a message header and data
     * payload.
     *
     * The first byte of the buffer will contain the message type as a Uint8.
     * The next 2 bytes will contain the message size as a Uint16.
     * The rest will have the data from the given messageBuffer copied into it.
     */
    static BinaryBufferSharedPtr constructMessage(MessageType type,
                                                  Uint8* messageBuffer,
                                                  std::size_t size);

    /**
     * Returns how much time in seconds is left until the next heartbeat.
     */
    double getTimeTillNextHeartbeat();

    /** Convenience for network-owned objects to get the current tick. */
    Uint32 getCurrentTick();

private:
    /**
     * Logs the network stats such as bytes sent/received per second.
     */
    void logNetworkStatistics();

    /**
     * Handles a received ClientInputs message.
     * @return The tick diff that inputMessageSorter.push() returned.
     */
    Sint64 handleClientInputs(ClientMessage& clientMessage,
                              BinaryBufferPtr& messageBuffer);

    /**
     * Handles a received Heartbeat message.
     * @return The difference between the message's tick and our current tick.
     */
    Sint64 handleHeartbeat(BinaryBufferPtr& messageBuffer);

    /** Maps IDs to their connections. Allows the game to say "send this message
        to this entity" instead of needing to track the connection objects. */
    ClientMap clientMap;

    /** Used to lock access to the clientMap.
        Note: ClientHandler's thread is the only one that obtains exclusive
              access to this mutex, so it doesn't bother obtaining shared
              access. If that changes, it will need to be updated. */
    std::shared_mutex clientMapMutex;

    ClientHandler clientHandler;

    /** Used to inform the sim of client connections. */
    moodycamel::ReaderWriterQueue<NetworkID> connectEventQueue;
    /** Used to inform the sim of client disconnects. */
    moodycamel::ReaderWriterQueue<NetworkID> disconnectEventQueue;
    /** Used to inform the sim of clients that we dropped messages from.
        The sim should re-send these clients their current state since they
        predicted an input that was dropped. */
    moodycamel::ReaderWriterQueue<NetworkID> messageDropEventQueue;

    /** Stores input messages received from clients, sorted by tick number. */
    MessageSorter<std::unique_ptr<ClientInput>> inputMessageSorter;

    /** The number of seconds we'll wait before logging our network
        statistics. */
    static constexpr unsigned int SECONDS_TILL_STATS_DUMP = 5;
    static constexpr unsigned int TICKS_TILL_STATS_DUMP
        = (1 / NETWORK_TICK_TIMESTEP_S) * SECONDS_TILL_STATS_DUMP;

    /** The number of ticks since we last logged our network statistics. */
    unsigned int ticksSinceNetstatsLog;

    /** Pointer to the game's current tick. */
    const std::atomic<Uint32>* currentTickPtr;
};

} // namespace Server
} // namespace AM
