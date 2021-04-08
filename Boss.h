#ifndef BOSS_H_INCLUDED
#define BOS_H_INCLUDED

#include "Monster.h"

class Boss : public Monster
{
public:
    Boss(std::vector<int> path) : Monster(path)
    {
        direction_count[LEFT] = 4;
        direction_count[RIGHT] = 4;
        direction_count[UP] = 4;
        direction_count[DOWN] = 4;

        HealthPoint = 1000;
        speed = 1;
        worth = 1000;
        score = 1000;

        strncpy(class_name, "Boss", 20);

        Load_Move();
    }

};


#endif // CLAW_H_INCLUDED

