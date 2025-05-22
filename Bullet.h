#pragma once
#include "Entity.h"

struct Bullet : public Entity {
    bool active = false;
    int dx = 0;
    Bullet();
    void Shoot(int sx, int sy, int ddx = 0);
    void Update(bool isEnemy = false);
};