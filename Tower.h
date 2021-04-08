#ifndef TOWER_H_INCLUDED
#define TOWER_H_INCLUDED

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "Object.h"
#include "Attack.h"
#include "Circle.h"
#include "Monster.h"
#include "global.h"

class Tower : public Object
{
public:
    Tower(int, int);
    virtual ~Tower();

    // override virtual function "Object::Draw"
    void Draw();
    void resetAttackCounter() { attack_counter = 0; }
    void ToggleClicked() { isClicked = !isClicked; }
    void setCircle( int x, int y );
    void broke();

    // update whole attack set
    // if any attack goes out of the range of tower, delete it
    void UpdateAttack();
    // detect if tower needs to attack some monster
    bool DetectAttack(Monster*);
    // process if some of attack in set touches monster
    bool TriggerAttack(Monster*);
    bool Subtract_HP(int);
    bool Isuseful() { return useful; }

    int getHP() { return HealthPoint; }
    virtual int getWidth() { return 40; }
    virtual int getHeight() { return 40; }

    virtual int getType() { return type; }
    // show selected tower image on cursor position
    static void SelectedTower(int, int, int);
    static ALLEGRO_BITMAP* img_s[6];
    static ALLEGRO_BITMAP* img_cs[6];

protected:
    // setting of attack
    int attack_frequency = 20;
    int attack_counter = 0;
    int attack_harm_point = 5;
    int attack_velocity = 10;
    int HealthPoint = 500;
    int HP_STAY = 500;
    ALLEGRO_BITMAP *attack_img;
    std::vector<Attack*> attack_set;

    // information of tower
    int type;
    bool useful = true;
    bool isClicked = false;
    ALLEGRO_BITMAP *img;
    ALLEGRO_BITMAP *img_c;
    ALLEGRO_BITMAP *img_b;
    ALLEGRO_BITMAP *img_w;
};


#endif // TOWER_H_INCLUDED
