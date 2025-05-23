#pragma once

class Entity {
public:
    int x, y;
    Entity(int x, int y) : x(x), y(y) {}
    virtual ~Entity() = default;
};