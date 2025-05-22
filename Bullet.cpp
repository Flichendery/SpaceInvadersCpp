#include "Bullet.h"

Bullet::Bullet() : Entity(-1, -1) {}

void Bullet::Shoot(int sx, int sy, int ddx) {
    if (!active) {
        x = sx;
        y = sy;
        dx = ddx;
        active = true;
    }
}

void Bullet::Update(bool isEnemy) {
    if (!active) return;
    y += isEnemy ? 1 : -1;
    x += dx;
    if (y < 0 || x < 0 || y >= 20 || x >= 100)
        active = false;
}