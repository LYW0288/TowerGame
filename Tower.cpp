#include "Tower.h"


Tower::Tower(int pos_x = 0, int pos_y = 0)
{
    this->circle = new Circle(pos_x, pos_y, 70);
}


Tower::~Tower()
{
    delete circle;

    al_destroy_bitmap(img);
    al_destroy_bitmap(attack_img);

    for(auto&& child : this->attack_set) {
        delete child;
    }
    this->attack_set.clear();
}


void
Tower::setCircle( int x, int y ){
    this->circle->x=x;
    this->circle->y=y;
}

void
Tower::Draw()
{
    int draw_x = circle->x - (TowerWidth[this->type]/2);
    int draw_y = circle->y - (TowerHeight[this->type] - (TowerWidth[this->type]/2));


    if(HealthPoint>=HP_STAY/2) al_draw_bitmap(img, draw_x, draw_y, 0);
    else if(HealthPoint>0) al_draw_bitmap(img_b, draw_x, draw_y, 0);
    else al_draw_bitmap(img_w, draw_x, draw_y, 0);
    attack_counter = (attack_counter + 1) % attack_frequency;

    for(unsigned int i=0; i<this->attack_set.size(); i++){
        this->attack_set[i]->Draw();
        if(attack_set[i]->getDirection_y()<0) {
            if(HealthPoint>=HP_STAY/2) al_draw_bitmap(img, draw_x, draw_y, 0);
            else if(HealthPoint>0) al_draw_bitmap(img_b, draw_x, draw_y, 0);
            else al_draw_bitmap(img_w, draw_x, draw_y, 0);
        }
    }
    if(isClicked)
    {
        al_draw_bitmap(img_c, circle->x-circle->r, circle->y-circle->r, 0);
        al_draw_filled_rectangle(circle->x-2, circle->y-2, circle->x+2, circle->y+2, al_map_rgb(0, 0, 0));
    }
}

void
Tower::broke(){

    attack_set.clear();
    useful = false;
}

void
Tower::SelectedTower(int mouse_x, int mouse_y, int type)
{
    int draw_x = mouse_x - mouse_x%4 - (TowerWidth[type]/2);
    int draw_y = mouse_y - mouse_y%4 - (TowerHeight[type] - (TowerWidth[type]/2));


    al_draw_bitmap(Tower::img_s[type], draw_x, draw_y, 0);
    al_draw_bitmap(Tower::img_cs[type], mouse_x - mouse_x%4-TowerRadius[type], mouse_y - mouse_y%4-TowerRadius[type], 0);
    al_draw_filled_rectangle(mouse_x - mouse_x%4-2, mouse_y - mouse_y%4-2, mouse_x - mouse_x%4+2, mouse_y - mouse_y%4+2, al_map_rgb(0, 0, 0));
}

bool
Tower::DetectAttack(Monster *monster)
{
    bool willAttack = false;
    Attack *attack;

    if(Circle::isOverlap(this->circle, monster->getCircle()))
    {
        /*
        * TODO:
        *   Don't allow tower make new attack over and over again.
        *   Only when the counter reaches "0" can tower trigger new attack.
        *
        */

        if(!attack_counter){
            attack = new Attack(
                this->circle,
                monster->getCircle(),
                this->attack_harm_point,
                this->attack_velocity,
                this->attack_img
            );

            this->attack_set.push_back(attack);
            willAttack = true;
        }
    }

    return willAttack;
}

bool
Tower::Subtract_HP(int harm_point)
{
    HealthPoint -= harm_point;

    return (HealthPoint <= 0);
}

bool
Tower::TriggerAttack(Monster *monster)
{
    bool isDestroyed = false;

    for(unsigned int i = 0; i < this->attack_set.size(); i++)
    {
        if(Circle::isOverlap(attack_set[i]->getCircle(), monster->getCircle()))
        {
            /*TODO:*/
            /*1. Reduce the monster HP by the harm point*/
            /*2. Erase and delete the attack from attack set*/
            /*3. Return true if the monster's HP is reduced to zero*/
            this->attack_set.erase(this->attack_set.begin() + i);
            if(monster->Subtract_HP(this->attack_harm_point)) return true;
        }
    }
    return false;
}

void
Tower::UpdateAttack()
{
    for(unsigned int i=0; i < this->attack_set.size(); i++)
    {
        if(!Circle::isOverlap(this->attack_set[i]->getCircle(), this->circle))
        {
            Attack *attack = this->attack_set[i];

            this->attack_set.erase(this->attack_set.begin() + i);
            i--;
            delete attack;
        }
    }
}
