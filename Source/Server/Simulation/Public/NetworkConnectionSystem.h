#pragma once

#include "NetworkDefs.h"
#include "SimDefs.h"
#include "entt/entity/registry.hpp"

namespace AM
{
namespace Server
{
class Simulation;
class World;
class Network;

/**
 * This system is in charge of processing connect/disconnect events and
 * updating the associated client entities.
 */
class NetworkConnectionSystem
{
public:
    NetworkConnectionSystem(Simulation& inSim, World& inWorld,
                            Network& inNetwork);

    /** Processes the effects of new connections and disconnects on the sim. */
    void processConnectionEvents();

private:
    /**
     * Processes all newly connected clients, adding them to the sim.
     */
    void processConnectEvents();

    /**
     * Processes all newly disconnected clients, removing them from the sim.
     */
    void processDisconnectEvents();

    /**
     * Sends a connection response to the client with the given networkID.
     *
     * @param networkID  The client's network ID to send the connection response
     *                   to.
     * @param newEntity  The entity that was created for this client.
     */
    void sendConnectionResponse(NetworkID networkID, entt::entity newEntity,
                                float spawnX, float spawnY);

    Simulation& sim;
    World& world;
    Network& network;
};

} // End namespace Server
} // End namespace AM
