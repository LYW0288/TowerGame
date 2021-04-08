#include "GameWindow.h"
#include "global.h"
#include <iostream>

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define ORANGE_LIGHT al_map_rgb(255, 196, 87)
#define ORANGE_DARK al_map_rgb(255, 142, 71)
#define PURPLE al_map_rgb(149, 128, 255)
#define BLUE al_map_rgb(77, 129, 179)

#define min(a, b) ((a) < (b)? (a) : (b))
#define max(a, b) ((a) > (b)? (a) : (b))


ALLEGRO_BITMAP* Tower::img_s[6] = {NULL};
ALLEGRO_BITMAP* Tower::img_cs[6];
float Attack::volume = 1.0;

void set_attack_volume(float volume)
{
    Attack::volume = volume;
}

bool compare(Tower *t1, Tower *t2)
{
    return (t1->getY() <= t2->getY());
}

void
GameWindow::game_init()
{
    Tower::img_s[0] = al_load_bitmap("./Tower/Arcane/Arcane.png");
    Tower::img_s[1] = al_load_bitmap("./Tower/Archer/Archer.png");
    Tower::img_s[2] = al_load_bitmap("./Tower/Canon/Canon.png");
    Tower::img_s[3] = al_load_bitmap("./Tower/Poison/Poison.png");
    Tower::img_s[4] = al_load_bitmap("./Tower/Storm/Storm.png");
    Tower::img_s[5] = al_load_bitmap("./Tower/Turrent/Turrent.png");
    Tower::img_cs[0] = al_load_bitmap("./Tower/Arcane/Arcane_c.png");
    Tower::img_cs[1] = al_load_bitmap("./Tower/Archer/Archer_c.png");
    Tower::img_cs[2] = al_load_bitmap("./Tower/Canon/Canon_c.png");
    Tower::img_cs[3] = al_load_bitmap("./Tower/Poison/Poison_c.png");
    Tower::img_cs[4] = al_load_bitmap("./Tower/Storm/Storm_c.png");
    Tower::img_cs[5] = al_load_bitmap("./Tower/Turrent/Turrent_c.png");
    char buffer[50];

    icon = al_load_bitmap("./icon.png");
    wall = al_load_bitmap("./Back/wall.png");
    rain= al_load_bitmap("./Back/rain.png");
    for(int i = 0; i < 4; i++)
    {
        sprintf(buffer, "./Back/leaf/leaf_%d.png", i+1);
        leaf[i] = al_load_bitmap(buffer);
    }
    talk[7]=al_load_bitmap("./Talk/TALK.png");
    for(int i = 0; i < 7; i++)
    {
        sprintf(buffer, "./Talk/TALK%d.png", i);
        talk[i] = al_load_bitmap(buffer);
    }
    for(int i = 0; i < 8; i++)
    {
        sprintf(buffer, "./Talk/TALK_E%d.png", i);
        talk_end[i] = al_load_bitmap(buffer);
    }
    talk_end[8]=al_load_bitmap("./Tower/Arcane/Arcane_Beam.png");
    for(int i = 0; i < 4; i++)
    {
        sprintf(buffer, "./Title/King/UP_%d.png", i);
        king[0][i] = al_load_bitmap(buffer);
    }
    for(int i = 0; i < 4; i++)
    {
        sprintf(buffer, "./Title/King/DOWN_%d.png", i);
        king[1][i] = al_load_bitmap(buffer);
    }
    title = al_load_bitmap("./Title/Title.png");
    title2 = al_load_bitmap("./Title/Title_2.png");
    title3 = al_load_bitmap("./Title/Title_B.png");
    BLOOD = al_load_bitmap("./Title/blood.png");
    for(int i = 0; i < 5; i++)
    {
        sprintf(buffer, "./Back/back%d.png", i);
        background[i] = al_load_bitmap(buffer);
    }
    for(int i = 0; i < 3; i++)
    {
        sprintf(buffer, "./Back/back%d-%d.png", i, i+1);
        change[i] = al_load_bitmap(buffer);
    }
    for(int i = 0; i < 4; i++)
    {
        sprintf(buffer, "./SelectScene/level%d.png", i+1);
        selectscene[i] = al_load_bitmap(buffer);
    }
    selectscene[4]= al_load_bitmap("./SelectScene/Locked.png");
    for(int i = 0; i < Num_TowerType; i++)
    {
        sprintf(buffer, "./Tower/%s.png", TowerClass[i]);
        tower[i] = al_load_bitmap(buffer);
    }

    al_set_display_icon(display, icon);
    al_reserve_samples(3);

    //MUSIC
    //for(int i = 0; i < 4; i++) backgroundSound[i] = al_load_sample("./Music/state%d.wav", i);
    Attacks = al_load_sample("Arrow.wav");
    backgroundSound[0] = al_load_sample("./Music/state0.wav");
    backgroundSound[1] = al_load_sample("./Music/state1.wav");
    backgroundSound[2] = al_load_sample("./Music/state2.wav");
    backgroundSound[3] = al_load_sample("./Music/state3.wav");
    startSound = al_load_sample("growl.wav");
    winSound = al_load_sample("Music/cheer.wav");
    loseSound = al_load_sample("Music/Sorrow.wav");
    titleSound = al_load_sample("./Music/The Sanctuary.wav");

    FILE *file;
    sprintf(buffer, "LEVEL/f.txt");
    file = fopen(buffer, "r");
    fscanf(file, "%s", buffer);
    first = atoi(buffer);
    fscanf(file, "%s", buffer);
    _end = atoi(buffer);
    fclose(file);

    level = new LEVEL(1);
    menu = new Menu();
}

bool
GameWindow::mouse_hover(int startx, int starty, int width, int height)
{
    if(mouse_x >= startx && mouse_x <= startx + width)
        if(mouse_y >= starty && mouse_y <= starty + height)
            return true;

    return false;
}

bool
GameWindow::isOnRoad()
{
    if( level->isTower(mouse_x/40 + mouse_y/40*15) )
        return true;
    int startx, starty;
    int widthOfTower;

    widthOfTower = TowerWidth[selectedTower];

    for(int i=0; i < NumOfGrid; i++)
    {
        startx = (i % 15) * 40;
        starty = (i / 15) * 40;

        if(level->isRoad(i)) {
            if((mouse_x + (widthOfTower/2) < startx) || (mouse_x - (widthOfTower/2) > startx + grid_width))
                continue;
            else if((mouse_y + (widthOfTower/2) < starty) || (mouse_y > starty + grid_height))
                continue;
            else
                return true;
        }
        if(level->isTree(i)) {
            if((mouse_x + (widthOfTower/2) < startx) || (mouse_x - (widthOfTower/2) > startx + grid_width))
                continue;
            else if((mouse_y + (widthOfTower/2) < starty) || (mouse_y > starty + grid_height))
                continue;
            else
                return true;
        }
    }
    return false;
}

Tower*
GameWindow::create_tower(int type)
{
    Tower *t = NULL;

    if(isOnRoad())
        return t;

    switch(type)
    {
    case ARCANE:
        t = new Arcane(mouse_x-mouse_x%4, mouse_y-mouse_y%4);
        break;
    case ARCHER:
        t = new Archer(mouse_x-mouse_x%4, mouse_y-mouse_y%4);
        break;
    case CANON:
        t = new Canon(mouse_x-mouse_x%4, mouse_y-mouse_y%4);
        break;
    case POISON:
        t = new Poison(mouse_x-mouse_x%4, mouse_y-mouse_y%4);
        break;
    case STORM:
        t = new Storm(mouse_x-mouse_x%4, mouse_y-mouse_y%4);
        break;
    case TURRENT:
        t = new Turrent(mouse_x-mouse_x%4, mouse_y-mouse_y%4);
        break;
    default:
        break;
    }
    level->settower(mouse_x/40 + mouse_y/40*15);

    menu->Change_Coin(menu->getTowerCoin(type));

    return t;
}

Monster*
GameWindow::create_monster()
{
    Monster *m = NULL;

    if(level->Monster_MAX) {
        switch(level->MonsterNum[level->Monster_MAX--]){
            case 1:
                m = new Wolf(level->ReturnPath());
                break;
            case 2:
                m = new WolfKnight(level->ReturnPath());
                break;
            case 3:
                m = new DemonNijia(level->ReturnPath());
                break;
            case 4:
                m = new CaveMan(level->ReturnPath());
                break;
            case 5:
                m = new Boss(level->ReturnPath());
                break;
        }
    }
    else
    {
        al_stop_timer(monster_pro);
    }

    return m;
}

void
GameWindow::game_play()
{
    int msg;

    srand(time(NULL));

    msg = -1;
    /*
    *  We start the game by calling game_begin to start timer and play music
    * [TODO]:
    *     You may add some function to create starting scene before calling game_begin
    *     e.g: game_start_scene()
    */
    game_begin();

    /*
    *  while game is running, the result of game_run will be returned to msg.
    *  If msg is GAME_EXIT, then tower game will terminate.
    */
    while(msg != GAME_EXIT)
        msg = game_run();

    show_err_msg(msg);
}

void
GameWindow::show_err_msg(int msg)
{
    if(msg == GAME_TERMINATE)
        fprintf(stderr, "Game Terminated...");
    else
        fprintf(stderr, "unexpected msg: %d", msg);

    game_destroy();
    exit(9);
}

GameWindow::GameWindow()
{
    if (!al_init())
        show_err_msg(-1);

    printf("Game Initializing...\n");

    display = al_create_display(window_width, window_height);
    event_queue = al_create_event_queue();

    /*
    * Create two timers to control different parts of tower game:
    *    a. timer: control the animation of each object, stopped when game doesn't run.
    *    b. monster_pro: control the production of monster, stooped when there is no need to produce monster.
    */
    timer = al_create_timer(1.0 / FPS);
    monster_pro = al_create_timer(1.0 * 80 / FPS);

    if(timer == NULL || monster_pro == NULL)
        show_err_msg(-1);

    if (display == NULL || event_queue == NULL)
        show_err_msg(-1);

    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    font = al_load_font("./Title/misaki_gothic.ttf", 24, NULL); // load small font
    Medium_font = al_load_ttf_font("./Title/misaki_gothic.ttf",48,0); //load medium font
    Large_font = al_load_ttf_font("./Title/misaki_gothic.ttf",72,0); //load large font

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(monster_pro));

    // Initializing resources that tower game needs.
    game_init();
}

void
GameWindow::game_begin()
{
    /*
    printf(">>> Start Level[%d]\n", level->getLevel());
    draw_running_map();

    al_play_sample_instance(startSound);
    while(al_get_sample_instance_playing(startSound));
    al_play_sample_instance(backgroundSound);
    */
    if(state == STATE_TITLE)set_state_title();
    else if(state == STATE_START) set_state_start();
    else if(state == STATE_CHANGE) set_state_change();
    else if(state == STATE_SETTING) set_state_setting();
    else if(state == STATE_LOSE) set_state_lose();
    else if(state == STATE_END) set_state_end();

    al_start_timer(timer);
    al_start_timer(monster_pro);
}

void
GameWindow::set_state_start()
{
    printf(">>> Start Level[%d]\n", level->getLevel());\
    if(start) draw_running_map();
    else draw_start_map();
    //al_play_sample(startSound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    al_play_sample(backgroundSound[level->getLevel()-1], volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void
GameWindow::set_state_title()
{
    al_stop_sample(ID);
    if(first)al_play_sample(titleSound, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, ID);
    draw_title_map();
}

void
GameWindow::set_state_end()
{
    al_stop_sample(ID);
    al_play_sample(titleSound, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, ID);
    draw_end_map();
}

void
GameWindow::set_state_lose()
{
    al_stop_sample(ID);
    al_play_sample(loseSound, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, ID);
    draw_lose_map();
}


void
GameWindow::set_state_setting()
{
    al_stop_samples();
    draw_setting_map();
}

void
GameWindow::set_state_change()
{
    if(level->getLevel() == 4) {
        state_change(0);
        return;
    }
    al_stop_sample(ID);
    al_play_sample(winSound, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    draw_change_map();
}

int
GameWindow::game_run()
{
    int error = GAME_CONTINUE;

    if (!al_is_event_queue_empty(event_queue)) {

        error = process_event();
    }
    return error;
}

int
GameWindow::game_update()
{
    static int num = 0;
    unsigned int i, j;
    std::list<Tower*>::iterator it;

    /*TODO:*/
    /*Allow towers to detect if monster enters its range*/
    /*Hint: Tower::DetectAttack*/
    for(it = towerSet.begin(); it != towerSet.end(); ++it){
        if(!(*it)->Isuseful()) continue;
        for(i=0; i < monsterSet.size(); i++)
        {
            Monster *m = monsterSet[i];
            Tower *t = *it;
            t->DetectAttack(m);
        }
    }


    // update every monster
    // check if it is destroyed or reaches end point
    for(i=0; i < monsterSet.size(); i++)
    {
        bool isDestroyed = false, isReachEnd = false;

        /*TODO:*/
        /*1. For each tower, traverse its attack set*/
        /*2. If the monster collide with any attack, reduce the HP of the monster*/
        /*3. Remember to set isDestroyed to "true" if monster is killed*/
        /*Hint: Tower::TriggerAttack*/
        for(it = towerSet.begin(); it != towerSet.end(); ++it){
            if(!(*it)->Isuseful()) continue;
            Monster *m = monsterSet[i];
            Tower *t = *it;
            if (t->TriggerAttack(m)) isDestroyed = true;
        }

        isReachEnd = monsterSet[i]->Move();

        if(isDestroyed)
        {
            Monster *m = monsterSet[i];

            menu->Change_Coin(m->getWorth());
            menu->Gain_Score(m->getScore());
            monsterSet.erase(monsterSet.begin() + i);
            i--;
            delete m;

        }
        else if(isReachEnd)
        {
            Monster *m = monsterSet[i];

            if(m->IsMonster("Boss")) state_change(STATE_LOSE);
            if(menu->Subtract_HP()){
                state_change(STATE_LOSE);
                return GAME_CONTINUE;
            }

            monsterSet.erase(monsterSet.begin() + i);
            i--;
            delete m;
        }
    }

    /*TODO:*/
    /*1. Update the attack set of each tower*/
    /*Hint: Tower::UpdateAttack*/

    for(it = towerSet.begin(); it != towerSet.end(); it++){
        if(!(*it)->Isuseful()) continue;
        Tower *t = *it;
        t->UpdateAttack();
        if(num%10 == 0){
            if(level->getLevel() == 2 || level->getLevel() == 3) t->Subtract_HP(5);
            else if(level->getLevel() == 4) t->Subtract_HP(10);
        }
        if(t->getHP() <= 0){
            t->broke();
        }
    }
    if(num%10 == 0) num = 0;
    num++;

    return GAME_CONTINUE;
}

void
GameWindow::game_reset()
{
    // clear all towers
    for(auto&& child : towerSet) {
        delete child;
    }
    towerSet.clear();
    monsterSet.clear();

    choose = 0;
    left = false;
    selectedTower = -1;
    lastClicked = -1;
    Coin_Inc_Count = 0;
    levelselect = 0;
    mute = false;
    redraw = false;
    next = 0;
    start = false;
    leafset[0] = false, leafset[1] = false, leafset[2] = false;
    if (!rainChange) rainset[0] = false, rainset[1] = false, rainset[2] = false;
    menu->Reset();

    // stop sample instance
    al_stop_sample(ID);

    // stop timer
    al_stop_timer(timer);
    al_stop_timer(monster_pro);

}

void
GameWindow::game_destroy()
{
    game_reset();

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_font(Medium_font);
    al_destroy_font(Large_font);

    al_destroy_timer(timer);
    al_destroy_timer(monster_pro);

    for(int i=0;i<5; i++)
        al_destroy_bitmap(tower[i]);

    al_destroy_bitmap(icon);
    al_destroy_bitmap(*background);

    al_destroy_sample(startSound);
    al_destroy_sample(*backgroundSound);

    delete level;
    delete menu;
}

int
GameWindow::process_event(){
    if(state == STATE_TITLE) return process_title_event();
    else if(state == STATE_START) return process_start_event();
    else if(state == STATE_LEVEL) return process_level_event();
    else if(state == STATE_CHANGE) return process_change_event();
    else if(state == STATE_FIRST) return process_first_event();
    else if(state == STATE_SETTING) return process_setting_event();
    else if(state == STATE_LOSE) return process_lose_event();
    else if(state == STATE_END) return process_end_event();
}

int
GameWindow::process_start_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    al_wait_for_event(event_queue, &event);
    redraw = false;
    if(start){
        if(event.type == ALLEGRO_EVENT_TIMER) {
            if(event.timer.source == timer) {
                redraw = true;

                if(Coin_Inc_Count == 0)
                    menu->Change_Coin(Coin_Time_Gain);

                Coin_Inc_Count = (Coin_Inc_Count + 1) % CoinSpeed;

                // All monsters are created and destroyed -> Game Exit
                if(monsterSet.size() == 0 && !al_get_timer_started(monster_pro))
                {
                    level->setRecord(1, menu->getHP(), menu->getCoin(), menu->getScore());
                    if(level->getLevel()!=4){
                        set_state_change();
                        state = STATE_CHANGE;
                    }
                    else{
                        win = true;
                        if(_end) state_change(STATE_LOSE);
                        else {
                            al_stop_sample(ID);
                            set_state_end();
                            state=STATE_END;
                        }
                    }
                    return GAME_CONTINUE;
                    //al_stop_timer(timer);
                    //return GAME_EXIT;
                }

            }
            else {
                Monster *m = create_monster();

                if(m != NULL)
                    monsterSet.push_back(m);
            }
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return GAME_EXIT;
        }
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(event.keyboard.keycode) {

                case ALLEGRO_KEY_P:
                    /*
                    * You can add some variable to control if game is paused.
                    * e.g: pause
                    * In addition to add variable, you also have to modify draw_running_map() and game_update()
                    * Or, monsters and towers can still work without being paused
                    */
                    if(pause){
                        al_start_timer(timer);
                        al_start_timer(monster_pro);
                    }
                    else{
                        al_stop_timer(timer);
                        al_stop_timer(monster_pro);
                    }
                    pause = !pause;
                    break;
                case ALLEGRO_KEY_M:
                    mute = !mute;
                    if(mute)
                        al_stop_sample(ID);
                    else
                        al_play_sample(backgroundSound[level->getLevel()-1], volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, ID);
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    state_change(STATE_TITLE);
                    break;
            }
        }
        else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if(event.mouse.button == 1) {
                if(left){
                    if(mouse_hover(lefx, lefy, 80, 40)) state_change(STATE_TITLE);
                    else left = false;
                }
                // Create tower
                if(selectedTower != -1 && mouse_hover(0, 0, field_width, field_height)) {
                    Tower *t = create_tower(selectedTower);

                    if(t == NULL)
                        printf("Wrong place\n");
                    else {
                        towerSet.push_back(t);
                        towerSet.sort(compare);
                    }
                }
                /*
                *  Check if any tower is clicked.
                *  If so, cancel the shown circle range of the tower.
                *  In addition, we also re-check if any circle range should show up.
                */
                else if(selectedTower == -1){
                    std::list<Tower*>::iterator it = towerSet.begin();
                    if(lastClicked != -1)
                    {
                        std::advance(it, lastClicked);
                        (*it)->ToggleClicked();
                    }
                    for(i=0, it = towerSet.begin(); it != towerSet.end(); it++, i++)
                    {
                        Circle *circle = (*it)->getCircle();
                        int t_width = (*it)->getWidth();

                        if(mouse_hover(circle->x - t_width/2, circle->y, t_width, t_width/2))
                        {
                            (*it)->ToggleClicked();
                            lastClicked = i;
                            break;
                        } else {
                            lastClicked = -1;
                        }
                    }

                }
                // check if user wants to create some kind of tower
                // if so, show tower image attached to cursor
                selectedTower = menu->MouseIn(mouse_x, mouse_y);

            }
            else if(event.mouse.button == 2) {
                if(selectedTower!=-1) selectedTower = -1;
                lefx = mouse_x;
                lefy = mouse_y;
                left = true;
            }
        }
        else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;

        menu->MouseIn(mouse_x, mouse_y);

    }
    }
    else {
        if(event.type == ALLEGRO_EVENT_TIMER) {
            if(event.timer.source == timer)
                redraw = true;
        }
        else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return GAME_EXIT;
        }
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    state_change(STATE_TITLE);
                    break;
            }
        }
    }
    if(redraw) {
        // update each object in game
        instruction = game_update();

        // Re-draw map
        if (start) draw_running_map();
        else draw_start_map();
        redraw = false;
    }

    return instruction;
}

int
GameWindow::process_title_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        if(event.timer.source == timer)
            redraw = true;
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                return GAME_EXIT;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        if(event.mouse.button == 1) {
            if(mouse_hover(window_width/2-64, window_height/2+2, 128, 20)){
                if(!first) state = STATE_FIRST, next = 0;
                else{
                    state = STATE_LEVEL;
                    level_choose[0] = false, level_choose[1] = false;
                }
            }
            else if(mouse_hover(window_width/2-64, window_height/2+22, 128, 20))
                state_change(STATE_SETTING);
            else if(mouse_hover(window_width/2-64, window_height/2+42, 128, 20))
                return GAME_EXIT;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
    }

    if(redraw) {
        // Re-draw map
        draw_title_map();
        redraw = false;
    }

    return instruction;
}


int
GameWindow::process_setting_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        if(event.timer.source == timer)
            redraw = true;
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                state_change(STATE_TITLE);
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        if(event.mouse.button == 1) {
            if(mouse_hover(4, 4, 56, 30)) state_change(STATE_TITLE);
            if(mouse_hover(window_width/2-100+180*volume, window_height/2-50, 20, 30)) choose = 1;
            else if(mouse_hover(window_width/2-100+180*Attack::volume, window_height/2+20, 20, 30)) choose = -1;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        if(event.mouse.button == 1) {
            if(choose!=0) choose = 0;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        if(choose==1){
            if(mouse_x<=window_width/2-90) volume = 0.0;
            else if(mouse_x >= window_width/2+90) volume = 1.0;
            else{
                int v = (mouse_x - window_width/2+90)/18;
                volume = (float)(v)/10;
            }
        }
        else if(choose==-1){
            if(mouse_x<=window_width/2-90) Attack::volume = 0.0;
            else if(mouse_x >= window_width/2+90) Attack::volume = 1.0;
            else{
                int v = (mouse_x - window_width/2+90)/18;
                Attack::volume = (float)(v)/10;
            }
        }
    }

    if(redraw) {
        // Re-draw map
        draw_setting_map();
        redraw = false;
    }

    return instruction;
}


int
GameWindow::process_lose_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        if(event.timer.source == timer)
            redraw = true;
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                state_change(STATE_TITLE);
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        if(event.mouse.button == 1) {
            if(mouse_hover(window_width/2-34, window_height/2+2, 68, 20))
                win = false;
                state_change(STATE_TITLE);
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
    }

    if(redraw) {
        // Re-draw map
        draw_lose_map();
        redraw = false;
    }

    return instruction;
}


int
GameWindow::process_first_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        if(event.timer.source == timer)
            redraw = true;
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                return GAME_EXIT;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        if(event.mouse.button == 1) {
            if(next>2 && next<8) next++;
            else if(next == 8){
                if(mouse_x>window_width/2-40 && mouse_y>window_height-160 && mouse_x<window_width/2+40 && mouse_y<window_height-120)
                    next++;
            }
            else if(next == 9){
                first = 1;
                FILE *fPtr;
                fPtr = fopen("LEVEL/f.txt", "w");
                fprintf(fPtr, "1 %d", _end);
                fclose(fPtr);
                state = STATE_LEVEL;
                level_choose[0] = false, level_choose[1] = false;
            }
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
    }

    if(redraw) {
        // Re-draw map
        draw_first_map();
        redraw = false;
    }

    return instruction;
}


int
GameWindow::process_end_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        if(event.timer.source == timer)
            redraw = true;
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                return GAME_EXIT;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        if(event.mouse.button == 1) {
            if(next>=2 && next<=8) next++;
            else if(next == 10)
                next++;
            else if(next == 11){
                next++;
                al_play_sample(Attacks, Attack::volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }
        else if(event.mouse.button == 2) {
            lefx = mouse_x;
            lefy = mouse_y;
            left = true;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
    }

    if(redraw) {
        // Re-draw map
        draw_end_map();
        redraw = false;
    }

    return instruction;
}

int
GameWindow::process_level_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        if(event.timer.source == timer)
            redraw = true;
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                state = STATE_TITLE;
                break;
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;
        if(event.mouse.button == 1) {
            if(event.mouse.y<window_height-10 &&event.mouse.y>window_height-50){
                if(event.mouse.x>window_width/2-170 && event.mouse.x<window_width/2-85){
                    level->setLevel(1);
                    state_change(STATE_START);
                }
                else if(event.mouse.x>window_width/2-85 && event.mouse.x<window_width/2 && level->init[0]) {
                    level->setLevel(2);
                    state_change(STATE_START);
                }
                else if(event.mouse.x>window_width/2 && event.mouse.x<window_width/2+85 && level->init[1]) {
                    level->setLevel(3);
                    state_change(STATE_START);
                }
                else if(event.mouse.x>window_width/2+85 && event.mouse.x<window_width/2+170 && level->init[2]) {
                    level->setLevel(4);
                    state_change(STATE_START);
                }
            }
            else if(event.mouse.y<34 &&event.mouse.y>8){
                if(event.mouse.x>8 && event.mouse.x<60){
                    state = STATE_TITLE;
                }
                else if(event.mouse.x>74 && event.mouse.x<144) level->setRecord(0, 0, 0, 0), next = 0, first = 0, _end = 0, state_change(STATE_TITLE);
            }
        }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES){
        mouse_x = event.mouse.x;
        mouse_y = event.mouse.y;


        if(event.mouse.y<window_height-10 &&event.mouse.y>window_height-50){
            if(event.mouse.x>window_width/2-170 && event.mouse.x<window_width/2-85) levelselect = 0;
            else if(event.mouse.x>window_width/2-85 && event.mouse.x<window_width/2) levelselect = 1;
            else if(event.mouse.x>window_width/2 && event.mouse.x<window_width/2+85) levelselect = 2;
            else if(event.mouse.x>window_width/2+85 && event.mouse.x<window_width/2+170) levelselect = 3;
        }
        else if(event.mouse.x>8 && event.mouse.x<60 && event.mouse.y<34 &&event.mouse.y>8) level_choose[0] = true;
        else if(event.mouse.x>74 && event.mouse.x<144 && event.mouse.y<34 &&event.mouse.y>8) level_choose[1] = true;
        else level_choose[0] = false, level_choose[1] = false;
    }

    if(redraw) {

        // Re-draw map
        draw_level_map();
        redraw = false;
    }

    return instruction;
}

int
GameWindow::process_change_event()
{
    int i;
    int instruction = GAME_CONTINUE;

    al_wait_for_event(event_queue, &event);
    redraw = false;

    if(event.type == ALLEGRO_EVENT_TIMER) {
        if(event.timer.source == timer)
            redraw = true;
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        return GAME_EXIT;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch(event.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                state_change(STATE_TITLE);
                break;
        }
    }
    if(redraw) {

        // Re-draw map
        draw_change_map();
        redraw = false;
    }

    return instruction;
}

void
GameWindow::draw_title_map()
{

    al_clear_to_color(al_map_rgb(0, 0, 0));
    if(_end) al_draw_bitmap(title3, 0, 0, 0);
    else al_draw_bitmap(title, 0, 0, 0);
    if(mouse_hover(window_width/2-64, window_height/2+2, 128, 20))
        al_draw_rectangle(window_width/2-64, window_height/2+2, window_width/2+64, window_height/2+22, al_map_rgb(255, 255, 255), 3);
    if(mouse_hover(window_width/2-64, window_height/2+22, 128, 20))
        al_draw_rectangle(window_width/2-64, window_height/2+22, window_width/2+64, window_height/2+42, al_map_rgb(255, 255, 255), 3);
    if(mouse_hover(window_width/2-64, window_height/2+42, 128, 20))
        al_draw_rectangle(window_width/2-64, window_height/2+42, window_width/2+64, window_height/2+62, al_map_rgb(255, 255, 255), 3);
    al_draw_text(font, al_map_rgb(255,255,255), window_width/2, window_height/2, ALLEGRO_ALIGN_CENTER, "Game start");
    al_draw_text(font, al_map_rgb(255,255,255), window_width/2, window_height/2+20, ALLEGRO_ALIGN_CENTER, "Setting");
    al_draw_text(font, al_map_rgb(255,255,255), window_width/2, window_height/2+40, ALLEGRO_ALIGN_CENTER, "EXIT");


    al_flip_display();
}


void
GameWindow::draw_lose_map()
{

    al_clear_to_color(al_map_rgb(0, 0, 0));
    if(win) al_draw_text(Medium_font, al_map_rgb(255,255,255), window_width/2, window_height/2-40, ALLEGRO_ALIGN_CENTER, "YOU WIN");
    else al_draw_text(Medium_font, al_map_rgb(255,255,255), window_width/2, window_height/2-40, ALLEGRO_ALIGN_CENTER, "YOU LOSE");
    if(mouse_hover(window_width/2-64, window_height/2+2, 128, 20))
        al_draw_rectangle(window_width/2-34, window_height/2+2, window_width/2+34, window_height/2+22, al_map_rgb(255, 255, 255), 3);
    al_draw_text(font, al_map_rgb(255,255,255), window_width/2, window_height/2, ALLEGRO_ALIGN_CENTER, "Back");


    al_flip_display();
}


void
GameWindow::draw_setting_map()
{

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_text(Medium_font, al_map_rgb(255,255,255), window_width/2, 40, ALLEGRO_ALIGN_CENTER, "VOLUME");
    al_draw_text(font, al_map_rgb(255,255,255), 40, window_height/2-50, ALLEGRO_ALIGN_LEFT, "BGM");
    al_draw_text(font, al_map_rgb(255,255,255), 40, window_height/2+30, ALLEGRO_ALIGN_LEFT, "SE");
    al_draw_filled_rectangle(window_width/2-90, window_height/2-40, window_width/2+90, window_height/2-30, al_map_rgb(255, 255, 255));
    al_draw_filled_rectangle(window_width/2-90, window_height/2+30, window_width/2+90, window_height/2+40, al_map_rgb(255, 255, 255));

    al_draw_filled_rectangle(window_width/2-90+180*volume, window_height/2-50, window_width/2-100+180*volume, window_height/2-20, al_map_rgb(255, 255, 255));
    al_draw_rectangle(window_width/2-90+180*volume, window_height/2-50, window_width/2-100+180*volume, window_height/2-20, al_map_rgb(0, 0, 0), 4);
    al_draw_filled_rectangle(window_width/2-90+180*Attack::volume, window_height/2+20, window_width/2-100+180*Attack::volume, window_height/2+50, al_map_rgb(255, 255, 255));
    al_draw_rectangle(window_width/2-90+180*Attack::volume, window_height/2+20, window_width/2-100+180*Attack::volume, window_height/2+50, al_map_rgb(0, 0, 0), 4);

    al_draw_text(font, al_map_rgb(255,255,255), 8, 8, ALLEGRO_ALIGN_LEFT, "BACK");
    if(mouse_hover(4, 4, 56, 30)) al_draw_rectangle(4, 4, 60, 34, al_map_rgb(255, 255, 255), 3);

    al_flip_display();
}


void
GameWindow::draw_end_map()
{
    static int y = 0, ky = 600, i=0, j=0;
    static bool up = false;
    static int now = 4;
    static int ec = 0;
    static int num = 0;
    if(next==0){
        ec = 0;
        now = 4;
        i = 0;
        j = 0;
        ky = 600;
        y = 0;
        next++;
        al_play_sample(titleSound, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, ID);
    }
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(background[3], 0, -y, 0);
    al_draw_bitmap(background[4], 0, -y+600, 0);
    for(std::list<Tower*>::iterator it = towerSet.begin(); it != towerSet.end(); it++){
        (*it)->setCircle((*it)->getX(), (*it)->getY()-4);
        (*it)->Draw();
    }
    al_draw_bitmap(king[i][j], 230, -y+ky+600, 0);
    if( y%150==0 && now != 0 ) now--;

    if(now){
        al_draw_bitmap(leaf[now-1], -l_x[0]*2, -600+l_x[0], 0);
        al_draw_bitmap(leaf[now-1], -l_x[1]*2, -600+l_x[1], 0);
        if(l_x[0] == 600) {
            l_x[0] = 0;
        }
        else l_x[0]+=1;
        if(l_x[1] == 600) {
            l_x[1] = 0;
        }
        else l_x[1]+=2;
    }

    switch(next){
        case 1:
            if(y == 600)
                next++, i=1, j=1;
            else{
                y+=4;
                ky-=2;
                if(ky%16==0) j++;
                if(j==4) j=0;
            }
            break;
        case 2:
            al_draw_bitmap(talk_end[0], window_width/2-Talk_r[0]/2-100, window_height/2-180, 0);
            al_draw_bitmap(talk[7], window_width/2-20-100, window_height/2-180+48+up*4, 0);
            break;
        case 3:
            al_draw_bitmap(talk_end[1], window_width/2-Talk_r[0]/2-100, window_height/2-180, 0);
            al_draw_bitmap(talk[7], window_width/2-20-100, window_height/2-180+48+up*4, 0);
            break;
        case 4:
            al_draw_bitmap(talk_end[2], window_width/2-Talk_r[0]/2-100, window_height/2-180, 0);
            al_draw_bitmap(talk[7], window_width/2-20-100, window_height/2-180+48+up*4, 0);
            break;
        case 5:
            al_draw_bitmap(talk_end[3], window_width/2-Talk_r[0]/2-67-100, window_height/2-180, 0);
            al_draw_bitmap(talk[7], window_width/2-20-100, window_height/2-180+48+up*4, 0);
            break;
        case 6:
            al_draw_bitmap(talk_end[4], window_width/2-Talk_r[0]/2-100, window_height/2-180, 0);
            al_draw_bitmap(talk[7], window_width/2-20-100, window_height/2-180+48+up*4, 0);
            break;
        case 7:
            al_draw_bitmap(talk_end[5], window_width/2-Talk_r[0]/2-100, window_height/2-180, 0);
            al_draw_bitmap(talk[7], window_width/2-20-100, window_height/2-180+48+up*4, 0);
            break;
        case 8:
            al_draw_bitmap(talk_end[6], window_width/2-Talk_r[0]/2-100, window_height/2-180, 0);
            al_draw_bitmap(talk[7], window_width/2-20-100, window_height/2-180+48+up*4, 0);
            break;
        case 9:
            Tower::SelectedTower(mouse_x, mouse_y, ARCANE);
            if(ec==50) ec = 0, next++;
            else ec++;
            break;
        case 10:
            Tower::SelectedTower(mouse_x, mouse_y, ARCANE);
            al_draw_bitmap(talk_end[7], window_width/2-Talk_r[0]/2-100, window_height/2-180, 0);
            al_draw_bitmap(talk[7], window_width/2-20-100, window_height/2-180+48+up*4, 0);
            break;
        case 11:
            Tower::SelectedTower(mouse_x, mouse_y, ARCANE);
            al_set_mouse_xy(display, 350, -y+ky+600);
            if(left) {
                if(mouse_hover(lefx-1, lefy-1, 80, 40)){
                    al_draw_filled_rectangle(lefx, lefy, lefx+80, lefy+40, al_map_rgb(0, 0, 0));
                    al_draw_rectangle(lefx, lefy, lefx+80, lefy+40, al_map_rgb(0, 0, 0), 4);
                    al_draw_text(font, al_map_rgb(255, 255, 255), lefx+40, lefy+10, ALLEGRO_ALIGN_CENTER, "KILL");
                }
                else{
                    al_draw_filled_rectangle(lefx, lefy, lefx+80, lefy+40, al_map_rgb(255, 255, 255));
                    al_draw_rectangle(lefx, lefy, lefx+80, lefy+40, al_map_rgb(0, 0, 0), 4);
                    al_draw_text(font, al_map_rgb(0, 0, 0), lefx+40, lefy+10, ALLEGRO_ALIGN_CENTER, "HIM");
                }
            }
            break;
        case 12:
            al_draw_bitmap(Tower::img_s[0], 350-36, -y+ky+600-121+36, 0);
            al_draw_bitmap(talk_end[8], 350-10-ec, -y+ky+600-10+ec/4, 0);
            if(ec==100) ec=0, next++;
            else ec+=4;
            break;
        case 13:
            al_draw_bitmap(Tower::img_s[0], 350-36, -y+ky+600-121+36, 0);
            al_draw_bitmap(talk_end[8], 250-10, -y+ky+600+15, 0);
            if(up)al_draw_filled_rectangle(0, 0, 600, 600, al_map_rgba(255, 0, 0, 100)), ec++;
            if(ec==4)next++;
            break;
        case 14:
            al_draw_bitmap(title3, 0, 0, 0);
            al_draw_text(font, al_map_rgb(255,255,255), window_width/2, window_height/2, ALLEGRO_ALIGN_CENTER, "Game start");
            al_draw_text(font, al_map_rgb(255,255,255), window_width/2, window_height/2+20, ALLEGRO_ALIGN_CENTER, "Setting");
            al_draw_text(font, al_map_rgb(255,255,255), window_width/2, window_height/2+40, ALLEGRO_ALIGN_CENTER, "EXIT");
            al_draw_bitmap(BLOOD, 0, -600+ec, 0);
            if(ec==1200){
                _end = 1;
                FILE *fPtr;
                fPtr = fopen("LEVEL/f.txt", "w");
                fprintf(fPtr, "%d 1", first);
                fclose(fPtr);
                state = STATE_TITLE;
            }
            else ec+=4;
            break;
    }
    if(next<14){
        al_draw_filled_rectangle(field_width, 0, window_width, window_height, al_map_rgb(0, 0, 0));
        menu->Draw();
    }
    if(num%15 == 0)num = 1, up=!up;
    else num++;
    al_flip_display();
}


void
GameWindow::draw_first_map()
{
    static int y = 0;
    static bool up = false;
    static int num = 0;
    if(next==0){
        y = 0;
        next++;
        al_play_sample(titleSound, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, ID);
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(title, 0, 0, 0);
    al_draw_bitmap(king[1][1], 377, 230-y, 0);
    al_draw_bitmap(title2, 0, 0, 0);
    al_draw_text(font, al_map_rgb(255,255,255), window_width/2, window_height/2, ALLEGRO_ALIGN_CENTER, "Game start");
    al_draw_text(font, al_map_rgb(255,255,255), window_width/2, window_height/2+20, ALLEGRO_ALIGN_CENTER, "Setting");
    al_draw_text(font, al_map_rgb(255,255,255), window_width/2, window_height/2+40, ALLEGRO_ALIGN_CENTER, "EXIT");
    switch(next){
        case 1:
            if(y == 150)
                next++;
            else y+=5;
            break;
        case 2:
            if(230-y == 158) next++;
            else y-=2;
            break;
        case 3:
            al_draw_bitmap(talk[0], window_width/2-Talk_r[0]/2, 20, 0);
            al_draw_bitmap(talk[7], window_width/2-20, 20+48+up*4, 0);
            break;
        case 4:
            al_draw_bitmap(talk[1], window_width/2-Talk_r[1]/2+20, 20, 0);
            al_draw_bitmap(talk[7], window_width/2-20, 20+48+up*4, 0);
            break;
        case 5:
            al_draw_bitmap(talk[2], window_width/2-Talk_r[2]/2-47, 20, 0);
            al_draw_bitmap(talk[7], window_width/2-20, 20+48+up*4, 0);
            break;
        case 6:
            al_draw_bitmap(talk[3], window_width/2-Talk_r[3]/2-7, 20, 0);
            al_draw_bitmap(talk[7], window_width/2-20, 20+48+up*4, 0);
            break;
        case 7:
            al_draw_bitmap(talk[4], window_width/2-Talk_r[4]/2-47, 20, 0);
            al_draw_bitmap(talk[7], window_width/2-20, 20+48+up*4, 0);
            break;
        case 8:
            al_draw_bitmap(talk[5], window_width/2-Talk_r[5]/2, 20, 0);
            al_draw_bitmap(talk[7], window_width/2-20, 20+48+up*4, 0);
            al_draw_filled_rectangle(window_width/2-40, window_height-160, window_width/2+40, window_height-120, al_map_rgb(255, 255, 255));
            al_draw_text(font, al_map_rgb(0, 0, 0), window_width/2, window_height-148, ALLEGRO_ALIGN_CENTER, "YES");
            break;
        case 9:
            al_draw_bitmap(talk[6], window_width/2-Talk_r[6]/2, 20, 0);
            al_draw_bitmap(talk[7], window_width/2-20, 20+48+up*4, 0);
            break;
    }
    if(num%15 == 0)num = 1, up=!up;
    else num++;
    al_flip_display();
}

void
GameWindow::draw_level_map()
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    switch (levelselect) {
        case 0:
            al_draw_rectangle(window_width/2-170, window_height-50, window_width/2-85, window_height-10, al_map_rgb(255, 255, 255), 3);
            al_draw_bitmap(selectscene[0], window_width/2-150, 60, 0);
        break;
        case 1:
            al_draw_rectangle(window_width/2-85, window_height-50, window_width/2, window_height-10, al_map_rgb(255, 255, 255), 3);
            if(!level->isInit(1)) al_draw_bitmap(selectscene[4], window_width/2-150, 60, 0);
            else al_draw_bitmap(selectscene[1], window_width/2-150, 60, 0);
            break;
        case 2:
            al_draw_rectangle(window_width/2, window_height-50, window_width/2+85, window_height-10, al_map_rgb(255, 255, 255), 3);
            if(!level->isInit(2)) al_draw_bitmap(selectscene[4], window_width/2-150, 60, 0);
            else al_draw_bitmap(selectscene[2], window_width/2-150, 60, 0);
            break;
        case 3:
            al_draw_rectangle(window_width/2+85, window_height-50, window_width/2+170, window_height-10, al_map_rgb(255, 255, 255), 3);
            if(!level->isInit(3)) al_draw_bitmap(selectscene[4], window_width/2-150, 60, 0);
            else al_draw_bitmap(selectscene[3], window_width/2-150, 60, 0);
            break;
    }
    if(level_choose[0]) {
        al_draw_rectangle(4, 4, 60, 34, al_map_rgb(255, 255, 255), 3);
    }
    else if(level_choose[1]) al_draw_rectangle(74, 4, 144, 34, al_map_rgb(255, 255, 255), 3);
    al_draw_text(font, al_map_rgb(255,255,255), window_width/2, window_height-40, ALLEGRO_ALIGN_CENTER, "LEVEL1 LEVEL2 LEVEL3 LEVEL4");


    al_draw_bitmap(menu->love, window_width/2-30, 400, 0);
    al_draw_textf(font, al_map_rgb(255, 255, 255), window_width/2, 400, ALLEGRO_ALIGN_LEFT, "x%d", level->heart[levelselect]);

    al_draw_textf(font, al_map_rgb(255, 255, 255), window_width/2, 440, ALLEGRO_ALIGN_CENTER, "Coin : %d", level->coin[levelselect]);

    al_draw_textf(font, al_map_rgb(255, 255, 255), window_width/2, 480, ALLEGRO_ALIGN_CENTER, "Score : %d", level->score[levelselect]);


    al_draw_text(font, al_map_rgb(255,255,255), 8, 8, ALLEGRO_ALIGN_LEFT, "BACK  CLEAR");
    al_flip_display();
}

void
GameWindow::draw_running_map()
{
    unsigned int i, j;

    al_clear_to_color(al_map_rgb(100, 100, 100));
    switch (level->getLevel()){
        case 1:
            al_draw_bitmap(background[0], 0, 0, 0);
            break;
        case 2:
            al_draw_bitmap(background[1], 0, 0, 0);
            break;
        case 3:
            al_draw_bitmap(background[2], 0, 0, 0);
            break;
        case 4:
            al_draw_bitmap(background[3], 0, 0, 0);
            break;
    }
    for(i = 0; i < field_height/40; i++)
    {
        for(j = 0; j < field_width/40; j++)
        {
            char buffer[50];
            sprintf(buffer, "%d", i*15 + j);
            for(std::list<Tower*>::iterator it = towerSet.begin(); it != towerSet.end(); it++)
                if( (*it)->getX()/40 + (*it)->getY()/8*3 == i*15 + j ) (*it)->Draw();
            for(int k=0; k<monsterSet.size(); k++)
            {
                if( monsterSet[k]->getX()/40 + monsterSet[k]->getY()/8*3 == i*15 + j ) monsterSet[k]->Draw();
            }
/*
            if(level->isRoad(i*15 + j)) {
                al_draw_filled_rectangle(j*40, i*40, j*40+40, i*40+40, al_map_rgb(198, 186, 133));
            }
*/
            // For debug usage, if you want to create a new map, you may turn off this comment.
            // al_draw_text(font, al_map_rgb(0, 0, 0), j*40 + 20, i*40 + 14, ALLEGRO_ALIGN_CENTER, buffer);
        }
    }
    if(level->getLevel() == 3)
        al_draw_bitmap(wall, 0, 0, 0);
    if(level->getLevel() == 2 || level->getLevel() == 3){
        if(!rainset[2]) r_x[0]=0, r_x[1]=0, pos_x=0, pos_y=0, rainset[2]=true;

        al_draw_bitmap(rain, -r_x[0]-pos_x, -1200+r_x[0]*2-pos_y, 0);
        if(rainset[0])
            al_draw_bitmap(rain, -r_x[0]-pos_x, 0+r_x[0]*2-pos_y, 0);

        al_draw_bitmap(rain, -r_x[1]-pos_x, -1200+r_x[1]*2-pos_y, 0);
        if(rainset[1])
            al_draw_bitmap(rain, -r_x[1]-pos_x, 0+r_x[1]*2-pos_y, 0);

        if(r_x[0] == 600) {
            r_x[0] = 0;
            if(!rainset[0]) rainset[0] = true;
        }
        else r_x[0]+=1;
        if(r_x[1] == 600) {
            r_x[1] = 0;
            if(!rainset[1]) rainset[1] = true;
        }
        else r_x[1]+=2;
    }
    else if(level->getLevel() == 4){

        al_draw_bitmap(leaf[3], -l_x[0]*2, -600+l_x[0], 0);

        al_draw_bitmap(leaf[3], -l_x[1]*2, -600+l_x[1], 0);

        if(l_x[0] == 600) {
            l_x[0] = 0;
        }
        else l_x[0]+=1;
        if(l_x[1] == 600) {
            l_x[1] = 0;
        }
        else l_x[1]+=2;
    }

    if(selectedTower != -1)
        Tower::SelectedTower(mouse_x, mouse_y, selectedTower);

    al_draw_filled_rectangle(field_width, 0, window_width, window_height, al_map_rgb(0, 0, 0));

    menu->Draw();

    if(left) {
        if(mouse_hover(lefx-1, lefy-1, 80, 40)){
            al_draw_filled_rectangle(lefx, lefy, lefx+80, lefy+40, al_map_rgb(0, 0, 0));
            al_draw_rectangle(lefx, lefy, lefx+80, lefy+40, al_map_rgb(0, 0, 0), 4);
            al_draw_text(font, al_map_rgb(255, 255, 255), lefx+40, lefy+10, ALLEGRO_ALIGN_CENTER, "EXIT");
        }
        else{
            al_draw_filled_rectangle(lefx, lefy, lefx+80, lefy+40, al_map_rgb(255, 255, 255));
            al_draw_rectangle(lefx, lefy, lefx+80, lefy+40, al_map_rgb(0, 0, 0), 4);
            al_draw_text(font, al_map_rgb(0, 0, 0), lefx+40, lefy+10, ALLEGRO_ALIGN_CENTER, "EXIT");
        }
    }

    al_flip_display();
}

void
GameWindow::draw_start_map()
{
    unsigned int i, j;
    static int num = 0;
    static int now = 0;
    static int x = -300;

    al_clear_to_color(al_map_rgb(100, 100, 100));
    switch (level->getLevel()){
        case 1:
            al_draw_bitmap(background[0], 0, 0, 0);
            break;
        case 2:
            al_draw_bitmap(background[1], 0, 0, 0);
            break;
        case 3:
            al_draw_bitmap(background[2], 0, 0, 0);
            al_draw_bitmap(wall, 0, 0, 0);
            break;
        case 4:
            al_draw_bitmap(background[3], 0, 0, 0);
            break;
    }
    if(rainChange){
        al_draw_bitmap(rain, -r_x[0]-pos_x, -1200+r_x[0]*2-pos_y, 0);
        al_draw_bitmap(rain, -r_x[0]-pos_x, 0+r_x[0]*2-pos_y, 0);

        al_draw_bitmap(rain, -r_x[1]-pos_x, -1200+r_x[1]*2-pos_y, 0);
        al_draw_bitmap(rain, -r_x[1]-pos_x, 0+r_x[1]*2-pos_y, 0);

        if(r_x[0] == 600) r_x[0] = 0;
        else r_x[0]+=1;
        if(r_x[1] == 600) r_x[1] = 0;
        else r_x[1]+=2;
    }
    else if(level->getLevel() == 4){
        if(!leafset[2]) l_x[0]=0, l_x[1]=0, leafset[2]=true, now =0;
        if( x%140 == 0 && now != 4 ) now++;

        if(now){
            al_draw_bitmap(leaf[now-1], -l_x[0]*2, -600+l_x[0], 0);
            al_draw_bitmap(leaf[now-1], -l_x[1]*2, -600+l_x[1], 0);
        }

        if(l_x[0] == 600) {
            l_x[0] = 0;
        }
        else l_x[0]+=1;
        if(l_x[1] == 600) {
            l_x[1] = 0;
        }
        else l_x[1]+=2;
    }
    al_draw_textf(Large_font, al_map_rgb(0, 0, 0), x, 20, 0, "LEVEL %d", level->getLevel());

    al_draw_filled_rectangle(field_width, 0, window_width, window_height, al_map_rgb(0, 0, 0));

    menu->Draw();

    al_flip_display();

    if(x == 40) {
        if(num != 80) num++;
        else {
            num = 0;
            x = -300;
            start = true;
            rainChange = false;
        }
    }
    else x += 2;
}

void
GameWindow::draw_change_map()
{
    static int x=0, y=0;
    static bool shine = true;
    static int num = 0;
    int i=level->getLevel()-1;

    al_clear_to_color(al_map_rgb(100, 100, 100));
    switch (level->getLevel()){
        case 1:
            x+=5;
            if(x%15==0) y++;
            break;
        case 2:
            x+=3;
            if(x%3==0) y+=2;
            break;
        case 3:
            y+=5;
            if(y%15==0) x--;
            break;
    }
    if(level->getLevel() != 3) al_draw_bitmap(change[i], 0-x, 0-y, 0);
    else al_draw_bitmap(change[i], -40-x, 0-y, 0);
    for(std::list<Tower*>::iterator it = towerSet.begin(); it != towerSet.end(); it++){
        switch (level->getLevel()){
            case 1:
                (*it)->setCircle((*it)->getX()-5, (*it)->getY());
                if(x%15==0) (*it)->setCircle((*it)->getX(), (*it)->getY()-1);
                break;
            case 2:
                (*it)->setCircle((*it)->getX()-3, (*it)->getY());
                if(x%3==0) (*it)->setCircle((*it)->getX(), (*it)->getY()-2);
                break;
            case 3:
                (*it)->setCircle((*it)->getX(), (*it)->getY()-5);
                if(y%15==0) (*it)->setCircle((*it)->getX()+1, (*it)->getY());
                break;
        }
        (*it)->Draw();
    }
    if(level->getLevel() == 3)
        al_draw_bitmap(wall, 0-x, 0-y, 0);

    if(shine) al_draw_textf(Large_font, al_map_rgb(0, 0, 0), 300, 300, ALLEGRO_ALIGN_CENTRE, "YOU WIN!!!");
    if(num%5 == 0)shine = !shine;

    if(i == 1){
        al_draw_bitmap(rain, -r_x[0]-x, -1200+r_x[0]*2-y, 0);
        al_draw_bitmap(rain, -r_x[0]-x, 0+r_x[0]*2-y, 0);

        al_draw_bitmap(rain, -r_x[1]-x, -1200+r_x[1]*2-y, 0);
        al_draw_bitmap(rain, -r_x[1]-x, 0+r_x[1]*2-y, 0);
        if(r_x[0] == 600) r_x[0] = 0;
        else r_x[0]+=1;
        if(r_x[1] == 600) r_x[1] = 0;
        else r_x[1]+=2;
    }

    al_draw_filled_rectangle(field_width, 0, window_width, window_height, al_map_rgb(0, 0, 0));

    menu->Draw();
    num++;

    al_flip_display();

    switch (level->getLevel()){
        case 1:
            if(x==600){
                x = 0, y = 0;
                num = 0;
                level->setLevel(i+2);
                state_change(1);
            }
            break;
        case 2:
            if(x == 600){
                rainChange = true;
                pos_x = x, pos_y = y;
                x = 0, y = 0;
                num = 0;
                level->setLevel(i+2);
                state_change(1);
            }
            break;
        case 3:
            if(y == 600){
                x = 0, y = 0;
                num = 0;
                level->setLevel(i+2);
                state_change(1);
            }
            break;
    }
}

void
GameWindow::state_change(int _state){
    game_reset();
    state = _state;
    game_begin();
}
