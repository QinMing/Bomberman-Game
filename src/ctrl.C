/*===================文件ctrl.c=======================
存放所有与人物控制相关的函数
----------------------------------------------------*/ 


/*=====================================================
函数原型：int check_if_someone_lost();
功能：检查双方生命值
入口：无
出口：全局变量loser
从属关系：被things.c中deathpotion()和 本文件中die() 调用
只要生命减少，它就会被调用
------------------------------------------------------*/ 
int check_if_someone_lost()
{    
	if ( life[0] == 0 ) {
		if ( loser != 0 ) 
			loser = 20;
		else
			loser = 10;
	}
	if ( life[1] == 0 ) {
		if ( loser != 0 ) 
			loser = 20;
		else
			loser = 11;
	}
	return 0;
}

/*=====================================================
函数原型：int count_ij(int who , int y , int x , int j[] , int i[]);
功能：根据当前人物在屏幕上的坐标，计算出他所在地图上的两个格点坐标，
     顺便在地图数组a中把这两格标记为相应数字：player0记为7，player1记为8
入口：人物编号who，在屏幕上的纵坐标y和横坐标x
出口：数组 int j[] , int i[]
从属关系：主要被本文件的 move_man()调用。另外，在人物刚出生时，还被born()调用
------------------------------------------------------*/ 
int count_ij(int who , int y , int x , int j[] , int i[])
{
	if(a[j[0]][i[0]] == 7+who) a[j[0]][i[0]] = 0;
	j[0]=xy_to_ij(y);
	i[0]=xy_to_ij(x);
	if(a[j[0]][i[0]] == 0) a[j[0]][i[0]] = 7+who;

	/*(j[0],i[0])是靠左(或上)的格子，下面判断是否占据右（或下）方的格子*/
	if(a[j[1]][i[1]] == 7+who) a[j[1]][i[1]] = 0;
	if (((y-10)%GRID_SIZE)!=0){
		j[1]=j[0]+1;
		i[1]=i[0];
	}else if(((x-10)%GRID_SIZE)!=0){
		j[1]=j[0];
		i[1]=i[0]+1;
	}else{
		j[1]=j[0];
		i[1]=i[0];
	}
	if(a[j[1]][i[1]] == 0) a[j[1]][i[1]] = 7+who;
	return 0;
}

/*=====================================================
函数原型：int check_next(int who, int y, int x);
功能：检查人物移动方向上，下一个是否有障碍物
入口：人物序号who，屏幕坐标（y，x）
出口：返回0表示无障碍，返回1表示有障碍
从属关系：被本文件的 move_man()调用
------------------------------------------------------*/ 
int check_next(int who, int y, int x)
{
	int j,i;
	if ((direction[who] == 2)||(direction[who] == 4)){
		y += GRID_SIZE -1;
		x += GRID_SIZE -1;
	}/*输入的x,y是人物左上角的坐标。如果是向下或向右移动，则改为检测右下角那个点。
	 否则如果是向上或左移动，则保持检测左上角的点不变*/

	j=xy_to_ij(y);
	i=xy_to_ij(x);
	if(a[j][i]==0) 
		return 0;
	else if (a[j][i]==7+who)
		return 0;
	else if ((1<=a[j][i])&&(a[j][i]<=17))
		return 1;
}

/*=====================================================
函数原型：int move_man(int who,int dy,int dx);
功能：将人物移动一段距离，实现一帧动画
入口：人物序号who，在屏幕上的坐标偏移量（△y，△x）
出口：调用draw_player()等绘图函数
从属关系：被本文件的 do_man()调用
------------------------------------------------------*/ 
int move_man(int who,int dy,int dx)
{
	int res , fix;
	res = check_next(who , py[who] + dy,px[who] + dx);

	/*-----下面开始对检查结果进行分析-------*/
	if(res==1){
		direction[who]=0;/*碰到障碍，强行停下。原来想走dx那么远，现在不行了，要减去△*/
		if(dy==0){
			fix=dx / abs(dx);
			do{
				dx=dx-fix;
			}while(((px[who] + dx)-10) % GRID_SIZE != 0);
		}
		else
		{
			fix=dy / abs(dy);
			do{
				dy=dy-fix;
			}while(((py[who] + dy)-10) % GRID_SIZE != 0);
		}
	} 
	py[who]+=dy;
	px[who]+=dx;
	if(a[ pj[who][0] ][ pi[who][0] ] != 18 ) draw(pj[who][0] , pi[who][0] , a[ pj[who][0] ][ pi[who][0] ] );
	if(a[ pj[who][1] ][ pi[who][1] ] != 18 ) draw(pj[who][1] , pi[who][1] , a[ pj[who][1] ][ pi[who][1] ] );
	draw_player(who,py[who],px[who]);
	count_ij(who ,  py[who] , px[who] , pj[who] ,pi[who]);
	return 0;
}

/*=====================================================
函数原型：int speed(int time);
功能：计算人物能够前进的一段小长度，也就是速度
入口：人物已经行走的时间
出口：返回速度
从属关系：人物在行走时，被本文件do_man()调用。其与函数move_man()配合使用
------------------------------------------------------*/ 
int speed(int time){
	if(time<13)
		return HI_SPEED;
	else
		return RUN_SPEED;

}

/*=====================================================
函数原型：int count_delta(int who,int xy);
功能：计算人与最近的一格的偏移量，以便在释放操作时，自动回到最近一格中。
入口：人物序号who，当前的一个坐标y或x
出口：返回偏移量。如果偏移量为正，说明他需要向下（或右）走才能归位；负的，则向上（或左）
从属关系：当人物没有操作时，被本文件do_man()调用。
------------------------------------------------------*/ 
int count_delta(int who,int xy)
{
	int temp;
	temp=(xy-10)%GRID_SIZE;
	if (walk_time[who] < 2)
	{/*开始走的时间极短，意味着用户轻触方向键，并立即放开。这时候让他走到下一格而不是找最近一格*/
		if ((direction[who] == 2)||(direction[who] == 4))
		{/*当前速度为 下或右*/
			return GRID_SIZE-temp;
		}else {/*上或左*/
			return -temp;
		}
	}else{/*开始走路已经有一段时间，这个时候停下来，则选择离他最近的一个，并返回他现在与该格的差距*/
		if(temp>=GRID_SIZE/2)
			return GRID_SIZE-temp;
		else 
			return -temp;
	}
}

/*=====================================================
函数原型：int do_man(int who);
功能：控制人物行为
入口：人物序号who
出口：无返回值。主要调用move_man进行操作
从属关系：以18.2Hz的频率，被MAIN.C中GamePlay()调用
------------------------------------------------------*/ 
int do_man(int who)
{
	int sign_of_d,delta;

	things( who , xy_to_ij(py[who]) , xy_to_ij(px[who]) );
	things( who , xy_to_ij(py[who]+GRID_SIZE-1) , xy_to_ij(px[who]+GRID_SIZE-1) );
	if(direction[who]==0){
		if(handle_direction[who]>0){/*动画不再进行，但有了新的操作*/
			direction[who]=handle_direction[who];
			walk_time[who]=0;
			move_man(who , speed(0) * fly[direction[who]][1] , speed(0) * fly[direction[who]][0]);
		}else{/*动画不在进行，且没有操作*/

		}
	}
	else
	{/*动画正在进行*/
		if((handle_direction[who]==direction[who])){/*动画正在进行,且操作不改变*/
			walk_time[who]++;
			move_man(who , speed(walk_time[who]) * fly[direction[who]][1] , speed(walk_time[who]) * fly[direction[who]][0]);
		}else{/*动画正在进行,但用户已经放开按键，现在应该回到离他最近的一格中*/
			if((direction[who]==1)||(direction[who]==2)){
				delta = count_delta(who, py[who] );/*如果上下移动*/
			}else{
				delta = count_delta(who, px[who] );/*左右*/
			}

			sign_of_d=(delta>=0)? 1:(-1);
			if ((delta < -LOW_SPEED)||(delta > LOW_SPEED)){ /*还远着呢*/
				if ((direction[who]==1)||(direction[who]==2)){
					move_man(who , sign_of_d * LOW_SPEED * fly[2][1] , sign_of_d * LOW_SPEED * fly[2][0]);
					/*向上下移动修正*/
				}else{
					move_man(who , sign_of_d * LOW_SPEED * fly[4][1] , sign_of_d * LOW_SPEED * fly[4][0]);
					/*左右*/
				}
				delta -= sign_of_d * LOW_SPEED ;        /*移动了LOW_SPEED距离，与目标格子的距离缩短了*/
			}else{/*就差一步了，人物移动到目标格中*/
				if (delta!=0) {
					if ((direction[who]==1)||(direction[who]==2))
						move_man(who , delta  * fly[2][1] , delta * fly[2][0]);
					else
						move_man(who , delta  * fly[4][1] , delta * fly[4][0]);
				}
				direction[who]=0;
			}
		}
	}
	return 0;
}

/*=====================================================
函数原型：int if_killed_by_bomb(int who);
功能：判断一个人有没有被炸弹炸死
入口：人物序号who
出口：如果没死则返回0，被炸死则返回1
从属关系：被MAIN.C中的GamePlay()不断地调用
------------------------------------------------------*/ 
int if_killed_by_bomb(int who)
{
	int y,x;
	if( lifetime[who] != -1) return 0;

	y= py[who] + 2;/*把人的面积缩小一点，没那么容易死*/
	x= px[who] + 2;
	if( a[ xy_to_ij(y) ][ xy_to_ij(x) ] ==18 ){
		return 1;
	}

	y = py[who] + GRID_SIZE -3;/*-1 -2*/
	x = px[who] + GRID_SIZE -3;
	if( a[ xy_to_ij(y) ][ xy_to_ij(x) ] ==18 ){
		return 1;
	}
	return 0;
}

/*=====================================================
函数原型：int die(int who);
功能：让人物死亡
入口：人物序号who
出口：无返回值。
从属关系：人物死亡时调用。主要由MAIN.C中的GamePlay()调用
------------------------------------------------------*/ 
int die(int who)
{
	lifetime[who] = (newtime + REVIVE_TIME) % 30000;
	life[who]--;
	write_life(who);
	bomb_num[who] = DEFAULT_BOMB_NUM;
	write_bomb_num(who);
	bomb_power[who] = DEFAULT_BOMB_POWER;
	write_bomb_power(who);
	check_if_someone_lost();

	if( a[ pj[who][0] ][ pi[who][0] ] == 7+who ) 
		a[ pj[who][0] ][ pi[who][0] ] = 0;
	draw(pj[who][0] , pi[who][0] , a[ pj[who][0] ][ pi[who][0] ]);

	if( a[ pj[who][1] ][ pi[who][1] ] == 7+who )
		a[ pj[who][1] ][ pi[who][1] ] = 0;
	draw(pj[who][1] , pi[who][1] , a[ pj[who][1] ][ pi[who][1] ]);

	py[who] = px[who]=-100;/*移出地图外。set_bomb()函数中要用到*/
	pj[who][0] = pi[who][0] = pj[who][1] = pi[who][1] =-100;
	return 0;
}

/*=====================================================
函数原型：int born(int who);
功能：让人物出生
入口：人物序号who
出口：无返回值
从属关系：由MAIN.C中的GamePlay()调用
------------------------------------------------------*/ 
int born(int who)
{
	lifetime[who] = -1;
	py[who]=ij_to_xy(bornj[who]);
	px[who]=ij_to_xy(borni[who]);
	count_ij(who,py[who],px[who],pj[who],pi[who]);
	draw_player(who,py[who],px[who]);

	return 0;
}
