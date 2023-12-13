#include "Obstaculo.h"

bool Obstacle::overlaps(const Obstacle &other) const {
        return !(topLeft.x > other.bottomRight.x ||
        bottomRight.x < other.topLeft.x ||
        topLeft.y > other.bottomRight.y ||
        bottomRight.y < other.topLeft.y);
}
