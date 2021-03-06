#pragma once

#include "SimDefs.h"
#include <vector>

namespace AM
{
namespace Server
{
class IDPool
{
public:
    IDPool(unsigned int inPoolSize);

    /**
     * Reserves and returns the next empty ID.
     *
     * Marches forward, e.g. if 0-10 were reserved and freed, 11 will be the
     * next reserved ID. This, along with SAFETY_BUFFER, aims to remove
     * situations where an ID was reserved, freed, and re-reserved while old
     * data exists in the system.
     */
    unsigned int reserveID();

    /**
     * Frees an ID for reuse.
     */
    void freeID(unsigned int ID);

private:
    /** Extra room so that we don't run into reuse issues when almost all IDs
        are reserved. */
    static constexpr unsigned int SAFETY_BUFFER = 100;

    /** The maximum number of IDs that we can give out. */
    unsigned int poolSize;

    /** The size of our container. Equal to poolSize + SAFETY_BUFFER. */
    unsigned int containerSize;

    /** The last index that we added an ID to. */
    unsigned int lastAddedIndex;

    /** The number of currently reserved IDs. */
    unsigned int reservedIDCount;

    /**
     * If ID 'x' is available, IDs[x] will be true. Else, it will be false.
     */
    std::vector<bool> IDs;
};

} // namespace Server
} // namespace AM
