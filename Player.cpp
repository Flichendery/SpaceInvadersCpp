#include "Player.h"

Player::Player(int x, int y) : Entity(x, y) {}

void Player::MoveLeft() {
    --x;
}

void Player::MoveRight() {
    ++x;
}