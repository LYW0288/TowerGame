#ifndef TURRENT_H_INCLUDED
#define TURRENT_H_INCLUDED

#include "Tower.h"

class Turrent : public Tower
{
public:
    Turrent(int mouse_x, int mouse_y) : Tower(mouse_x, mouse_y)
    {
        type = TURRENT;

        circle->r = 150;
        attack_frequency = 50;
        attack_velocity = 20;
        attack_harm_point = 20;
        HealthPoint = 2000;
        HP_STAY = 2000;
        img = al_load_bitmap("./Tower/Turrent/Turrent.png");
        img_b = al_load_bitmap("./Tower/Turrent/Turrent_b.png");
        img_w = al_load_bitmap("./Tower/Turrent/Turrent_w.png");
        img_c = al_load_bitmap("./Tower/Turrent/Turrent_c.png");
        attack_img = al_load_bitmap("./Tower/Turrent/Turrent_Beam.png");
    }

    int getWidth() override { return TowerWidth[TURRENT]; }
    int getHeight() override { return TowerHeight[TURRENT]; }
};

#endif // ARCANE_H_INCLUDED

