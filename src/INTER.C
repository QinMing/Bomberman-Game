/*===================�ļ�INTER.C=======================
��ż����жϺ�ʱ���жϺ���
----------------------------------------------------*/ 

/*����һ������ָ��*/
void interrupt (*oldTimer)();

/*=====================================================
����ԭ�ͣ�void interrupt newTimer();
���ܣ�ʱ���жϺ������˺�������18.2Hz��Ƶ�ʱ����ã�
     �Ӷ��ü�ʱ������ÿ�� 1/18.2s ������1
������ϵ����ϵͳ����
------------------------------------------------------*/ 
void interrupt newTimer()
{
	Musiccount[mi]++;
	newtime = (newtime+1) % 30000;
	oldTimer();
	return;
}

/*��װʱ���ж�*/ 
void setTimer(void interrupt (*IntProc)())
{
	oldTimer=getvect(TIMER);
	disable();
	setvect(TIMER,IntProc);
	enable();
}

/*ɾ��ʱ���ж�*/
void killTimer()
{
	disable();
	setvect(TIMER,oldTimer);
	enable();
}

/*�����жϡ��а������»��ͷ�ʱ������*/
void far interrupt NewInt9(void)
{
	unsigned char ScanCode,temp;
	ScanCode=inportb(0x60);
	temp=inportb(0x61);
	outportb(0x61,temp | 0x80);
	outportb(0x61,temp & 0x7f);
	if(ScanCode&0x80)
	{/*�а����ͷ�*/
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
	{/*�а�������*/
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

 /*��װ�����ж�*/
void InstallKeyboard(void)
{
	OldInt9Handler = getvect(9);         /*�ж�����ֵ*/
	setvect(9,NewInt9);            /*�жϳ���NewInt9��ַ����ָ�����ж���������INT09H*/
}

 /*ɾ�������ж�*/
void ShutDownKeyboard(void)
{
	setvect(9,OldInt9Handler);
}

