#pragma once

struct Entity {
    int x, y;
    Entity(int x, int y) : x(x), y(y) {}
    virtual ~Entity() = default;
};