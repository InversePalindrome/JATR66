/*
Copyright (c) 2018 Inverse Palindrome
Apophis - ObjectType.hpp
InversePalindrome.com
*/


#pragma once

#include <enum.h>


BETTER_ENUM(ObjectType, int,
    Undefined = 0,
    Player = (1 << 0),
    Striker = (1 << 1),
    Drone = (1 << 2),
    Projectile = (1 << 3),
    Explosion = (1 << 4),
    Asteroid = (1 << 5),
    Item = (1 << 6),
    Patrol = (1 << 7),
    Path = (1 << 8),
    Enemy = Striker | Drone | Patrol,
    Alive = Player | Enemy,
    Obstacle = Asteroid | Item)