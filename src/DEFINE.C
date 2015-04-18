#include <stdio.h>
#include <stdlib.h>
#include <graphics.h> 
#include <bios.h> 
#include <dos.h> 
#include <conio.h>

#define NPC 0 /*电脑玩家的序号*/
#define GRID_SIZE 30
#define MAP_SIZE 15
#define MAXBOMB 25 /*attention*/
#define TIMER 0x1c /*设置时间中断要用到*/
#define KEY_A 0x1e
#define KEY_D 0x20
#define KEY_S 0x1f
#define KEY_W 0x11
#define KEY_ESC 0x01
#define KEY_ENTER 0x1c
#define KEY_SPACE 0x39
#define KEY_UP 0x48
#define KEY_LEFT 0x4b
#define KEY_RIGHT 0x4d
#define KEY_DOWN 0x50
#define KEY_1 0x02
#define KEY_2 0x03
#define KEY_3 0x04
#define KEY_H 0x23
#define KEY_P 0x19

#define DEFAULT_LIFE 3
#define MAX_LIFE 5
#define DEFAULT_BOMB_NUM 1
#define MAX_BOMB_NUM 8
#define DEFAULT_BOMB_POWER 2
#define MAX_BOMB_POWER 50
#define LOW_SPEED 6
#define HI_SPEED 10 /*attention*/
#define RUN_SPEED 14
#define BOMB_TIME 40
#define BLAST_TIME 7
#define REVIVE_TIME 30
#define MAP_COLOR WHITE

/*****     音乐模块     *****/
#define MAXY 63 /*最大菜单背景音乐音符数*/
#define MAXM 120 /*最大游戏背景音乐音符数*/

 /*菜单的背景音乐乐谱信息*/
int freq[MAXY]={1319,1397,1568,1568,1760,1976,2094,2094,
		1319,1397,1568,1568,1760,1568,1397,1397,
		1319,1568,1047,1319,1175,1397,988,1047,
		784,1047,1175,1319,1175,1397,1319,1175,1047,988,1047,988,1047,1175,988,784,1047,1175,1319,
		1047,1175,1319,1175,1397,1319,1175,1047,0,1047,1175,1319,988,1047,988,1047,1175,988,1047,0};        
 /*菜单背景音乐每个音符的长度*/
int dely[MAXY]={3,1,4,8,3,1,4,8,
		3,1,4,8,3,1,4,8,
		4,4,4,4,4,8,4,28,
		4,6,2,4,4,4,4,4,4,2,2,2,2,4,4,4,4,4,4,
		6,2,4,4,4,4,4,4,4,4,4,4,2,2,2,2,4,4,29,16};  
 /*游戏中的背景音乐乐谱信息*/
int freq2[MAXM]={988,1047,1047,988,1047,1047,988,1047,1047,988,1047,1047,1319,1175,1047,880,1397,1319,1175,988,880,784,880,784,880,1175,1047,1175,1319,
       880,1319,1175,1047,880,1397,1319,1175,988,880,784,880,784,880,1175,1047,1047,1047,1047,784,880,1047,1047,1175,1319,1175,1319,1175,1319,1175,1047,
                   1047,784,880,1047,1047,1175,1319,1175,1319,1175,988,1047,988,1047,1047,988,1047,1047,988,1047,1047,988,1047,1047,
       880,1319,1175,1047,880,1397,1319,1175,988,880,784,880,784,880,1175,1047,1175,1319,880,1319,1175,988,880,1397,1319,1175,1047,880,784,880,784,880,1175,1047,1047,1047};
 /*游戏中的背景音乐每个音符的长度*/
int dely2[MAXM]={4,4,8,4,4,8,4,4,8,4,4,8,8,6,2,4,4,4,2,2,2,2,2,2,4,4,4,4,8,
        4,4,6,2,4,4,4,2,2,2,2,2,2,4,4,4,4,8,12,4,4,8,4,2,2,2,2,4,4,4,12,
                    12,4,4,8,4,2,2,2,2,4,4,16,4,4,8,4,4,8,4,4,8,4,4,8,
        4,4,6,2,4,4,4,2,2,2,2,2,2,4,4,4,4,8,4,4,6,2,4,4,4,2,2,2,2,2,2,4,4,4,4,8};  
int Musiccount[MAXM],mi,beginmi;/*一些控制音符的循环变量*/

/*****     地图信息     *****/
char back_a[MAP_SIZE][MAP_SIZE];
char  **a , *b[MAP_SIZE+2] , c[MAP_SIZE+2][MAP_SIZE+2]; 
/*--------  0是可以通过的   ------------------- 
0:道路    
----------  1~17是障碍，不允许通过的东西-----------
1:墙,砖块      3:岩石      5:炸弹       7:玩家0    8:玩家1
----------  18~19   --------------------------
18:爆炸图案          
-------------30-39的都是道具（有利的）------------------
31：加生命道具      32：加炸弹道具      33：加威力道具 
-------------40-49的都是道具（有害的）------------------
42：引爆所有炸弹的道具      43：换位道具        44：死亡药水道具
*/
#define THINGS_MIN 31 /*things.c中的random_thing()函数，在产生随机数时要用到*/
#define THINGS_MAX 44

char map[3][MAP_SIZE+1][MAP_SIZE]={
        /*map0*/    {
        {0,0,1,1,1,1,1,0,0,1,1,1,1,0,0},{0,1,1,0,1,3,1,3,0,1,1,0,1,3,0},
        {1,1,1,1,0,1,1,1,1,1,0,1,1,1,1},{1,3,1,1,1,1,1,3,1,3,1,3,1,3,1},
        {1,1,1,1,0,1,1,1,1,1,1,1,1,1,1},{0,0,1,3,1,3,1,3,1,0,1,0,1,0,1},
        {0,0,1,1,1,1,3,3,3,3,1,1,1,0,0},{1,1,1,0,1,3,3,3,3,3,1,3,1,1,1},
        {1,1,1,1,0,1,1,1,0,1,1,1,0,1,1},{1,0,1,3,1,3,1,3,1,3,1,3,1,0,1},
        {1,1,1,1,0,1,1,1,1,1,1,1,1,1,1},{0,1,1,1,1,3,0,0,1,1,1,3,1,1,1},
        {0,0,1,1,3,1,0,0,0,1,1,1,1,1,1},{0,0,0,0,1,1,1,1,1,1,1,1,1,1,0},
        {0,1,1,3,1,1,1,1,1,1,1,3,1,0,0},{0,0,14,14,0,0,0,0,0,0,0,0,0,0,0}},/*最后一行为基本信息，代表出生点的坐标(y0,x0)，(y1,x1)*/ 
        /*map1*/    {
        {1,1,1,1,0,0,0,1,0,0,0,1,1,1,1},{1,3,1,0,1,1,1,0,1,1,1,0,1,3,1},
        {1,1,0,1,0,1,0,1,0,1,0,1,0,1,1},{1,0,1,0,1,1,1,1,1,1,1,0,1,0,1},
        {1,0,1,1,1,1,1,1,1,1,1,1,1,0,1},{1,0,1,0,1,1,1,1,1,1,1,0,1,0,1},
        {1,0,1,1,1,1,3,3,3,1,1,1,1,0,1},{1,1,0,1,0,1,1,1,1,1,0,1,0,1,1}, 
        {1,1,1,0,1,1,1,1,1,1,1,0,1,1,1},{1,1,1,1,0,1,0,1,0,1,0,1,1,1,1},
        {1,1,0,1,1,0,1,1,1,0,1,1,0,1,1},{1,3,0,1,1,1,0,0,0,1,1,1,0,3,1},
        {1,0,0,1,1,1,1,1,1,1,1,1,1,0,1},{0,0,0,0,0,1,1,1,1,1,1,3,1,1,0},
        {0,0,1,3,1,1,3,1,1,1,3,1,1,0,0},{14,1,14,14,0,0,0,0,0,0,0,0,0,0,0}},
        /*map2*/    {        
        {0,0,1,3,1,1,1,0,0,1,1,1,1,0,0},{0,1,0,0,1,3,0,3,0,0,1,0,1,3,0},
        {1,1,1,1,3,1,1,3,3,1,1,1,1,1,1},{1,3,1,1,0,0,0,0,0,0,0,0,1,3,1},
        {1,1,1,1,0,1,1,0,0,0,1,1,1,1,1},{0,0,1,3,1,3,1,3,0,0,1,0,1,0,1},
        {0,0,1,1,0,0,3,3,3,3,1,1,1,0,0},{1,1,1,0,1,3,3,3,3,3,1,3,1,1,1},
        {1,1,1,1,0,1,1,1,0,0,1,1,0,1,1},{1,0,1,3,1,3,1,3,1,3,1,3,1,0,1},
        {1,1,1,1,0,1,1,1,1,1,1,1,1,1,1},{0,1,1,1,1,3,0,0,1,1,1,3,1,1,1},
        {0,0,1,1,3,1,0,0,0,1,1,1,1,1,1},{0,0,0,0,1,1,1,1,1,1,1,1,1,1,0},
        {0,1,1,3,1,1,1,1,1,1,1,3,1,0,0},{3,7,14,14,0,0,0,0,0,0,0,0,0,0,0}
},
};

/*===============================函数和全局变量================================*/

/*------------------ 记录人物属性的全局变量     ------------------*/
int px[2] , pi[2][2];/* 人物的当前位置。注意：x和y表示在屏幕上的坐标，i和j表示在地图上的格点坐标*/
int py[2] , pj[2][2]; 
int life[2];        /*当前剩余的生命数*/
int lifetime[2]={-3,-3};  /*-3表示关闭对此人操作  -2表示应该死亡，等待处理      -1表示还活着。≥0表示死了，并且在什么时候复活*/
int bomb_num[2];/*人物当前拥有的炸弹数*/
int bomb_power[2];/*人物当前炸弹的威力*/
int bornj[2],borni[2]; /*两个人出生点的坐标*/

/*------------------  基础模块   在main.c中  ------------------*/
    /*全局变量*/
int continuegame;/*控制游戏的持续进行。当其为0时，游戏停止，转入菜单*/
int if_play_again;/*控制游戏重新开始的开关*/
int loser=0;  /*0或1表示相应的玩家输了，3表示平局*/
int newtime;    /*全局时间，由时间中断更新。每过1/18.2秒，newtime就加一*/
int thetime;    /*用于与newtime进行比较，如果二者不等，则说明过了一个单位时间，从而执行一系列动作*/
int MODE=1;  /*游戏模式*/
int sel_map; /*选择的地图*/
     /*函数*/
int init_game(int which_map);
int gameover();
int pausegame();
int GamePlay(void);

/*------------------   菜单模块 在menu.c中  ------------------*/
void drawStar(void); /*画背景的漫天星星效果*/
void welcome(void);  /*游戏欢迎界面*/
void selectmap(void); /*选择游戏地图界面*/
void putbmp(char *bmpname); /*贴图函数*/
void selectlevel(void); /*选择游戏难度*/
int musiC(void);  /*播放菜单的背景音乐*/

/*------------------   操作模块  在key.c中   ------------------*/
    /*全局变量*/
int pause_button;/*暂停按钮。如果用户想要暂停，这个变量将被设成1并且等待处理*/
int bomb_button[2];/*炸弹按钮。如果用户想要安放炸弹，这个变量将被设成1并且等待处理*/
int handle_direction[2];/*用户当前的操作方向，其值为0不动，1上，2下，3左，4右*/
int keystate[2][5];    /*key.c专用变量，由于加工键盘传入的信息*/
int queue[2][30],qp[2];/*key.c专用变量，由于加工键盘传入的信息*/
int menu_keystate[128];/*标识每个按键的状态，游戏菜单模块需要使用的变量*/
     /*函数*/
int keypress(int who,int k);
int keyrelease(int who,int k);

/*------------------   人物控制模块   在ctrl.c中 ------------------*/
    /*全局变量*/
int fly[5][2]={{0,0},{0,-1},{0,1},{-1,0},{1,0}};  
/*(dx,dy)四个方向的偏移量。第一维度表示方向，第二维度分别为纵横坐标。（一个坐标+fly[想要的方向][相应坐标]）=移动后的坐标*/
int walk_time[2];/*此人已经连续按住键盘、持续行走了多长时间*/
int direction[2];/*此人目前的前进方向 0不动，1上，2下，3左，4右*/
     /*函数*/
int check_if_someone_lost(void);
int count_ij(int who , int y , int x , int j[] , int i[]);
int check_next(int who, int y, int x);
int move_man(int who,int dy,int dx);
int speed(int time);
int count_delta(int who,int xy);
int do_man(int who);
int if_killed_by_bomb(int who);
int die(int who);
int born(int who);

/*------------------   炸弹模块  在bomb.c中 ------------------*/
    /*全局变量*/
int bomb[MAXBOMB];
int whosebomb[MAXBOMB];
int bomb_rm[MAXBOMB];
int bombj[MAXBOMB],bombi[MAXBOMB];
int bombrange[MAXBOMB];
int bomb_blastto[MAXBOMB][5];
     /*函数*/
int set_bomb(int who);
int serial_blast(int j,int i);
int blast(int p);
int remove_blast(int p);

/*------------------    AI模块  在AI.C中   ------------------*/
    /*全局变量*/
int exroad=0;                /*记录AI当前是否在执行一条道路*/
int gamelevel;      /*AI的难度*/
int ai_sbb;   /*AI放炸弹的变量为1表示AI将要执行放炸弹的功能*/
     /*函数*/
int danger(int cx,int cy,int t);  /*判断编号为t的炸弹对(cx,cy)点是否有威胁*/
int potential(int cx,int cy,int j,int t); /*判断与玩家(cx,cy)点同行或同列的炸弹是否是潜在威胁*/
void AI_OP(int b);        /*AI接口操作函数*/
int AI_Search(int t);     /*AI的搜索函数，功能是寻找一条到达指定位置的道路，其算法是一个A*寻路算法*/
void AI_Move(int t);    /*该函数控制AI的移动*/
int AI_danger(int cx,int cy);   /*判断AI在(cx,cy)位置是否危险*/
void calc_pos(void);   /*计算AI当前所在的格子坐标*/
int cov(int x,int y);  /*判断AI在目前的道路上是否被阻挡*/
void AI_Reset(void); /*重置AI的路径信息*/

/*------------------   中断函数   在inter.c中 ------------------*/
void interrupt newTimer();
void setTimer(void interrupt (*IntProc)());
void killTimer();
void far interrupt NewInt9(void);
void interrupt far (*OldInt9Handler)();      /*键盘中断程序*/
void InstallKeyboard(void);
void ShutDownKeyboard(void);

/*------------------    图形模块， 在graph.c文件中 ------------------*/
int xy_to_ij(int xy);
int ij_to_xy(int ij);
void installGraph(void);
int draw_UI(int mode);
int draw_map(void);
int draw_player(int who,int y,int x);
int draw_road(int y,int x);
int draw_wall(int y,int x);
int draw_stone(int y,int x);
int draw_bomb(int y,int x);
int draw_blast(int y,int x);
int draw_pluslife(int y,int x);
int draw_plusbomb(int y,int x);
int draw_pluspower(int y,int x);
int draw_blastall(int y,int x);
int draw_exchange(int y,int x);
int draw_deathpotion(int y,int x);
int draw(int j,int i,int state);
int animat_bomb(void);
int write_life(int who );
int write_bomb_num(int who);
int write_bomb_power(int who);

/*------------------    道具模块   在things.c中 ------------------*/
int exchange2nums(int *a , int * b);
int random_thing();
int pluslife(int who);
int plusbomb(int who);
int pluspower(int who);
int blastall();
int exchange();
int deathpotion(int who);
int things(int who,int j,int i);
