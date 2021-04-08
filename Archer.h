#ifndef ARCHER_H_INCLUDED
#define ARCHER_H_INCLUDED

#include "Tower.h"

class Archer : public Tower
{
public:
    Archer(int mouse_x = 0, int mouse_y = 0) : Tower(mouse_x, mouse_y)
    {
        type = ARCHER;

        circle->r = 80;
        attack_frequency = 15;
        attack_velocity = 8;
        attack_harm_point = 7;
        HealthPoint = 1000;
        HP_STAY = 1000;
        img = al_load_bitmap("./Tower/Archer/Archer.png");
        img_b = al_load_bitmap("./Tower/Archer/Archer_b.png");
        img_w = al_load_bitmap("./Tower/Archer/Archer_w.png");
        img_c = al_load_bitmap("./Tower/Archer/Archer_c.png");
        attack_img = al_load_bitmap("./Tower/Archer/Archer_Beam.png");
    }

    int getWidth() override { return TowerWidth[ARCHER]; }
    int getHeight() override { return TowerHeight[ARCHER]; }
};

#endif // ARCHER_H_INCLUDED
