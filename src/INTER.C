/*===================文件INTER.C=======================
存放键盘中断和时间中断函数
----------------------------------------------------*/ 

/*定义一个函数指针*/
void interrupt (*oldTimer)();

/*=====================================================
函数原型：void interrupt newTimer();
功能：时间中断函数。此函数将以18.2Hz的频率被调用，
     从而让计时器变量每过 1/18.2s 就增加1
从属关系：被系统调用
------------------------------------------------------*/ 
void interrupt newTimer()
{
	Musiccount[mi]++;
	newtime = (newtime+1) % 30000;
	oldTimer();
	return;
}

/*安装时间中断*/ 
void setTimer(void interrupt (*IntProc)())
{
	oldTimer=getvect(TIMER);
	disable();
	setvect(TIMER,IntProc);
	enable();
}

/*删除时间中断*/
void killTimer()
{
	disable();
	setvect(TIMER,oldTimer);
	enable();
}

/*键盘中断。有按键按下或释放时被调用*/
void far interrupt NewInt9(void)
{
	unsigned char ScanCode,temp;
	ScanCode=inportb(0x60);
	temp=inportb(0x61);
	outportb(0x61,temp | 0x80);
	outportb(0x61,temp & 0x7f);
	if(ScanCode&0x80)
	{/*有按键释放*/
		ScanCode&=0x7f;
		switch(ScanCode){
		case KEY_W: keyrelease(0,1);break;
		case KEY_S: keyrelease(0,2);break;
		case KEY_A: keyrelease(0,3);break;
		case KEY_D: keyrelease(0,4);break;
		case KEY_UP: keyrelease(1,1);break;
		case KEY_DOWN: keyrelease(1,2);break;
		case KEY_LEFT: keyrelease(1,3);break;
		case KEY_RIGHT: keyrelease(1,4);break;
		case KEY_SPACE: bomb_button[0]=1;break;
		case KEY_ENTER: bomb_button[1]=1;break;
		case KEY_P: pause_button = 1;break;
		case KEY_ESC:if_play_again=0;  continuegame=0;break;
		}
		menu_keystate[ScanCode] = 1 ;

	}
	else
	{/*有按键按下*/
		switch(ScanCode){
		case KEY_W: keypress(0,1);break;
		case KEY_S: keypress(0,2);break;
		case KEY_A: keypress(0,3);break;
		case KEY_D: keypress(0,4);break;
		case KEY_UP: keypress(1,1);break;
		case KEY_DOWN: keypress(1,2);break;
		case KEY_LEFT: keypress(1,3);break;
		case KEY_RIGHT: keypress(1,4);break;
		}
	}
	outportb(0x20,0x20);
}

void interrupt far (*OldInt9Handler)(); 

 /*安装键盘中断*/
void InstallKeyboard(void)
{
	OldInt9Handler = getvect(9);         /*中断向量值*/
	setvect(9,NewInt9);            /*中断程序NewInt9地址存入指定的中断向量表中INT09H*/
}

 /*删除键盘中断*/
void ShutDownKeyboard(void)
{
	setvect(9,OldInt9Handler);
}


