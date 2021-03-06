#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include "Object.h"
#include "Circle.h"
#include "global.h"

class Menu : public Object
{
public:
    Menu();
    ~Menu();

    void Reset();
    void Draw();

    // Detect if cursor hovers over any of tower on menu
    // If so, return its type
    // Otherwise, return -1
    int MouseIn(int, int);

    // static function that detect if one point is on a line
    // This function is just used to simplify "MouseIn"
    static bool isInRange(int, int, int);

    // Check if current coin is not less than needed coin
    bool Enough_Coin(int);
    void Change_Coin(int change) { Coin += change; }
    bool Subtract_HP(int escapeNum = 1);
    void Gain_Score(int);

    int getTowerCoin(int type) { return need_coin[type]; }
    int getScore() { return Score; }
    int getCoin() { return Coin; }
    int getHP() { return HealthPoint; }
    int getKilled() { return killedMonster; }
    ALLEGRO_BITMAP *love = NULL;

private:
    std::vector<ALLEGRO_BITMAP*> menu_tower;
    ALLEGRO_FONT *menuFont;
    int HealthPoint = 10;
    int killedMonster = 0;
    int Score = 0;
    int Coin = 0;
    int selectedTower = -1;
    int HP[Num_TowerType] = {500, 1000, 1000, 5000, 200, 2000};
    int ATK[Num_TowerType] = {5, 7, 20, 5, 10, 20};
    int need_coin[Num_TowerType] = {-5, -10, -20, -30, -70, -70};
};

#endif // MENU_H_INCLUDED
