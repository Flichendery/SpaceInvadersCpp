#pragma once
#include "Entity.h"

class Bullet : public Entity {
public:
    bool active = false;
    int dx = 0;
    Bullet();
    void Shoot(int sx, int sy, int ddx = 0);
    void Update(bool isEnemy = false);
};