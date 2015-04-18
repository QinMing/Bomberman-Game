/*===================�ļ�graph.c=======================
���ͼ����ʾģ��ĺ���
----------------------------------------------------*/ 


/*=====================================================
����ԭ�ͣ�int xy_to_ij(int xy)
���ܣ�����Ļ�ϵ����� ת���� ��ͼ�ϵĸ�����ꡣ
��ڣ���Ļ�ϵ�����y����x��
���ڣ���ͼ�ϵĸ������j����i��
������ϵ�����ܶ�ģ����Ϊ���ߺ���ʹ��
------------------------------------------------------*/ 
int xy_to_ij(int xy){
	if (xy<10) return -1;  
	else return ((xy-10)/GRID_SIZE);
}

/*=====================================================
����ԭ�ͣ�int ij_to_xy(int ij)
���ܣ������溯���෴
��ڣ���ͼ�ϵĸ������j����i��
���ڣ���Ļ�ϵ�����y����x��
������ϵ�����ܶ�ģ����Ϊ���ߺ���ʹ��
------------------------------------------------------*/ 
int ij_to_xy(int ij){
	return 10+ij*GRID_SIZE;
}

/*ͼ��ģʽ��ʼ��*/ 
void installGraph(void)           
{
	int grdriver=DETECT;
	int grmode;
	int errorcode;
	char *errormsg;
	initgraph(&grdriver,&grmode,"");
	errorcode=graphresult();
	errormsg=grapherrormsg(errorcode);
	if(errorcode!=grOk)
	{
		printf("Graphics error:%s\n",errormsg);
		printf("You should check the path of BGIdriver\n");
		printf("Please input the right path in the function \"InstallGraph()\" and try again.\n");
		printf("Press any key to exit.\n");
		delay(5000);
		exit(1);
	}
	return;
}

/*=====================================================
����ԭ�ͣ�int draw(int j,int i,int state);
���ܣ�����һ�������ĸ��ӡ�����ͼ��ģ�����ں���
��ڣ����ӵ������У�j,i��  �� Ҫ���ƵĶ���
���ڣ����ñ��ļ���һЩ�ԡ�draw_��Ϊǰ׺�ĺ���
������ϵ����Ϊͼ��ģ�����ں���������bomb.c��ctrl.c���ļ��У���Ҫ�����ʾ�ĵط��������á�
------------------------------------------------------*/ 
int draw(int j,int i,int state)
{
	int x,y;

	y=ij_to_xy(j);
	x=ij_to_xy(i);
	switch(state){
	case 0: 
	case 7: 
	case 8:
		draw_road(y,x);break;
	case 1: draw_wall(y,x);break;
	case 3: draw_stone(y,x);break;
	case 5: draw_bomb(y,x);break;
	case 18: draw_blast(y,x);break;
	case 31: draw_pluslife(y,x);break;
	case 32: draw_plusbomb(y,x);break;
	case 33: draw_pluspower(y,x);break;
	case 42: draw_blastall(y,x);break;
	case 43: draw_exchange(y,x);break;
	case 44: draw_deathpotion(y,x);break;
	}
	return 0;
}

/*������Ϸ���棬��Ҫ���ұߵ���Ϣ��ʾ���*/ 
int draw_UI(int mode){
	cleardevice();
	setfillstyle(1,LIGHTBLUE);
	bar(0,0,640,480);
	settextstyle(1,0,1);
	setcolor(YELLOW);
	if (mode==1) {
		outtextxy(480,15,"Player1: ");
		settextstyle(2,0,6);
		outtextxy(480,35,"Life: ");
		outtextxy(480,65,"Bomb number: ");
		outtextxy(480,95,"Bomb power: ");
		settextstyle(1,0,1);
		outtextxy(480,250,"Player2: ");
		settextstyle(2,0,6);
		outtextxy(480,270,"Life: ");
		outtextxy(480,300,"Bomb number: ");
		outtextxy(480,330,"Bomb power: ");
	}
	else {
		outtextxy(470,15,"Enemy: ");
		settextstyle(2,0,6);
		outtextxy(480,35,"Life: ");
		outtextxy(480,65,"Bomb number: ");
		outtextxy(480,95,"Bomb power: ");
		settextstyle(1,0,1);
		outtextxy(480,250,"You: ");
		settextstyle(2,0,6);
		outtextxy(480,270,"Life: ");
		outtextxy(480,300,"Bomb number: ");
		outtextxy(480,330,"Bomb power: ");
	}   
	write_life(0);
	write_life(1);
	write_bomb_num(0);
	write_bomb_num(1);
	write_bomb_num(0);
	write_bomb_power(0);
	write_bomb_power(1);
	setcolor(LIGHTGREEN);
	settextstyle(2,0,6);
	if (mode==1) {
		outtextxy(480,135,"   UP   :   W ");
		outtextxy(480,155,"  DOWN  :   S ");
		outtextxy(480,175,"  LEFT  :   A ");
		outtextxy(480,195,"  RIGHT :   D ");
		outtextxy(480,215,"PUT BOMB: SPACE");
	}
	outtextxy(480,355,"   UP   :   UP ");
	outtextxy(480,375,"  DOWN  :  DOWN");
	outtextxy(480,395,"  LEFT  :  LEFT");
	outtextxy(480,415,"  RIGHT : RIGHT");
	outtextxy(480,435,"PUT BOMB: ENTER");

	return 0;
}

/*������Ϸ��������Ҫ����ǽ��·���*/ 
int draw_map(void){
	int i,j;
	setcolor(BLUE);
	setbkcolor(WHITE);
	rectangle(9,9,461,461);/*��ͼ��� */
	for(j=0;j<MAP_SIZE;j++){
		for(i=0;i<MAP_SIZE;i++){
			draw(j,i,a[j][i]);
		}
	}
	born(0);
	born(1);
	return 0;
}

/*����·��*/ 
int draw_road(int y,int x)
{
	setfillstyle(1,MAP_COLOR);
	bar(x,y,x+29,y+29);
	return 0;
}

/*����שǽ*/ 
int draw_wall(int y,int x)                  
{
	int i;
	setfillstyle(1,RED);
	bar(x,y,x+29,y+29);
	setcolor(WHITE);
	rectangle(x+1,y+1,x+10,y+8);
	rectangle(x+10,y+1,x+20,y+8);
	rectangle(x+20,y+1,x+29,y+8);

	rectangle(x+1,y+8,x+6,y+15);
	rectangle(x+6,y+8,x+15,y+15);
	rectangle(x+15,y+8,x+24,y+15);
	rectangle(x+24,y+8,x+29,y+15);

	rectangle(x+1,y+15,x+10,y+22);
	rectangle(x+10,y+15,x+20,y+22);
	rectangle(x+20,y+15,x+29,y+22);

	rectangle(x+1,y+22,x+6,y+29);
	rectangle(x+6,y+22,x+15,y+29);
	rectangle(x+15,y+22,x+24,y+29);
	rectangle(x+24,y+22,x+29,y+29);
	return 0;
}

/*���Ƽ᲻�ɴݵ�ǽ*/ 
int draw_stone(int y,int x)             
{
	setfillstyle(1,MAP_COLOR);
	bar(x,y,x+29,y+29);
	setfillstyle(6,BROWN);
	bar(x+1,y+1,x+28,y+28);
	return 0;
}

/*=====================================================
����ԭ�ͣ�int draw_player(int who,int y,int x);
���ܣ����ˡ�������ʱ��Ĳ�ͬ���������ֺͽ�Ҳ��ͬ��ʵ�ֶ���
��ڣ��������who������Ļ�ϵ����꣨y,x��
���ڣ��޷���ֵ��
������ϵ����Ҫ��ctrl.c�е�move_man()�������ã��Բ����ػ����ʵ�ֶ���
------------------------------------------------------*/ 
int draw_player(int who,int y,int x){
	setcolor(LIGHTGRAY);
	circle(x+15,y+6,5);
	if(who==0) setfillstyle(1,YELLOW);
	else if(who==1) setfillstyle(1,GREEN);
	else return 0;
	floodfill(x+15,y+5,LIGHTGRAY);

	putpixel(x+13,y+5,RED);
	putpixel(x+17,y+5,RED);
	moveto(x+15,y+12);
	lineto(x+15,y+19);
	if (thetime % 8 <= 3){
		/*��*/
		line(x+15,y+12,x+8,y+19);
		line(x+15,y+12,x+22,y+19);
		/*��*/
		line(x+15,y+20,x+8,y+28);
		line(x+15,y+20,x+22,y+28);
	}else{
		/*��*/
		line(x+15,y+12,x+10,y+20);
		line(x+15,y+12,x+20,y+20);
		/*��*/
		line(x+15,y+20,x+12,y+29);
		line(x+15,y+20,x+18,y+29);
	}
	return 0;
}

/*=====================================================
����ԭ�ͣ�int draw_bomb(int y,int x);
���ܣ�����ը����������ʱ��Ĳ�ͬ������ͬ����״��ʵ��ը���Ķ���
������ϵ�������ļ��� draw()ֱ�ӵ��á�����Ϸ�����У�����animat_bomb()������ͨ��draw()������ӵ���
------------------------------------------------------*/ 
int  draw_bomb(int y,int x)    
{
	setfillstyle(1,MAP_COLOR);
	bar(x,y,x+29,y+29);
	setcolor(LIGHTCYAN);
	setfillstyle(1,LIGHTCYAN);

	switch(thetime / 2 % 6){
	case 0:
		ellipse(x+15,y+13,0,360,13,12);
		floodfill(x+15,y+13,LIGHTCYAN);
		setcolor(WHITE);
		setfillstyle(1,WHITE);
		pieslice(x+19,y+8,0,360,5);
		break;
	case 1: case 5:
		ellipse(x+15,y+14,0,360,13,12);
		floodfill(x+15,y+14,LIGHTCYAN);
		setcolor(WHITE);
		setfillstyle(1,WHITE);
		pieslice(x+19,y+9,0,360,5);
		break;
	case 2: case 4:
		ellipse(x+15,y+15,0,360,13,11);
		floodfill(x+15,y+15,LIGHTCYAN);
		setcolor(WHITE);
		setfillstyle(1,WHITE);
		pieslice(x+19,y+10,0,360,5);
		break;
	case 3:
		ellipse(x+15,y+16,0,360,13,10);
		floodfill(x+15,y+16,LIGHTCYAN);
		setcolor(WHITE);
		setfillstyle(1,WHITE);
		pieslice(x+19,y+11,0,360,5);
		break;
	}
	return 0;
}

/*���Ʊ�ըͼ��*/ 
int draw_blast(int y,int x)
{
	setfillstyle(1,YELLOW);
	bar(x,y,x+29,y+29);
	return 0;
}

/*���Ƽ���������*/ 
int draw_pluslife(int y,int x)
{
	setfillstyle(1,MAP_COLOR);
	bar(x,y,x+29,y+29);
	setcolor(RED);
	rectangle(x+1,y+1,x+29,y+29);
	setcolor(LIGHTRED);
	settextstyle(1,0,3);
	outtextxy(x+2,y+1,"+1");
	return 0;
}

/*���Ƽ�ը������*/ 
int draw_plusbomb(int y,int x)
{
	setfillstyle(1,MAP_COLOR);
	bar(x,y,x+29,y+29);
	/*setcolor(WHITE);
	rectangle(x+1,y+1,x+29,y+29);
	floodfill(x+10,y+10,WHITE);*/
	setcolor(LIGHTCYAN);
	circle(x+12,y+12,10);
	setfillstyle(1,LIGHTCYAN);
	floodfill(x+12,y+12,LIGHTCYAN);
	setcolor(LIGHTBLUE);
	circle(x+21,y+21,6);
	setfillstyle(1,LIGHTBLUE);
	floodfill(x+24,y+24,LIGHTBLUE);
	return 0;
}

/*���Ƽ�ը����������*/ 
int draw_pluspower(int y,int x)
{
	int a[8]={12,10,12,13,8,16,8,27}; /* һЩͼ��������λ�� */
	int b[8]={18,10,18,13,22,16,22,27};
	int i;

	setfillstyle(1,MAP_COLOR);
	bar(x,y,x+29,y+29);
	setcolor(BLACK);
	rectangle(x+1,y+1,x+29,y+29);
	setfillstyle(1,LIGHTRED);
	ellipse(x+15,y+7,0,360,4,2);
	floodfill(x+15,y+7,BLACK);
	for(i=0;i<6;i+=2)
	{
		moveto(x+a[i],y+a[i+1]);
		lineto(x+a[i+2],y+a[i+3]);
		moveto(x+b[i],y+b[i+1]);
		lineto(x+b[i+2],y+b[i+3]);
	}
	moveto(x+a[6],y+a[7]);
	lineto(x+b[6],y+b[7]);
	setfillstyle(1,LIGHTGREEN);
	floodfill(x+15,y+15,BLACK);
	setfillstyle(10,BLACK);
	floodfill(x+1,y+1,BLACK);
	return 0;
}

/*���ơ���������ը��������*/
int draw_blastall(int y,int x)
{
	setfillstyle(1,MAP_COLOR);
	bar(x,y,x+29,y+29);
	setcolor(WHITE);
	rectangle(x+1,y+1,x+29,y+29);
	setcolor(LIGHTCYAN);
	setfillstyle(1,LIGHTCYAN);
	pieslice(x+15,y+15,0,360,11);
	setcolor(WHITE);
	setfillstyle(1,WHITE);
	pieslice(x+16,y+11,0,360,4);
	setcolor(LIGHTBLUE);
	setfillstyle(1,LIGHTBLUE);
	bar(x+12,y+3,x+18,y+27);
	setfillstyle(1,LIGHTBLUE);
	bar(x+3,y+12,x+27,y+18);
	return 0;
}

/*���ơ���λ������*/
int draw_exchange(int y,int x)
{
	setfillstyle(1,MAP_COLOR);
	bar(x,y,x+29,y+29);
	setcolor(LIGHTMAGENTA);
	rectangle(x+1,y+1,x+29,y+29);
	setcolor(MAGENTA);
	moveto(x+28,y+2);
	lineto(x+11,y+11);
	lineto(x+18,y+18);
	lineto(x+2,y+28);
	moveto(x+27,y+2);
	lineto(x+10,y+11);
	lineto(x+17,y+18);
	lineto(x+2,y+27);
	moveto(x+28,y+3);
	lineto(x+12,y+11);
	lineto(x+19,y+18);
	lineto(x+3,y+28);
	return 0;
}

/*���ơ�����ҩˮ������*/
int draw_deathpotion(int y,int x)
{
	int a[8]={12,10,12,13,8,16,8,27};     /* һЩͼ��������λ�� */
	int b[8]={18,10,18,13,22,16,22,27};
	int i;
	setfillstyle(1,MAP_COLOR);
	bar(x,y,x+29,y+29);
	setcolor(BLACK);
	rectangle(x+1,y+1,x+29,y+29);
	setfillstyle(1,LIGHTGRAY);
	ellipse(x+15,y+7,0,360,4,2);
	floodfill(x+15,y+7,BLACK);
	for(i=0;i<6;i+=2)
	{
		moveto(x+a[i],y+a[i+1]);
		lineto(x+a[i+2],y+a[i+3]);
		moveto(x+b[i],y+b[i+1]);
		lineto(x+b[i+2],y+b[i+3]);
	}
	moveto(x+a[6],y+a[7]);
	lineto(x+b[6],y+b[7]);
	setfillstyle(1,DARKGRAY);
	floodfill(x+15,y+15,BLACK);
	setfillstyle(10,BLACK);
	floodfill(x+1,y+1,BLACK);
	moveto(x+2,y+2);
	setcolor(LIGHTRED);
	lineto(x+28,y+28);
	moveto(x+2,y+28);
	lineto(x+28,y+2);
	return 0;
}

/*=====================================================
����ԭ�ͣ�int animat_bomb(void);
���ܣ�ʵ��ը���Ķ���Ч������ը������������һ���е��Ե���
������ϵ������GamePlay()���á�����ͨ��draw()����ӵ���draw_bomb()
------------------------------------------------------*/ 
int animat_bomb(void)
{
	int i;
	for(i=0;i<MAXBOMB;i++)
		if(bomb[i] >= 0){
			draw(bombj[i] , bombi[i] , 5);
		}
		return 0;
}

/*��ʾ��ǰ����ֵ*/ 
int write_life(int who )
{
	char s[3];
	sprintf(s,"%d",life[who]);

	setcolor(LIGHTRED);
	setfillstyle(SOLID_FILL,LIGHTBLUE);
	settextstyle(1,0,1);
	if (who==0) {
		switch (life[who]) {
		case 0:bar(480,55,628,65); break;
		case 1:bar(480,55,628,65); setfillstyle(SOLID_FILL,RED); bar(480,55,510,65); break;
		case 2: bar(480,55,628,65); setfillstyle(SOLID_FILL,YELLOW); bar(480,55,540,65); break;
		case 3: bar(480,55,628,65); setfillstyle(SOLID_FILL,GREEN); bar(480,55,570,65); break;
		case 4: bar(480,55,628,65); setfillstyle(SOLID_FILL,GREEN); bar(480,55,600,65); break;
		case 5: bar(480,55,628,65); setfillstyle(SOLID_FILL,GREEN); bar(480,55,628,65); break;
		}
	}else{
		switch (life[who]) {
		case 0:bar(480,290,628,300); break;
		case 1:bar(480,290,628,300); setfillstyle(SOLID_FILL,RED); bar(480,290,510,300); break;
		case 2: bar(480,290,628,300); setfillstyle(SOLID_FILL,YELLOW); bar(480,290,540,300); break;
		case 3: bar(480,290,628,300); setfillstyle(SOLID_FILL,GREEN); bar(480,290,570,300); break;
		case 4: bar(480,290,628,300); setfillstyle(SOLID_FILL,GREEN); bar(480,290,600,300); break;
		case 5: bar(480,290,628,300); setfillstyle(SOLID_FILL,GREEN); bar(480,290,628,300); break;
		}
	}
	return 0;
}

/*��ʾ��ǰ�����ӵ�е�ը����*/ 
int write_bomb_num(int who)
{
	int i;
	char s[3];
	sprintf(s,"%d",bomb_num[who]);

	setcolor(LIGHTRED);
	setfillstyle(SOLID_FILL,LIGHTBLUE);
	settextstyle(1,0,1);
	if (who==0) {
		bar(480,82,628,95);
		for (i=0;i<bomb_num[who];i++) {
			setcolor(LIGHTCYAN);
			setfillstyle(1,LIGHTCYAN);
			pieslice(485+13*i,88,0,360,5);
			setcolor(WHITE);
			setfillstyle(1,WHITE);
			pieslice(486+13*i,87,0,360,1);
		}
	}else{
		bar(480,317,628,330);
		for (i=0;i<bomb_num[who];i++) {
			setcolor(LIGHTCYAN);
			setfillstyle(1,LIGHTCYAN);
			pieslice(485+13*i,323,0,360,5);
			setcolor(WHITE);
			setfillstyle(1,WHITE);
			pieslice(486+13*i,322,0,360,1);
		}
	}
	return 0;
}

/*��ʾ��ǰ����ҵ�ը������*/ 
int write_bomb_power(int who){
	char s[3];
	sprintf(s,"%d",bomb_power[who]);

	setcolor(LIGHTRED);
	setfillstyle(SOLID_FILL,LIGHTBLUE);
	settextstyle(1,0,1);
	if (who==0) {
		bar(600,95,628,110);
		outtextxy(600,95,s);
	}else{
		bar(600,330,628,350);
		outtextxy(600,330,s);
	}
	return 0;
}