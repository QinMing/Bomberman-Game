/*===================�ļ�MAIN.C=======================
���������������Ϸ��ʼ�������С���ͣ�������ĺ���
----------------------------------------------------*/ 
#include "define.c"
#include "key.c"
#include "graph.c"
#include "bomb.c"
#include "ctrl.c"
#include "things.c"
#include "inter.c" 
#include "menu.c"
#include "AI.c"


/*=====================================================
����ԭ�ͣ�int init_game(int which_map);
���ܣ���ʼ����Ϸ����һ����ر�����Ϊ��ʼֵ��Ȼ���ȡ��ͼ��Ϣ��
��ڣ���ͼ���
������ϵ����GamePlay()����
------------------------------------------------------*/ 
int init_game(int which_map)
{
    int i,j;
    srand((unsigned long)time(NULL));
    newtime=0;
    loser=0;
    life[0] = life[1] = DEFAULT_LIFE;
    lifetime[0] = lifetime[1] = -1;
    bomb_num[0] = bomb_num[1] = DEFAULT_BOMB_NUM;
    bomb_power[0] = bomb_power[1] = DEFAULT_BOMB_POWER; 

    pause_button = 0;
    bomb_button[0] = bomb_button[1] = 0;
    handle_direction[0] = handle_direction[1] = 0;
    queue[0][0] = 0;
    queue[1][0] = 0;
    qp[0] = qp[1] = 0;
    for(i=0;i<2;i++){
        for(j=0;j<5;j++){
            keystate[i][j] = 0;
        }
    }

    walk_time[0] = walk_time[1] = 0;
    direction[0] = direction[1] = 0;
    for(i=0;i<MAXBOMB;i++){
        bomb[i]=-1;
        bomb_rm[i]=-1;
    }

    /* c�Ǳȵ�ͼ��Ķ�ά���飬b��һά��ָ�����飬a��һ��ָ��ָ���ָ�� */
	/* �ȸ�b��ֵΪc�ĵڶ��е�Ԫ�صĵ�ַ��Ȼ���a��ֵΪb�ĵڶ���Ԫ�صĵ�ַ*/
	/*����һ�����Ϳ��Եõ�һ���µĶ�ά����a�������ص��ǣ�a[-1][..]��a[..][-1]Ҳ����Ч��*/
	/*��������Ŀ���ǣ�����ͼ������һȦ���3��Ҳ���Ǽ᲻�ɴݵ�ǽ�������˺�ը����ը������ͼ*/
    for(i=0 ; i<MAP_SIZE+2 ; i++){
        b[i] = c[i] + 1;
    }
    a = b + 1;
    for(i=-1 ; i<MAP_SIZE+1 ; i++){
        a[-1][i] = 3;
        a[MAP_SIZE][i] = 3;
        a[i][-1] = 3;
        a[i][MAP_SIZE] = 3;
    }
    /* -----------��ȡ��ͼ---------------*/
    for(j=0;j<MAP_SIZE;j++){
        for(i=0;i<MAP_SIZE;i++){
            a[j][i]=map[which_map][j][i];
        }
    }

    bornj[0] = map[which_map][15][0];
    borni[0] = map[which_map][15][1];
    bornj[1] = map[which_map][15][2];
    borni[1] = map[which_map][15][3];

    return 0;
}

/*=====================================================
����ԭ�ͣ�int gameover();
���ܣ���Ϸ����
������ϵ������⵽ loser > 0 ʱ����GamePlay(void)����
------------------------------------------------------*/ 
int gameover(){
    int i,m,n;
    int key;
    char losestring[100];
    nosound();
    /*killTimer();��Ҫɱ�����κ㣡������*/
    for (i=1;i<600;i++) delay(1000);
    cleardevice();
    setbkcolor(BLACK);
    drawStar();
    setcolor(LIGHTMAGENTA);
    settextstyle(1,0,4);
    if (MODE==1){
        if (loser==10) sprintf(losestring,"    Player1 Lost!!!");
        if (loser==11) sprintf(losestring,"    Player2 Lost!!!");
        if (loser==20) sprintf(losestring,"    It's  a Draw!!!");   
    }
    if (MODE==0) {
        if (loser==10) sprintf(losestring,"    You Win!!!");
        if (loser==11) sprintf(losestring,"    You Lose!!!");
        if (loser==20) sprintf(losestring,"    It's  a Draw!!!");   
    }
    outtextxy(150,170,losestring);
    settextstyle(1,0,2);
    outtextxy(160,350,"Press ESC to quit, ENTER to replay");
    for (i=0;i<128;i++) menu_keystate[i]=0;
    do  {
    }while ((!menu_keystate[0x1c])&&(!menu_keystate[0x01]));
    if (menu_keystate[0x1c]) 
        if_play_again = 1;
    else 
        if_play_again = 0;
    continuegame = 0;
    return 0;
}

/*=====================================================
����ԭ�ͣ�int pausegame();
���ܣ���ͣ��Ϸ
������ϵ������⵽ (pause_button == 1) ʱ����GamePlay(void)����
------------------------------------------------------*/ 
int pausegame(){
    static short int now_state=0;/*���ڵ�״̬��0������Ϸ�С�����1������ͣ�С�*/
    static int newtime_save;
    if (now_state == 0){
        newtime_save = newtime;
        thetime = 30001;
        lifetime[0] = lifetime[1] = -3;
        now_state = 1;
    }else{
        newtime = thetime = newtime_save;
        lifetime[0] = lifetime[1] = -1;
        now_state = 0;
    }
    nosound();
    return 0;
}

/*������Ϸ�ĺ�����*/ 
int GamePlay(void){
    int i;

    welcome();
    init_game(sel_map);
    draw_UI( MODE );
    draw_map();

    memset(Musiccount,sizeof(Musiccount),0);       /*��ʼ������*/
    mi=0;
    sound(freq2[mi]);                                                  /*��ʼ��Ϸ�������ֵ�һ������*/
    thetime = newtime;
    continuegame=1;
    while(continuegame){
        if (Musiccount[mi]>=dely2[mi]*1.5)                             /*��ʱ���Ƶ��������������㹻ʱ������һ������*/
        {
            nosound();Musiccount[mi]=0;mi++; sound(freq2[mi]);
        }
        if (mi==MAXM) mi=0;                                           /*���ַ����ˣ���ͷ����*/
        if (newtime >= (thetime+1) % 30000){
            if (lifetime[0] == -1) do_man(0);/*�������*/
            if (lifetime[1] == -1) do_man(1);

            if( if_killed_by_bomb(0) )  die(0); 
            if( if_killed_by_bomb(1) )  die(1);
            if ((lifetime[0]>=0)&&(newtime >= lifetime[0] ))   born(0);
            if ((lifetime[1]>=0)&&(newtime >= lifetime[1] ))   born(1);

            for (i=0;i<MAXBOMB;i++){
                if((bomb_rm[i]>=0)&&(newtime >= bomb_rm[i]))
                    remove_blast(i);
        	}
            for (i=0;i<MAXBOMB;i++){
                if((bomb[i]>=0)&&(newtime >= bomb[i]))
                    blast(i);
        	}
            if (newtime%2==0)
				animat_bomb();

            thetime=newtime;
        }
        if(MODE==0) {/*��AI����˼��*/
            if (lifetime[0]==-1) AI_OP(bomb_num[0]);
            else if (lifetime[0]!=-1) AI_Reset();
        }
        if ((MODE!=0)&&(lifetime[0] == -1)&&(bomb_button[0] == 1)){
            set_bomb(0);
            bomb_button[0] = 0;
        }
        if ((lifetime[1] == -1)&&(bomb_button[1] == 1)){
            set_bomb(1);
            bomb_button[1] = 0;
        }
        if (pause_button == 1){
            pausegame();
            pause_button = 0;
            while (pause_button == 0 ) { }
            pausegame();
            pause_button = 0;
        }
        if( loser > 0 ){
            lifetime[0] = -3;
            lifetime[1] = -3;
            gameover();
        }
    }/* end of "while(continuegame)" */
    return 0; 
}

int main()
{
    installGraph();
    setTimer(newTimer);
    InstallKeyboard();
    do{
        GamePlay();
    }while(if_play_again);
    killTimer();
    ShutDownKeyboard();
    return 0;
}