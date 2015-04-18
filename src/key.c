/*===================文件key.c=======================
此文件的函数被INTER.C中的键盘中断“NewInt9()”调用
用于将键盘上传入的原始信号，加工成游戏使用的操作，并使用全局变量handle_direction[]传出
此模块封装程度高，入口和出口简单。能够处理两个用户同时操作的情况、一个用户同时按多个方向键的情况
----------------------------------------------------*/ 

/*=====================================================
函数原型：int keypress(int who,int k);
功能：当键盘上有新的按键按下时，就把此按键加入队列，并将其设置为用户的当前操作handle_direction。
入口：玩家序号who、按键值k（k的取值为1,2,3,4，分别代表上下左右）
出口：修改全局变量handle_direction[]
从属关系：此函数当有按键按下时，被INTER.C中的键盘中断“NewInt9()”调用
-------------------------------------------------------*/ 
int keypress(int who,int k)
{
	if((MODE==0)&&(who==NPC)) return 0;/*如果现在是人机对战，则不对player0的键盘进行处理*/
	if(k==queue[who][ qp[who] ]) return 0;
	keystate[who][k]=1;
	qp[who]++;
	queue[who][ qp[who] ]=k;
	handle_direction[who]=k;
	return 0;
}

/*=====================================================
函数原型：int keyrelease(int who,int k);
功能：当键盘上有按键释放时，将keystate设为释放状态，
     然后在队列中寻找上一个按下的、并仍然按着的按键，将这个按键设为用户操作handle_direction。
入口：玩家序号who、按键值k（k的取值为1,2,3,4，分别代表上下左右）
出口：修改全局变量handle_direction[]
从属关系：此函数当有按键释放时，被INTER.C中的键盘中断“NewInt9()”调用
------------------------------------------------------*/ 
int keyrelease(int who,int k)
{
	if((MODE==0)&&(who==NPC)) return 0;
	keystate[who][k]=0;
	if(k==queue[who][ qp[who] ]){
		do{
			qp[who]--;
		}while((keystate[who][  queue[who][ qp[who] ]  ]==0)&&(qp[who]>0));
		handle_direction[who] = queue[who][ qp[who] ];
	}
	return 0;
}