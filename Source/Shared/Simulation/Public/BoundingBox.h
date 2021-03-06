#pragma once

namespace AM
{
/**
 * This struct represents a set of axis-aligned 3D bounds, forming a box.
 */
class BoundingBox
{
public:
    float minX{0};
    float maxX{0};

    float minY{0};
    float maxY{0};

    float minZ{0};
    float maxZ{0};
};

} // End namespace AM
