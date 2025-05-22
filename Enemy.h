#pragma once
#include "Entity.h"

struct Enemy : public Entity {
    bool alive = true;
    Enemy(int x, int y) : Entity(x, y) {}
};