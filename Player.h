#pragma once
#include "Entity.h"

class Player : public Entity {
public:
    Player(int x, int y); 
    void MoveLeft();
    void MoveRight();
};