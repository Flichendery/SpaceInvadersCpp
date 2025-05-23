#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
    bool alive = true;
    Enemy(int x, int y) : Entity(x, y) {}
};
