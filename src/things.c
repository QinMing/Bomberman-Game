/*===================文件things.c=======================
存放地图上道具的效果
----------------------------------------------------*/ 


/*=====================================================
函数原型：int things(int who,int j,int i);
功能：检查人物碰到的物品，并实现物品效果，然后把物品从地图上擦除。
入口：人物序号who，人触碰到的格点的行与列（j,i）
出口：调用本页的各道具函数，并调用draw()绘图
从属关系：作为“道具”模块的入口函数，它被ctrl.c中的do_man()函数调用。
------------------------------------------------------*/ 
int things(int who,int j,int i){/*who这个人碰到了t物品*/
	int t;
	t=a[j][i];
	switch(t){
	case 31: pluslife(who);break;
	case 32: plusbomb(who);break;
	case 33: pluspower(who);break;
	case 42: blastall();break;
	case 43: exchange();break;
	case 44: deathpotion(who);break;
	}

	switch(t){
	case 31: case 32: case 33: 
	case 42: case 43: case 44:
		a[j][i] = 0;
		draw(j,i,0);
		break;
	}
	return 0;
}


/*交换a，b两个数*/ 
int exchange2nums(int *a , int * b)
{
	int t;
	t=*a;
	*a=*b;
	*b=t;
	return 0;
}

/*=====================================================
函数原型：int random_thing();
功能：
入口：无参数
出口：返回一个随机数，且此数是某个道具的编号
从属关系：被bomb.c中的blast()调用。
   blast()在炸到一个砖墙的时候，需要随机掉出一个道具
------------------------------------------------------*/ 
int random_thing()
{
	int n;
	if(random(3)>0) return 0;

	do{
		n=random(THINGS_MAX - THINGS_MIN + 1 ) + THINGS_MIN;
		switch(n){
		case 31: case 32: case 33: 
		case 42: case 43: case 44:
			return n;
		}
	}while(1);
}

 /*  “加生命”道具效果*/
int pluslife(int who)
{
	if (life[who]+1>MAX_LIFE) return 0;
	life[who]++;
	write_life(who);
	return 0;
}

/*  “加炸弹数”道具效果*/
int plusbomb(int who)
{
	if (bomb_num[who]+1>MAX_BOMB_NUM) return 0;
	bomb_num[who]++;
	write_bomb_num(who);
	return 0;
}

/*  “加炸弹威力”道具效果*/
int pluspower(int who){
	if (bomb_power[who]+1>MAX_BOMB_POWER) return 0;
	bomb_power[who]++;
	write_bomb_power(who);
	return 0;
}

/*  “引爆所有炸弹”道具效果*/
int blastall(){
	int i;
	for(i=0;i<MAXBOMB;i++)
		if (bomb[i]>0) {
			bomb[i] = newtime + 1;
			bomb_rm[i] = (newtime + 1 + BLAST_TIME) % 30000;
		}
		return 0;
}

/*  “换位”道具效果*/
int exchange(){
	if (lifetime[0]!=-1||lifetime[1]!=-1) return 0;
	exchange2nums(&px[0] , &px[1]);
	exchange2nums(&py[0] , &py[1]);

	exchange2nums(&pj[0][0] , &pj[1][0]);
	exchange2nums(&pj[0][1] , &pj[1][1]);

	exchange2nums(&pi[0][0] , &pi[1][0]);
	exchange2nums(&pi[0][1] , &pi[1][1]);

	a[ pj[0][0] ][ pi[0][0] ] = a[ pj[0][1] ][ pi[0][1] ] = 7;
	a[ pj[1][1] ][ pi[1][1] ] = a[ pj[1][0] ][ pi[1][0] ] = 8;

	exchange2nums(&direction[0] , &direction[1]);

	draw_player(0 , py[0] , px[0]);
	draw_player(1 , py[1] , px[1]);
	return 0;
}

/*  “死亡药水”道具效果（并没有“死亡”，只是生命数减一）*/
int deathpotion(int who){
	life[who]--;
	write_life(who);
	check_if_someone_lost();

	return 0;
}