#include <stdio.h>
#include <stdlib.h>
#include <graphics.h> 
#include <bios.h> 
#include <dos.h> 
#include <conio.h>

#define NPC 0 /*������ҵ����*/
#define GRID_SIZE 30
#define MAP_SIZE 15
#define MAXBOMB 25 /*attention*/
#define TIMER 0x1c /*����ʱ���ж�Ҫ�õ�*/
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

/*****     ����ģ��     *****/
#define MAXY 63 /*���˵���������������*/
#define MAXM 120 /*�����Ϸ��������������*/

 /*�˵��ı�������������Ϣ*/
int freq[MAXY]={1319,1397,1568,1568,1760,1976,2094,2094,
		1319,1397,1568,1568,1760,1568,1397,1397,
		1319,1568,1047,1319,1175,1397,988,1047,
		784,1047,1175,1319,1175,1397,1319,1175,1047,988,1047,988,1047,1175,988,784,1047,1175,1319,
		1047,1175,1319,1175,1397,1319,1175,1047,0,1047,1175,1319,988,1047,988,1047,1175,988,1047,0};        
 /*�˵���������ÿ�������ĳ���*/
int dely[MAXY]={3,1,4,8,3,1,4,8,
		3,1,4,8,3,1,4,8,
		4,4,4,4,4,8,4,28,
		4,6,2,4,4,4,4,4,4,2,2,2,2,4,4,4,4,4,4,
		6,2,4,4,4,4,4,4,4,4,4,4,2,2,2,2,4,4,29,16};  
 /*��Ϸ�еı�������������Ϣ*/
int freq2[MAXM]={988,1047,1047,988,1047,1047,988,1047,1047,988,1047,1047,1319,1175,1047,880,1397,1319,1175,988,880,784,880,784,880,1175,1047,1175,1319,
       880,1319,1175,1047,880,1397,1319,1175,988,880,784,880,784,880,1175,1047,1047,1047,1047,784,880,1047,1047,1175,1319,1175,1319,1175,1319,1175,1047,
                   1047,784,880,1047,1047,1175,1319,1175,1319,1175,988,1047,988,1047,1047,988,1047,1047,988,1047,1047,988,1047,1047,
       880,1319,1175,1047,880,1397,1319,1175,988,880,784,880,784,880,1175,1047,1175,1319,880,1319,1175,988,880,1397,1319,1175,1047,880,784,880,784,880,1175,1047,1047,1047};
 /*��Ϸ�еı�������ÿ�������ĳ���*/
int dely2[MAXM]={4,4,8,4,4,8,4,4,8,4,4,8,8,6,2,4,4,4,2,2,2,2,2,2,4,4,4,4,8,
        4,4,6,2,4,4,4,2,2,2,2,2,2,4,4,4,4,8,12,4,4,8,4,2,2,2,2,4,4,4,12,
                    12,4,4,8,4,2,2,2,2,4,4,16,4,4,8,4,4,8,4,4,8,4,4,8,
        4,4,6,2,4,4,4,2,2,2,2,2,2,4,4,4,4,8,4,4,6,2,4,4,4,2,2,2,2,2,2,4,4,4,4,8};  
int Musiccount[MAXM],mi,beginmi;/*һЩ����������ѭ������*/

/*****     ��ͼ��Ϣ     *****/
char back_a[MAP_SIZE][MAP_SIZE];
char  **a , *b[MAP_SIZE+2] , c[MAP_SIZE+2][MAP_SIZE+2]; 
/*--------  0�ǿ���ͨ����   ------------------- 
0:��·    
----------  1~17���ϰ���������ͨ���Ķ���-----------
1:ǽ,ש��      3:��ʯ      5:ը��       7:���0    8:���1
----------  18~19   --------------------------
18:��ըͼ��          
-------------30-39�Ķ��ǵ��ߣ������ģ�------------------
31������������      32����ը������      33������������ 
-------------40-49�Ķ��ǵ��ߣ��к��ģ�------------------
42����������ը���ĵ���      43����λ����        44������ҩˮ����
*/
#define THINGS_MIN 31 /*things.c�е�random_thing()�������ڲ��������ʱҪ�õ�*/
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
        {0,1,1,3,1,1,1,1,1,1,1,3,1,0,0},{0,0,14,14,0,0,0,0,0,0,0,0,0,0,0}},/*���һ��Ϊ������Ϣ�����������������(y0,x0)��(y1,x1)*/ 
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

/*===============================������ȫ�ֱ���================================*/

/*------------------ ��¼�������Ե�ȫ�ֱ���     ------------------*/
int px[2] , pi[2][2];/* ����ĵ�ǰλ�á�ע�⣺x��y��ʾ����Ļ�ϵ����꣬i��j��ʾ�ڵ�ͼ�ϵĸ������*/
int py[2] , pj[2][2]; 
int life[2];        /*��ǰʣ���������*/
int lifetime[2]={-3,-3};  /*-3��ʾ�رնԴ��˲���  -2��ʾӦ���������ȴ�����      -1��ʾ�����š���0��ʾ���ˣ�������ʲôʱ�򸴻�*/
int bomb_num[2];/*���ﵱǰӵ�е�ը����*/
int bomb_power[2];/*���ﵱǰը��������*/
int bornj[2],borni[2]; /*�����˳����������*/

/*------------------  ����ģ��   ��main.c��  ------------------*/
    /*ȫ�ֱ���*/
int continuegame;/*������Ϸ�ĳ������С�����Ϊ0ʱ����Ϸֹͣ��ת��˵�*/
int if_play_again;/*������Ϸ���¿�ʼ�Ŀ���*/
int loser=0;  /*0��1��ʾ��Ӧ��������ˣ�3��ʾƽ��*/
int newtime;    /*ȫ��ʱ�䣬��ʱ���жϸ��¡�ÿ��1/18.2�룬newtime�ͼ�һ*/
int thetime;    /*������newtime���бȽϣ�������߲��ȣ���˵������һ����λʱ�䣬�Ӷ�ִ��һϵ�ж���*/
int MODE=1;  /*��Ϸģʽ*/
int sel_map; /*ѡ��ĵ�ͼ*/
     /*����*/
int init_game(int which_map);
int gameover();
int pausegame();
int GamePlay(void);

/*------------------   �˵�ģ�� ��menu.c��  ------------------*/
void drawStar(void); /*����������������Ч��*/
void welcome(void);  /*��Ϸ��ӭ����*/
void selectmap(void); /*ѡ����Ϸ��ͼ����*/
void putbmp(char *bmpname); /*��ͼ����*/
void selectlevel(void); /*ѡ����Ϸ�Ѷ�*/
int musiC(void);  /*���Ų˵��ı�������*/

/*------------------   ����ģ��  ��key.c��   ------------------*/
    /*ȫ�ֱ���*/
int pause_button;/*��ͣ��ť������û���Ҫ��ͣ����������������1���ҵȴ�����*/
int bomb_button[2];/*ը����ť������û���Ҫ����ը������������������1���ҵȴ�����*/
int handle_direction[2];/*�û���ǰ�Ĳ���������ֵΪ0������1�ϣ�2�£�3��4��*/
int keystate[2][5];    /*key.cר�ñ��������ڼӹ����̴������Ϣ*/
int queue[2][30],qp[2];/*key.cר�ñ��������ڼӹ����̴������Ϣ*/
int menu_keystate[128];/*��ʶÿ��������״̬����Ϸ�˵�ģ����Ҫʹ�õı���*/
     /*����*/
int keypress(int who,int k);
int keyrelease(int who,int k);

/*------------------   �������ģ��   ��ctrl.c�� ------------------*/
    /*ȫ�ֱ���*/
int fly[5][2]={{0,0},{0,-1},{0,1},{-1,0},{1,0}};  
/*(dx,dy)�ĸ������ƫ��������һά�ȱ�ʾ���򣬵ڶ�ά�ȷֱ�Ϊ�ݺ����ꡣ��һ������+fly[��Ҫ�ķ���][��Ӧ����]��=�ƶ��������*/
int walk_time[2];/*�����Ѿ�������ס���̡����������˶೤ʱ��*/
int direction[2];/*����Ŀǰ��ǰ������ 0������1�ϣ�2�£�3��4��*/
     /*����*/
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

/*------------------   ը��ģ��  ��bomb.c�� ------------------*/
    /*ȫ�ֱ���*/
int bomb[MAXBOMB];
int whosebomb[MAXBOMB];
int bomb_rm[MAXBOMB];
int bombj[MAXBOMB],bombi[MAXBOMB];
int bombrange[MAXBOMB];
int bomb_blastto[MAXBOMB][5];
     /*����*/
int set_bomb(int who);
int serial_blast(int j,int i);
int blast(int p);
int remove_blast(int p);

/*------------------    AIģ��  ��AI.C��   ------------------*/
    /*ȫ�ֱ���*/
int exroad=0;                /*��¼AI��ǰ�Ƿ���ִ��һ����·*/
int gamelevel;      /*AI���Ѷ�*/
int ai_sbb;   /*AI��ը���ı���Ϊ1��ʾAI��Ҫִ�з�ը���Ĺ���*/
     /*����*/
int danger(int cx,int cy,int t);  /*�жϱ��Ϊt��ը����(cx,cy)���Ƿ�����в*/
int potential(int cx,int cy,int j,int t); /*�ж������(cx,cy)��ͬ�л�ͬ�е�ը���Ƿ���Ǳ����в*/
void AI_OP(int b);        /*AI�ӿڲ�������*/
int AI_Search(int t);     /*AI������������������Ѱ��һ������ָ��λ�õĵ�·�����㷨��һ��A*Ѱ·�㷨*/
void AI_Move(int t);    /*�ú�������AI���ƶ�*/
int AI_danger(int cx,int cy);   /*�ж�AI��(cx,cy)λ���Ƿ�Σ��*/
void calc_pos(void);   /*����AI��ǰ���ڵĸ�������*/
int cov(int x,int y);  /*�ж�AI��Ŀǰ�ĵ�·���Ƿ��赲*/
void AI_Reset(void); /*����AI��·����Ϣ*/

/*------------------   �жϺ���   ��inter.c�� ------------------*/
void interrupt newTimer();
void setTimer(void interrupt (*IntProc)());
void killTimer();
void far interrupt NewInt9(void);
void interrupt far (*OldInt9Handler)();      /*�����жϳ���*/
void InstallKeyboard(void);
void ShutDownKeyboard(void);

/*------------------    ͼ��ģ�飬 ��graph.c�ļ��� ------------------*/
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

/*------------------    ����ģ��   ��things.c�� ------------------*/
int exchange2nums(int *a , int * b);
int random_thing();
int pluslife(int who);
int plusbomb(int who);
int pluspower(int who);
int blastall();
int exchange();
int deathpotion(int who);
int things(int who,int j,int i);