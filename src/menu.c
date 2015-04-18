#define DELAY_TIME 1000
/*===================�ļ�menu.c=======================
���ļ����Ի���Ϸ�Ĳ˵����� 
----------------------------------------------------*/ 

/*=====================================================
����ԭ�ͣ�void drawStar(void);
���ܣ��������е����� 
��ڣ���
���ڣ���
������ϵ���˺�����welcome()�Լ�gameover()�Ⱥ������ã������������Ǳ��� 
-------------------------------------------------------*/ 
void drawStar(void)                                   /*  ���� */
{
	int seed=2000;
	int i,dotx,doty,height,width,color,maxcolor;
	maxcolor=getmaxcolor();
	width=getmaxx();
	height=getmaxy();
	srand(seed);
	for(i=0;i<250;i++)
	{
		dotx=i+random(width-1);
		doty=i+random(height-1);
		color=random(maxcolor);
		setcolor(color);
		putpixel(dotx,doty,color);
		circle(dotx+1,doty+1,1);
	}
	srand(seed);
}

/*=====================================================
����ԭ�ͣ�void welcome(void);
���ܣ���Ϸ�����˵���Ҳ����ӭ���� 
��ڣ���
���ڣ���
������ϵ���˺�����gameplay()�������ã�Ϊ��Ϸ�����˵� 
-------------------------------------------------------*/ 
void welcome(void){
	int map_key,i;

	for (i=0;i<128;i++) 
		menu_keystate[i]=0;
Loop1:    
	cleardevice();
	setbkcolor(BLACK);
	drawStar();
	setcolor(RED);
	settextstyle(1,0,4);
	outtextxy(80,100," WELCOME TO PLAY BOMB MAN!");
	setcolor(YELLOW);
	settextstyle(1,0,3);
	outtextxy(50,200,"Please Select Game Mode or press H for help:");
	outtextxy(50,240,"Press ESC to Quit.");
	setfillstyle(1,LIGHTBLUE);
	bar(220,300,420,460);
	rectangle(220,300,420,460);
	settextstyle(1,0,3);
	setcolor(YELLOW);
	outtextxy(240,315,"1.One Player");
	outtextxy(240,365,"2.Two Players");
	outtextxy(240,415,"3.About..");
	beginmi=0;
Loop2:map_key=musiC();                          /*��Ϸ��ʼ������,map_keyΪ��⵽����*/
	if (map_key==KEY_1)                                   /*��⵽��1*/
	{
		MODE=0;
		settextstyle(1,0,3);
		setcolor(13);
		outtextxy(240,315,"1.One Player"); 
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		cleardevice();
		selectlevel();
	}

	if (map_key==KEY_2)                                              /*��⵽��2��ͬ��*/
	{
		MODE=1;
		settextstyle(1,0,3);
		setcolor(13);
		outtextxy(240,365,"2.Two Players");
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		cleardevice();
		selectmap();
	}
	if (map_key==KEY_3)                                        /*��⵽��3*/
	{
		settextstyle(1,0,3);
		setcolor(13);
		outtextxy(240,415,"3.About..");
		cleardevice();
		putbmp("About.bmp");                                     /*�����ڵ�bmpͼ*/
		while (map_key!=KEY_ENTER&&map_key!=KEY_ESC) map_key=musiC();                 /*���������֣�����⵽����*/
		if (map_key==KEY_ENTER)                                      /*��⵽�س�ʱ���ٻص����˵�����������*/
		{
			cleardevice();
			goto Loop1;
		}
		if (map_key==KEY_ESC) exit(0);
	}
	if (map_key==KEY_H)                                        /*��⵽��H*/
	{
		cleardevice();
		putbmp("HELP.bmp");                                     /*��������bmpͼ*/
		while (map_key!=KEY_ENTER&&map_key!=KEY_ESC) map_key=musiC();                                        /*���������֣�����⵽����*/
		if (map_key==KEY_ENTER)                                      /*��⵽�س�ʱ���ٻص����˵�����������*/
		{
			cleardevice();
			goto Loop1;
		}
		if (map_key==KEY_ESC) exit(0);
	}
	if (map_key==KEY_ENTER)                                          /*�����˵���⵽�س��������������������֣��ȴ�����*/
	{
		goto Loop2;
	}
	if (map_key==KEY_ESC) exit(0);                                     
	return ;
}

/*=====================================================
����ԭ�ͣ�void selectmap(void);
���ܣ���ѡ���ͼ����ĺ��� 
��ڣ���
���ڣ���
������ϵ���˺�����welcome()��selectlevel()�������ã���ѡ���ͼ����ĺ��� 
-------------------------------------------------------*/ 
void selectmap(void){
	int map_key,i;
	setbkcolor(BLACK);
	drawStar();
	setcolor(YELLOW);
	settextstyle(1,0,4);
	outtextxy(100,100,"Please Select a Map:");
	bar(100,240,240,360);
	rectangle(100,240,240,360);
	setfillstyle(1,LIGHTBLUE);
	bar(280,240,420,360);
	rectangle(280,240,420,360);
	bar(480,240,620,360);
	rectangle(480,240,620,360);
	settextstyle(1,0,5);
	setcolor(YELLOW);
	outtextxy(160,270,"1");
	outtextxy(340,270,"2");
	outtextxy(540,270,"3");
	map_key=musiC();
	if (map_key==KEY_1) {   
		settextstyle(1,0,5);
		setcolor(13);
		outtextxy(160,270,"1");
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		cleardevice();
		settextstyle(1,0,5); setcolor(YELLOW); outtextxy(240,200,"MAP 1");
		for (i=1;i<=600;i++) delay(DELAY_TIME);    /*������������ʾMAP 1��ͣ��һ��ʱ��*/
		sel_map=0;
	}
	if (map_key==KEY_2) {
		settextstyle(1,0,5);
		setcolor(13);
		outtextxy(340,270,"2");
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		cleardevice();   
		settextstyle(1,0,5); setcolor(YELLOW); outtextxy(240,200,"MAP 2");
		for (i=1;i<=600;i++) delay(DELAY_TIME);    /*������������ʾMAP 2��ͣ��һ��ʱ��*/
		sel_map=1;
	}
	if (map_key==KEY_3) {   
		settextstyle(1,0,5);
		setcolor(13);
		outtextxy(540,270,"3");
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		cleardevice();  
		settextstyle(1,0,5); setcolor(YELLOW); outtextxy(240,200,"MAP 3");
		for (i=1;i<=600;i++) delay(DELAY_TIME);    /*������������ʾMAP 3��ͣ��һ��ʱ��*/
		sel_map=2;
	}
	if (map_key==KEY_ESC) exit(0);
	return ;
}

/*=====================================================
����ԭ�ͣ�void selectlevel(void);
���ܣ���ѡ����Ϸ�ѶȽ���ĺ��� 
��ڣ���
���ڣ���
������ϵ���˺�����welcome()�������ã���ѡ����Ϸ�ѶȽ���ĺ��� 
-------------------------------------------------------*/ 
void selectlevel(void){
	int map_key,i;
	setbkcolor(BLACK);
	drawStar();
	setcolor(YELLOW);
	settextstyle(1,0,4);
	outtextxy(100,100,"Please Select Game Level:");
	setcolor(GREEN);
	settextstyle(1,0,4);
	outtextxy(250,200,"1.Easy");
	outtextxy(250,270,"2.Medium");
	outtextxy(250,340,"3.Crazy");
	map_key=musiC();
	if (map_key==KEY_1) {   
		setcolor(13);
		settextstyle(1,0,4);
		outtextxy(250,200,"1.Easy");
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		gamelevel=0;
		cleardevice();
		selectmap();
	}
	if (map_key==KEY_2) {
		setcolor(13);
		settextstyle(1,0,4);
		outtextxy(250,270,"2.Medium");
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		gamelevel=1;
		cleardevice();
		selectmap();
	}
	if (map_key==KEY_3) {   
		setcolor(13);
		settextstyle(1,0,4);
		outtextxy(250,340,"3.Crazy");
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		gamelevel=2;
		cleardevice();
		selectmap();
	}
	if (map_key==KEY_ESC) exit(0);
	return ;
}

/*=====================================================
����ԭ�ͣ�int musiC(void);
���ܣ��ڲ˵��в��ű������֣������ܲ˵��İ������� 
��ڣ���
���ڣ���
������ϵ��menu.c�д������� 
-------------------------------------------------------*/ 
int musiC(void)
{
	int i=beginmi,j;

	while(1)
	{
		nosound();
		sound(freq[i]);                                                                  /*ѡ������*/
		for (j=1;j<=400;j++)
			delay(135*dely[i]);                                                            /*������������*/
		i++;
		if (i>=MAXY) i=0;
		if (menu_keystate[0x02]) {nosound(); beginmi=i; menu_keystate[0x02]=0; return 0x02;}                                       /*��⵽����1�������������֣����ذ���ֵ*/
		if (menu_keystate[0x03]) {nosound(); beginmi=i; menu_keystate[0x03]=0; return 0x03;}                                       /*��⵽����2�������������֣����ذ���ֵ*/
		if (menu_keystate[0x04]) {nosound(); beginmi=i; menu_keystate[0x04]=0; return 0x04;}                                       /*��⵽����3�������������֣����ذ���ֵ*/
		if (menu_keystate[0x01]) {nosound(); beginmi=i; menu_keystate[0x01]=0; return 0x01;}                                       /*��⵽����ESC�������������֣����ذ���ֵ  */
		if (menu_keystate[0x23]) {nosound(); beginmi=i; menu_keystate[0x23]=0; return 0x23;}                             /*��⵽����H�����ذ���ֵ�����ֲ�ֹͣ�����Ըı�begini��ֵ���´�(HELP����)����ʱ�ӵ�ǰ������ʼ*/
		if (menu_keystate[0x1c]) {nosound(); beginmi=i; menu_keystate[0x1c]=0; return 0x1c;}                             /*��⵽����Enter�����ذ���ֵ�����ֲ�ֹͣ�����Ըı�begini��ֵ���´�(�ص���Ϸ���˵�����)����ʱ�ӵ�ǰ������ʼ*/                         
	}
	nosound();
	return 0;
}

/*=====================================================
����ԭ�ͣ�void putbmp(char *bmpname);
���ܣ���ͼ 
��ڣ���
���ڣ���
������ϵ���˺�����welcome()�������ã����������ںͰ���ͼ 
-------------------------------------------------------*/ 
void putbmp(char *bmpname)
{
	FILE *fp;
	int r,c;
	char color;
	fp=fopen(bmpname,"rb");
	fseek(fp,118,SEEK_SET);/*����λͼ�ļ���ɫ��Ϣ����Ĳ���(��16ɫλͼ��Ϊ118�ֽ�)*/
	for(r=479;r>=0;r--)
		for(c=0;c<639;c+=2)/*ѭ����ȡ16ɫλͼ��ɫ��Ϣ(��Ϊ�䱣��˳��Ϊ���µ���,��������)*/
		{
			color=fgetc(fp);
			switch(color & 0xF0)/*ǰ4λΪ��һ�����ص���ɫ*/
			{/*��16ɫλͼ��ɫ�滻��TC����ӽ�����ɫ*/
			case 0x00:
				{
					putpixel(c,r,BLACK);
					break;
				}
			case 0x10:
				{
					putpixel(c,r,RED);
					break;
				}
			case 0x20:
				{
					putpixel(c,r,GREEN);
					break;
				}
			case 0x30:
				{
					putpixel(c,r,BROWN);
					break;
				}
			case 0x40:
				{
					putpixel(c,r,BLUE);
					break;
				}
			case 0x50:
				{
					putpixel(c,r,MAGENTA);
					break;
				}
			case 0x60:
				{
					putpixel(c,r,CYAN);
					break;
				}
			case 0x70:
				{
					putpixel(c,r,DARKGRAY);
					break;
				}
			case 0x80:
				{
					putpixel(c,r,LIGHTGRAY);
					break;
				}
			case 0x90:
				{
					putpixel(c,r,LIGHTRED);
					break;
				}
			case 0xa0:
				{
					putpixel(c,r,LIGHTGREEN);
					break;
				}
			case 0xb0:
				{
					putpixel(c,r,YELLOW);
					break;
				}
			case 0xc0:
				{
					putpixel(c,r,LIGHTBLUE);
					break;
				}
			case 0xd0:
				{
					putpixel(c,r,LIGHTMAGENTA);
					break;
				}
			case 0xe0:
				{
					putpixel(c,r,LIGHTCYAN);
					break;
				}
			case 0xf0:
				{
					putpixel(c,r,WHITE);
					break;
				}
			}
			switch(color & 0x0F)/*��4λΪ�ڶ������ص���ɫ*/
			{/*ͬ��*/
			case 0x00:
				{
					putpixel(c+1,r,BLACK);
					break;
				}
			case 0x01:
				{
					putpixel(c+1,r,RED);
					break;
				}
			case 0x02:
				{
					putpixel(c+1,r,GREEN);
					break;
				}
			case 0x03:
				{
					putpixel(c+1,r,BROWN);
					break;
				}
			case 0x04:
				{
					putpixel(c+1,r,BLUE);
					break;
				}
			case 0x05:
				{
					putpixel(c+1,r,MAGENTA);
					break;
				}
			case 0x06:
				{
					putpixel(c+1,r,CYAN);
					break;
				}
			case 0x07:
				{
					putpixel(c+1,r,DARKGRAY);
					break;
				}
			case 0x08:
				{
					putpixel(c+1,r,LIGHTGRAY);
					break;
				}
			case 0x09:
				{
					putpixel(c+1,r,LIGHTRED);
					break;
				}
			case 0x0a:
				{
					putpixel(c+1,r,LIGHTGREEN);
					break;
				}
			case 0x0b:
				{
					putpixel(c+1,r,YELLOW);
					break;
				}
			case 0x0c:
				{
					putpixel(c+1,r,LIGHTBLUE);
					break;
				}
			case 0x0d:
				{
					putpixel(c+1,r,LIGHTMAGENTA);
					break;
				}
			case 0x0e:
				{
					putpixel(c+1,r,LIGHTCYAN);
					break;
				}
			case 0x0f:
				{
					putpixel(c+1,r,WHITE);
					break;
				}
			}
		}
		fclose(fp);/*��ӡ���,�ر��ļ�*/
}
