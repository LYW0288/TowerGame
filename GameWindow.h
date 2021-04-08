#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include <iostream>
#include <string>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <list>
#include <time.h>
#include "Menu.h"
#include "Level.h"
#include "WolfKnight.h"
#include "CaveMan.h"
#include "Wolf.h"
#include "DemonNijia.h"
#include "Arcane.h"
#include "Archer.h"
#include "Canon.h"
#include "Poison.h"
#include "Turrent.h"
#include "Storm.h"
#include "Attack.h"
#include "Slider.h"
#include "Boss.h"

#define GAME_INIT -1
#define GAME_SETTING 0
#define GAME_SELECT 1
#define GAME_BEGIN 2
#define GAME_CONTINUE 3
#define GAME_FAIL 4
#define GAME_TERMINATE 5
#define GAME_NEXT_LEVEL 6
#define GAME_EXIT 7
#define GAME_WIN 8

#define STATE_TITLE 0
#define STATE_START 1
#define STATE_LEVEL 2
#define STATE_CHANGE 3
#define STATE_FIRST 4
#define STATE_SETTING 5
#define STATE_LOSE 6
#define STATE_END 7

using namespace std;
// clock rate
const float FPS = 60;

// total number of level
const int LevelNum = 4;

// 1 coin every 2 seconds
const int CoinSpeed = FPS * 2;
const int Coin_Time_Gain = 1;

const int Talk_r[7] = {200, 240, 240, 320, 240, 200, 200};
class GameWindow
{
public:
    // constructor
    GameWindow();
    // each process of scene
    void game_init();
    void game_reset();
    void game_play();
    void game_begin();

    int game_run();
    int game_update();

    void show_err_msg(int msg);
    void game_destroy();

    //state
    void set_state_start();
    void set_state_title();
    void set_state_change();
    void set_state_setting();
    void set_state_lose();
    void set_state_end();


    // draw running game map
    void draw_running_map();
    void draw_start_map();
    void draw_title_map();
    void draw_level_map();
    void draw_change_map();
    void draw_first_map();
    void draw_setting_map();
    void draw_lose_map();
    void draw_end_map();

    void state_change(int);

    // process of updated event
    int process_event();
    int process_start_event();
    int process_title_event();
    int process_level_event();
    int process_change_event();
    int process_first_event();
    int process_setting_event();
    int process_lose_event();
    int process_end_event();
    // detect if mouse hovers over a rectangle
    bool mouse_hover(int, int, int, int);
    // detect if a tower will be constructed on road
    bool isOnRoad();


    Tower* create_tower(int);
    Monster* create_monster();

public:
    bool initial = true;

private:
    ALLEGRO_BITMAP *icon;
    ALLEGRO_BITMAP *wall;
    ALLEGRO_BITMAP *rain = NULL;
    ALLEGRO_BITMAP *leaf[4];
    ALLEGRO_BITMAP *tower[Num_TowerType];
    ALLEGRO_BITMAP *selectscene[5];
    ALLEGRO_BITMAP *background[5];
    ALLEGRO_BITMAP *change[3];
    ALLEGRO_BITMAP *king[2][4];
    ALLEGRO_BITMAP *talk[8];
    ALLEGRO_BITMAP *talk_end[10];
    ALLEGRO_BITMAP *title = NULL;
    ALLEGRO_BITMAP *title2 = NULL;
    ALLEGRO_BITMAP *title3 = NULL;
    ALLEGRO_BITMAP *BLOOD = NULL;

    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_FONT *Medium_font = NULL;
    ALLEGRO_FONT *Large_font = NULL;

    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_TIMER *monster_pro = NULL;

    ALLEGRO_SAMPLE_ID *ID = (ALLEGRO_SAMPLE_ID *)malloc(sizeof(ALLEGRO_SAMPLE_ID));
    //start
    ALLEGRO_SAMPLE *Attacks = NULL;
    ALLEGRO_SAMPLE *startSound = NULL;
    ALLEGRO_SAMPLE *clearSound = NULL;
    ALLEGRO_SAMPLE *failSound = NULL;
    ALLEGRO_SAMPLE *winSound = NULL;
    ALLEGRO_SAMPLE *loseSound = NULL;
    ALLEGRO_SAMPLE *backgroundSound[5] = {NULL};

    //title
    ALLEGRO_SAMPLE *titleSound = NULL;


    LEVEL *level = NULL;
    Menu *menu = NULL;

    std::vector<Monster*> monsterSet;
    std::list<Tower*> towerSet;

    int state = STATE_TITLE;
    int Coin_Inc_Count = 0;
    int mouse_x, mouse_y;
    int selectedTower = -1, lastClicked = -1;
    int levelselect = 0;
    int r_x[2]={0};
    int l_x[2]={0};
    int pos_x, pos_y;
    int next = 0;
    int lefx, lefy;
    int choose = 0;
    float volume = 1.0;

    bool win = false;
    bool pause = false;
    bool left = false;
    bool first = false, _end = false;
    bool rainset[3] = {false};
    bool leafset[3] = {false};
    bool rainChange = false;
    bool start = false;
    bool redraw = false;
    bool mute = false;
    bool level_choose[2] = {false};
};


#endif // MAINWINDOW_H_INCLUDED
