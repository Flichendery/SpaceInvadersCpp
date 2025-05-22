#pragma once
#include "Entity.h"

struct Barrier : public Entity {
    int hp = 3;
    Barrier(int x, int y) : Entity(x, y) {}
};