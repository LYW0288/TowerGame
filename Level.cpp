#include "Level.h"

LEVEL::LEVEL(const int level) {
    char buffer[50];

    setLevel(level);
}

LEVEL::~LEVEL()
{
    for(int i=0;i < NumOfGrid;i++)
    {
        levelMap[i].roadPoint = false;
    }
}

void
LEVEL::setRecord(int _init, int _heart, int _coin, int _score){
    int now = this->getLevel();

    init[now-1] = _init;
    heart[now-1] = _heart;
    coin[now-1] = _coin;
    score[now-1] = _score;
    FILE *fPtr;
    fPtr = fopen("LEVEL/LEVEL_r.txt", "w");
    if(_init) {
        fprintf(fPtr, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
            init[0], init[1], init[2], init[3],
            heart[0], heart[1], heart[2], heart[3],
            coin[0], coin[1], coin[2], coin[3],
            score[0], score[1], score[2], score[3]);
    }
    else {
        fprintf(fPtr, "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
        memset(init, 0, sizeof(init));
        memset(heart, 0, sizeof(heart));
        memset(coin, 0, sizeof(coin));
        memset(score, 0, sizeof(score));
    }
    fclose(fPtr);
    if(!_init) {
        fPtr = fopen("LEVEL/f.txt", "w");
        fprintf(fPtr, "0 0");
        fclose(fPtr);
    }
}

void
LEVEL::setLevel(const int level)
{
    char buffer[50];
    FILE *file;

    sprintf(buffer, "LEVEL/LEVEL%d.txt", level);
    file = fopen(buffer, "r");

    this->level = level;
    this->road_grid.clear();

    for(int i = 0; i < NumOfGrid; i++)
    {
        this->levelMap[i].roadPoint = false;
        this->levelMap[i].treePoint = false;
        this->levelMap[i].towerPoint = false;
    }

    fscanf(file, "%s", buffer);
    Monster_MAX = atoi(buffer);

    for(int i=Monster_MAX; i > 0; i--)
    {
        fscanf(file, "%s", buffer);
        MonsterNum[i] = atoi(buffer);
    }

    while(fscanf(file, "%s", buffer) != EOF) {
        this->levelMap[atoi(buffer)].roadPoint = true;
        road_grid.push_back(atoi(buffer));
    }

    fclose(file);


    sprintf(buffer, "Tree/LEVEL%d.txt", level);
    file = fopen(buffer, "r");

    fscanf(file, "%s", buffer);


    while(fscanf(file, "%s", buffer) != EOF) this->levelMap[atoi(buffer)].treePoint = true;

    fclose(file);

    //RECORD


    sprintf(buffer, "LEVEL/LEVEL_r.txt", level);
    file = fopen(buffer, "r");

    for(int i=0; i<4; i++){
        fscanf(file, "%s", buffer);
        init[i] = atoi(buffer);
    }

    for(int i=0; i<4; i++){
        fscanf(file, "%s", buffer);
        heart[i] = atoi(buffer);
    }
    for(int i=0; i<4; i++){
        fscanf(file, "%s", buffer);
        coin[i] = atoi(buffer);
    }
    for(int i=0; i<4; i++){
        fscanf(file, "%s", buffer);
        score[i] = atoi(buffer);
    }

    fclose(file);
}
