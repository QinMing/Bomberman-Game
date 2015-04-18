#define DELAY_TIME 1000
/*===================文件menu.c=======================
此文件用以画游戏的菜单界面 
----------------------------------------------------*/ 

/*=====================================================
函数原型：void drawStar(void);
功能：画背景中的星星 
入口：无
出口：无
从属关系：此函数被welcome()以及gameover()等函数调用，画的是满天星背景 
-------------------------------------------------------*/ 
void drawStar(void)                                   /*  背景 */
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
函数原型：void welcome(void);
功能：游戏的主菜单，也即欢迎界面 
入口：无
出口：无
从属关系：此函数被gameplay()函数调用，为游戏的主菜单 
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
Loop2:map_key=musiC();                          /*游戏开始的流程,map_key为检测到按键*/
	if (map_key==KEY_1)                                   /*检测到按1*/
	{
		MODE=0;
		settextstyle(1,0,3);
		setcolor(13);
		outtextxy(240,315,"1.One Player"); 
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		cleardevice();
		selectlevel();
	}

	if (map_key==KEY_2)                                              /*检测到按2，同上*/
	{
		MODE=1;
		settextstyle(1,0,3);
		setcolor(13);
		outtextxy(240,365,"2.Two Players");
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		cleardevice();
		selectmap();
	}
	if (map_key==KEY_3)                                        /*检测到按3*/
	{
		settextstyle(1,0,3);
		setcolor(13);
		outtextxy(240,415,"3.About..");
		cleardevice();
		putbmp("About.bmp");                                     /*贴关于的bmp图*/
		while (map_key!=KEY_ENTER&&map_key!=KEY_ESC) map_key=musiC();                 /*继续放音乐，并检测到按键*/
		if (map_key==KEY_ENTER)                                      /*检测到回车时，再回到主菜单，并放音乐*/
		{
			cleardevice();
			goto Loop1;
		}
		if (map_key==KEY_ESC) exit(0);
	}
	if (map_key==KEY_H)                                        /*检测到按H*/
	{
		cleardevice();
		putbmp("HELP.bmp");                                     /*贴帮助的bmp图*/
		while (map_key!=KEY_ENTER&&map_key!=KEY_ESC) map_key=musiC();                                        /*继续放音乐，并检测到按键*/
		if (map_key==KEY_ENTER)                                      /*检测到回车时，再回到主菜单，并放音乐*/
		{
			cleardevice();
			goto Loop1;
		}
		if (map_key==KEY_ESC) exit(0);
	}
	if (map_key==KEY_ENTER)                                          /*在主菜单检测到回车，忽视它，继续放音乐，等待按键*/
	{
		goto Loop2;
	}
	if (map_key==KEY_ESC) exit(0);                                     
	return ;
}

/*=====================================================
函数原型：void selectmap(void);
功能：画选择地图界面的函数 
入口：无
出口：无
从属关系：此函数被welcome()和selectlevel()函数调用，是选择地图界面的函数 
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
		for (i=1;i<=600;i++) delay(DELAY_TIME);    /*清屏后正中显示MAP 1，停顿一段时间*/
		sel_map=0;
	}
	if (map_key==KEY_2) {
		settextstyle(1,0,5);
		setcolor(13);
		outtextxy(340,270,"2");
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		cleardevice();   
		settextstyle(1,0,5); setcolor(YELLOW); outtextxy(240,200,"MAP 2");
		for (i=1;i<=600;i++) delay(DELAY_TIME);    /*清屏后正中显示MAP 2，停顿一段时间*/
		sel_map=1;
	}
	if (map_key==KEY_3) {   
		settextstyle(1,0,5);
		setcolor(13);
		outtextxy(540,270,"3");
		for (i=1;i<=600;i++) delay(DELAY_TIME);
		cleardevice();  
		settextstyle(1,0,5); setcolor(YELLOW); outtextxy(240,200,"MAP 3");
		for (i=1;i<=600;i++) delay(DELAY_TIME);    /*清屏后正中显示MAP 3，停顿一段时间*/
		sel_map=2;
	}
	if (map_key==KEY_ESC) exit(0);
	return ;
}

/*=====================================================
函数原型：void selectlevel(void);
功能：画选择游戏难度界面的函数 
入口：无
出口：无
从属关系：此函数被welcome()函数调用，是选择游戏难度界面的函数 
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
函数原型：int musiC(void);
功能：在菜单中播放背景音乐，并接受菜单的按键操作 
入口：无
出口：无
从属关系：menu.c中大量调用 
-------------------------------------------------------*/ 
int musiC(void)
{
	int i=beginmi,j;

	while(1)
	{
		nosound();
		sound(freq[i]);                                                                  /*选择音符*/
		for (j=1;j<=400;j++)
			delay(135*dely[i]);                                                            /*决定音符长度*/
		i++;
		if (i>=MAXY) i=0;
		if (menu_keystate[0x02]) {nosound(); beginmi=i; menu_keystate[0x02]=0; return 0x02;}                                       /*检测到按下1即结束播放音乐，返回按键值*/
		if (menu_keystate[0x03]) {nosound(); beginmi=i; menu_keystate[0x03]=0; return 0x03;}                                       /*检测到按下2即结束播放音乐，返回按键值*/
		if (menu_keystate[0x04]) {nosound(); beginmi=i; menu_keystate[0x04]=0; return 0x04;}                                       /*检测到按下3即结束播放音乐，返回按键值*/
		if (menu_keystate[0x01]) {nosound(); beginmi=i; menu_keystate[0x01]=0; return 0x01;}                                       /*检测到按下ESC即结束播放音乐，返回按键值  */
		if (menu_keystate[0x23]) {nosound(); beginmi=i; menu_keystate[0x23]=0; return 0x23;}                             /*检测到按下H即返回按键值，音乐不停止，所以改变begini的值，下次(HELP界面)调用时从当前音符开始*/
		if (menu_keystate[0x1c]) {nosound(); beginmi=i; menu_keystate[0x1c]=0; return 0x1c;}                             /*检测到按下Enter即返回按键值，音乐不停止，所以改变begini的值，下次(回到游戏主菜单界面)调用时从当前音符开始*/                         
	}
	nosound();
	return 0;
}

/*=====================================================
函数原型：void putbmp(char *bmpname);
功能：贴图 
入口：无
出口：无
从属关系：此函数被welcome()函数调用，用来贴关于和帮助图 
-------------------------------------------------------*/ 
void putbmp(char *bmpname)
{
	FILE *fp;
	int r,c;
	char color;
	fp=fopen(bmpname,"rb");
	fseek(fp,118,SEEK_SET);/*跳过位图文件颜色信息以外的部分(在16色位图中为118字节)*/
	for(r=479;r>=0;r--)
		for(c=0;c<639;c+=2)/*循环读取16色位图颜色信息(因为其保存顺序为从下到上,从左至右)*/
		{
			color=fgetc(fp);
			switch(color & 0xF0)/*前4位为第一个象素的颜色*/
			{/*将16色位图颜色替换成TC中最接近的颜色*/
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
			switch(color & 0x0F)/*后4位为第二个象素的颜色*/
			{/*同上*/
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
		fclose(fp);/*打印完成,关闭文件*/
}
