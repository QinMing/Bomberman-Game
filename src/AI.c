/*===================文件AI.c=======================
此文件为游戏的人工智能引擎，在GamePlay()函数调用本文件中的AI_OP()文件实现嵌入 
本引擎设置了3种不同难度的AI，由全局变量gamelevel决定，可供玩家自主选择 
----------------------------------------------------*/ 

static char q[15001][2]; /*搜索顺序使用宽度优先搜索，用队列作为存储数据结构*/
static char pt[1000];  /*记录到达目标的路径*/ 
static int open,closed,pth;  /*队首队尾指针和路径的长度*/
static const int s[2][4]={{-1,1,0,0},{0,0,-1,1}};  /*搜索走的策略也就是上下左右四个方向*/
struct pos { 
	int x,y;
} p1,cur,goal; /*记录搜索当前位置的变量*/
static int Location[2],nx,ny,nx1,ny1;  /*AI目前在地图的位置和下一格的位置*/

/*=====================================================
函数原型：int danger(int cx,int cy,int t);
功能：判断编号为t的炸弹对(cx,cy)点是否有威胁 
入口：AI的格点坐标(cx,cy)，和炸弹编号t 
出口：返回0和1，0表示无威胁，1表示有威胁 
从属关系：此函数被AI_Danger函数调用，为子判读程序，且只在AI难度为Crazy时会使用 
-------------------------------------------------------*/ 
int danger(int cx,int cy,int t)  
{
	int deltax,deltay,whs,pw;
	deltax=abs(cx-bombi[t]);
	deltay=abs(cy-bombj[t]);
	whs=whosebomb[t];
	pw=bomb_power[whs];
	if ((deltax==0&&deltay<=pw)||(deltax<=pw&&deltay==0)||(deltax==0&&deltay==0))return 1;
	return 0;
}

/*=====================================================
函数原型：int potential(int cx,int cy,int j,int t)； 
功能：判断与玩家(cx,cy)点同行或同列的炸弹是否是潜在威胁 
入口：AI的格点坐标(cx,cy)，炸弹的列号或行号j，同列t取0，同行t取1 
出口：返回0和1，0表示这颗炸弹不是潜在威胁，1表示 是潜在威胁 
从属关系：此函数被AI_Danger函数调用，为子判读程序，且只在AI难度为Crazy时会使用 
-------------------------------------------------------*/
int potential(int cx,int cy,int j,int t)  
{
	int i;
	if (t==0) {
		if (j>=cy)
			for (i=cy+1;i<j;i++) if (a[i][cx]!=0&&a[i][cx]!=5) return 0;
			else for (i=j+1;i<cy;i++) if (a[i][cx]!=0&&a[i][cx]!=5) return 0;
	}
	if (t==1) {
		if (j>=cx)
			for (i=cx+1;i<j;i++) if (a[cy][i]!=0&&a[cy][i]!=5) return 0;
			else for (i=j+1;i<cx;i++) if (a[cy][i]!=0&&a[cy][i]!=5) return 0;
	}
	return 1;
} 

/*=====================================================
函数原型：int AI_danger(int cx,int cy)； 
功能：判断与玩家(cx,cy)点是否有危险 
入口：AI的格点坐标(cx,cy)
出口：返回0和1，0表示AI在该点无危险，1表示有危险 
从属关系：此函数被AI_Search()函数调用，用于判断所走路径是否安全 
-------------------------------------------------------*/
int AI_danger(int cx,int cy)   
{
	int i,j,zds;
	short bombx[16],bomby[16];
	zds=-1;
	if (gamelevel<2) {
		for (j=0;j<15;j++) if (a[j][cx]==5) return 1;
		for (j=0;j<15;j++) if (a[cy][j]==5) return 1;   
	}
	if (gamelevel==2) {
		for (j=0;j<15;j++) if (a[j][cx]==5&&potential(cx,cy,j,0)) {zds++; bombx[zds]=cx; bomby[zds]=j;}
		for (j=0;j<15;j++) if (a[cy][j]==5&&potential(cx,cy,j,1)) {zds++; bombx[zds]=j; bomby[zds]=cy;}
	}
	j=0;
	while (j<=zds) {
		for (i=0;i<MAXBOMB;i++) 
			if (bombi[i]==bombx[j]&&bombj[i]==bomby[j]) if (danger(cx,cy,i)) return 1;
		j++;
	}
	return 0;
}

/*=====================================================
函数原型：int cov(int x,int y)； 
功能：判断(x,y)点是否可以到达 
入口：坐标(x,y)
出口：返回0和1，0表示无法到达，1表示可以到达 
从属关系：此函数被AI_OP()函数调用，用于判断当前要走的这一步是否可以走到，
也就是说，判断搜索到的路径是否畅通，也就是没有被对手做了某些事堵上 
-------------------------------------------------------*/
int cov(int x,int y)    
{
	if (a[y][x]==7||a[y][x]==0||a[y][x]>=30) return 1;
	return 0;
}

/*=====================================================
函数原型：int AI_Search(int t)； 
功能：搜索一条到达指定目标的路径，返回值为0表示周围没有要找的东西，1为找到了，并且会返回一条到达该点的路径信息在pt[]数组中 
入口：t=0为炸弹，1为对手，2为道具，3为砖块，4为躲避点,5为判断该点放了炸弹后是否可以躲避 
出口：返回0和1，0表示没有这样的路径，1表示存在这样的路径 ，并且把路径存在pt[]里
从属关系：此函数被AI_OP()函数调用，用于搜索指定的目标用以决策 
-------------------------------------------------------*/
int AI_Search(int t)  
{
	int i,j,k;
	char ok[MAP_SIZE][MAP_SIZE]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},   /*判断地图上的点是否被访问过*/
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},  
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, 
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
	short fat[MAP_SIZE][MAP_SIZE]={{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},  
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}, /*记录路径的走法，0上1下2左3右走不到为-1*/
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},   
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},  
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},  
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
	{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

	cur.x=Location[0]; cur.y=Location[1];   /*下面的没什么好说的，就是一个宽度优先搜索算法*/ 
	p1.x=q[0][0]=cur.x; p1.y=q[0][1]=cur.y;
	open=-1; closed=0; pth=-1; 
	while (open!=closed)
	{
		open=(open+1)%15000;
		cur.x=q[open][0]; cur.y=q[open][1]; ok[cur.y][cur.x]=1;
		switch (t) {
		case 0:return AI_danger(cur.x,cur.y);
		case 1:
			if (a[cur.y][cur.x]==8) {
			goal.x=cur.x; goal.y=cur.y;
			while (cur.x!=Location[0]||cur.y!=Location[1])
			{
				pth++; pt[pth]=fat[cur.y][cur.x];
				cur.x=cur.x-s[0][pt[pth]];
				cur.y=cur.y-s[1][pt[pth]];
			}
			return 1;
			   }
			   for (i=0;i<4;i++)
				   if ((a[cur.y+s[1][i]][cur.x+s[0][i]]==0||a[cur.y+s[1][i]][cur.x+s[0][i]]==8||a[cur.y+s[1][i]][cur.x+s[0][i]]==7)&&(!ok[cur.y+s[1][i]][cur.x+s[0][i]])&&(!(AI_danger(cur.x+s[0][i],cur.y+s[1][i]))))
				   {
					   if ((closed+1)%15000!=open){
						   closed=(closed+1)%15000;
						   q[closed][0]=cur.x+s[0][i]; 
						   q[closed][1]=cur.y+s[1][i]; 
						   fat[q[closed][1]][q[closed][0]]=i;
					   }
				   } break;
		case 2: 
			if (a[cur.y][cur.x]>=30&&a[cur.y][cur.x]<=43) {
			goal.x=cur.x; goal.y=cur.y;
			while (cur.x!=Location[0]||cur.y!=Location[1])
			{
				pth++; pt[pth]=fat[cur.y][cur.x];
				cur.x=cur.x-s[0][pt[pth]];
				cur.y=cur.y-s[1][pt[pth]];
			}
			return 1;
				}
				for (i=0;i<4;i++)
					if ((!ok[cur.y+s[1][i]][cur.x+s[0][i]])&&(!(AI_danger(cur.x+s[0][i],cur.y+s[1][i])))) 
						if (a[cur.y+s[1][i]][cur.x+s[0][i]]==0||(a[cur.y+s[1][i]][cur.x+s[0][i]]>=30&&a[cur.y+s[1][i]][cur.x+s[0][i]]<=43)||a[cur.y+s[1][i]][cur.x+s[0][i]]==7) {
							if ((closed+1)%15000!=open){
								closed=(closed+1)%15000;
								q[closed][0]=cur.x+s[0][i]; 
								q[closed][1]=cur.y+s[1][i]; 
								fat[q[closed][1]][q[closed][0]]=i;
							}
						}

						break;
		case 3:
			if (a[cur.y][cur.x]==1) {
			goal.x=cur.x; goal.y=cur.y;
			while (cur.x!=Location[0]||cur.y!=Location[1])
			{
				pth++; pt[pth]=fat[cur.y][cur.x];
				cur.x=cur.x-s[0][pt[pth]];
				cur.y=cur.y-s[1][pt[pth]];
			}
			return 1;
			   }
			   for (i=0;i<4;i++)
				   if ((a[cur.y+s[1][i]][cur.x+s[0][i]]==0||a[cur.y+s[1][i]][cur.x+s[0][i]]==1||a[cur.y+s[1][i]][cur.x+s[0][i]]==7)&&(!ok[cur.y+s[1][i]][cur.x+s[0][i]])&&(!(AI_danger(cur.x+s[0][i],cur.y+s[1][i])))) {
					   if ((closed+1)%15000!=open){
						   closed=(closed+1)%15000;
						   q[closed][0]=cur.x+s[0][i]; q[closed][1]=cur.y+s[1][i]; fat[q[closed][1]][q[closed][0]]=i;
					   }
				   }
				   break;
		case 4:
			if (!AI_danger(cur.x,cur.y)) {
			goal.x=cur.x; goal.y=cur.y;
			while (cur.x!=Location[0]||cur.y!=Location[1])
			{
				pth++; pt[pth]=fat[cur.y][cur.x];
				cur.x=cur.x-s[0][pt[pth]];
				cur.y=cur.y-s[1][pt[pth]];
			}
			return 1;
			   }
			   for (i=0;i<4;i++)
				   if ((a[cur.y+s[1][i]][cur.x+s[0][i]]==0||a[cur.y+s[1][i]][cur.x+s[0][i]]==7)&&(!ok[cur.y+s[1][i]][cur.x+s[0][i]])) {
					   if ((closed+1)%15000!=open){
						   closed=(closed+1)%15000;
						   q[closed][0]=cur.x+s[0][i]; q[closed][1]=cur.y+s[1][i]; fat[q[closed][1]][q[closed][0]]=i;
					   }
				   }
				   break;
		case 5:
			if (!AI_danger(cur.x,cur.y)&&cur.x!=p1.x&&cur.y!=p1.y) return 1;
			for (i=0;i<4;i++)
				if ((a[cur.y+s[1][i]][cur.x+s[0][i]]==0||a[cur.y+s[1][i]][cur.x+s[0][i]]==7)&&(!ok[cur.y+s[1][i]][cur.x+s[0][i]]))
				{
					if ((closed+1)%15000!=open){
						closed=(closed+1)%15000;
						q[closed][0]=cur.x+s[0][i]; q[closed][1]=cur.y+s[1][i]; fat[q[closed][1]][q[closed][0]]=i;
					}
				}
				break;
		} 
	}
	return 0;
}

/*=====================================================
函数原型：void calc_pos(void)； 
功能：更新AI当前坐标 
从属关系：此函数被AI_OP()函数调用，每次计算新路径前先计算AI当前的坐标 
-------------------------------------------------------*/
void calc_pos(void)   /*计算AI当前的格点坐标*/
{
	Location[0]=xy_to_ij(px[0]+15);
	Location[1]=xy_to_ij(py[0]+15);
}


/*=====================================================
函数原型：void AI_Move(int t)； 
功能：控制AI的移动 
入口：移动的策略t,1为躲避炸弹策略，2为攻击对手，3为寻找道具，4为寻找可以炸掉的墙 
出口：无
从属关系：此函数被AI_OP()函数调用，用以控制AI的移动 
-------------------------------------------------------*/
void AI_Move(int t)  
{
	int state=0,can,i,px,py;
	if (t==1) {  /*这是AI有危险的情形，此时将会搜索一个没有危险的位置，然后走到该位置以躲避炸弹*/
		if ((p1.x==goal.x||p1.y==goal.y)&&(pth<0)) state=AI_Search(4);
		if (state) exroad=1; 
		if (pth>=0) {
			switch (pt[pth])
			{
			case 0:handle_direction[0]=3; nx=Location[0]+s[0][0]; ny=Location[1]+s[1][0]; break;
			case 1:handle_direction[0]=4; nx=Location[0]+s[0][1]; ny=Location[1]+s[1][1]; break;
			case 2:handle_direction[0]=1; nx=Location[0]+s[0][2]; ny=Location[1]+s[1][2]; break;
			case 3:handle_direction[0]=2; nx=Location[0]+s[0][3]; ny=Location[1]+s[1][3]; break;
			}
			pth--;
		}
		else {
			exroad=0;
		}
		return ;
	}
	if (t==2) { /*这种情况是当AI附近没有炸弹威胁，此时开始找对手，如果找到对手则移动到对手旁以将其消灭*/
		if (pth>0) {
			switch (pt[pth])
			{
			case 0:handle_direction[0]=3; nx=Location[0]+s[0][0]; ny=Location[1]+s[1][0]; break;
			case 1:handle_direction[0]=4; nx=Location[0]+s[0][1]; ny=Location[1]+s[1][1]; break;
			case 2:handle_direction[0]=1; nx=Location[0]+s[0][2]; ny=Location[1]+s[1][2]; break;
			case 3:handle_direction[0]=2; nx=Location[0]+s[0][3]; ny=Location[1]+s[1][3]; break;
			}
			pth--;
		}
		else {
			exroad=0;
			ai_sbb=1; 
		}
		return ;
	}
	if (t==3) {/*寻找附近有无有利的道具，有则移动过去捡*/
		if (pth>=0) {
			switch (pt[pth])
			{
			case 0:handle_direction[0]=3; nx=Location[0]+s[0][0]; ny=Location[1]+s[1][0]; break;
			case 1:handle_direction[0]=4; nx=Location[0]+s[0][1]; ny=Location[1]+s[1][1]; break;
			case 2:handle_direction[0]=1; nx=Location[0]+s[0][2]; ny=Location[1]+s[1][2]; break;
			case 3:handle_direction[0]=2; nx=Location[0]+s[0][3]; ny=Location[1]+s[1][3]; break;
			}
			pth--;
		}
		else exroad=0;
		return ;
	}
	if (t==4) {/*找一个可以炸掉的砖块，将其炸掉*/
		if (pth>0) {
			switch (pt[pth])
			{
			case 0:handle_direction[0]=3; nx=Location[0]+s[0][0]; ny=Location[1]+s[1][0]; break;
			case 1:handle_direction[0]=4; nx=Location[0]+s[0][1]; ny=Location[1]+s[1][1]; break;
			case 2:handle_direction[0]=1; nx=Location[0]+s[0][2]; ny=Location[1]+s[1][2]; break;
			case 3:handle_direction[0]=2; nx=Location[0]+s[0][3]; ny=Location[1]+s[1][3]; break;
			}
			pth--;
		}
		else {
			goal.x=Location[0]; goal.y=Location[1];
			can=AI_Search(5);
			if (can) {exroad=0; ai_sbb=1; return;}
			else {
				px=Location[0]; py=Location[1];
				for (i=0;i<4;i++)
					if ((a[py+s[1][i]][px+s[0][i]]==0||a[py+s[1][i]][px+s[0][i]]==7||(a[py+s[1][i]][px+s[0][i]]>=30&&a[py+s[1][i]][px+s[0][i]]<=43))&&(!(AI_danger(px+s[0][i],py+s[1][i])))) break;
				switch (i)
				{
				case 0:handle_direction[0]=3; nx=Location[0]+s[0][0]; ny=Location[1]+s[1][0]; break;
				case 1:handle_direction[0]=4; nx=Location[0]+s[0][1]; ny=Location[1]+s[1][1]; break;
				case 2:handle_direction[0]=1; nx=Location[0]+s[0][2]; ny=Location[1]+s[1][2]; break;
				case 3:handle_direction[0]=2; nx=Location[0]+s[0][3]; ny=Location[1]+s[1][3]; break;
				}
				exroad=0;
			}
		}
		return ;
	}
}

/*=====================================================
函数原型：void AI_OP(int b)； 
功能：AI操作的接口，用以统领AI的全部功能 
入口：b为AI现在可以放的炸弹数 
出口：无
从属关系：此函数在GamePlay()中被调用，为AI的入口 
-------------------------------------------------------*/
void AI_OP(int b)  
{
	static int state,celue;
	calc_pos();  /*先计算坐标*/
	if (handle_direction[0]!=0) {
		switch (handle_direction[0])
		{
		case 1:nx1=Location[0]+s[0][2]; ny1=Location[1]+s[1][2]; break;
		case 2:nx1=Location[0]+s[0][3]; ny1=Location[1]+s[1][3]; break;
		case 3:nx1=Location[0]+s[0][0]; ny1=Location[1]+s[1][0]; break;
		case 4:nx1=Location[0]+s[0][1]; ny1=Location[1]+s[1][1]; break;
		}    /*计算下一个将要走到的点坐标*/
		if (!cov(nx1,ny1)) {AI_Reset(); return ;}   /*判断路径下一步是否可以到达，如果可以就走，如果下一步不可达，说明可能被对手做了某些事挡住了，需要重置AI重新计算路径*/
		if (Location[0]!=nx||Location[1]!=ny) return ;
		else {handle_direction[0]=0; return ;}  /*如果下一格可达，则一直保持移动直到到达下一格为止*/
	}                     
	if ((px[0]-10)%30!=0||(py[0]-10)%30!=0) return ;  /*判断是否停在整格*/
	if (ai_sbb) {set_bomb(0); ai_sbb=0;}   /*路径运行到最后，判断这条路径最后是否需要放炸弹，如果要，就放炸弹*/
	if (!exroad) {  /*如果AI此时不在执行一条已搜索到的路径，就开始搜索路径*/
		celue=state=0;
		state=AI_Search(0);      /*先搜索附近是否有炸弹*/
		if (state!=0) celue=1;   /*若有有炸弹，执行策略1*/
		else if (celue==0) {
			if (gamelevel==0) 
				if (random(3)==0) state=AI_Search(1);  /*如果这时候是简单模式，有1/3的概率AI会选择来攻击对手*/
				else ;
			else state=AI_Search(1);
		}   /*炸弹找不到，找对手*/
		if (celue==0&&state!=0) {
			celue=2;  /*附近有对手，执行策略2*/
		}else if (celue==0) {
			state=AI_Search(2);  /*如果附近没有对手，找道具*/
		}
		if (celue==0&&state!=0) {
			celue=3; /*如果附近有道具，执行策略3*/
		}else if (celue==0) {
			state=AI_Search(3);  /*如果道具也找不到，就找可以炸掉的墙把它炸掉*/ 
		}
		if (celue==0&&b>0&&state!=0) celue=4; /*如果附近有墙，就执行策略4*/
		if (celue>1) exroad=1;  /*以上所有情况都算找到路径*/
		if (celue>=1&&celue<=4) AI_Move(celue); /*找到了东西，开始执行策略(不可能4种情况都找不到)*/
	}
	else AI_Move(celue);  /*如果这时候正在执行一条路径，就继续执行*/
	return ;
}

/*=====================================================
函数原型：void AI_Reset(void)； 
功能：重置AI的路径信息 
入口：无
出口：无
从属关系：此函数在AI_OP()以及GamePlay()中被调用，如果AI当前路径失效或者是死了，就要重置其路径信息 
-------------------------------------------------------*/
void AI_Reset(void) 
{
	handle_direction[0]=0;
	ai_sbb=0;
	exroad=0;
}
