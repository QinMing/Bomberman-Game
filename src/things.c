/*===================�ļ�things.c=======================
��ŵ�ͼ�ϵ��ߵ�Ч��
----------------------------------------------------*/ 


/*=====================================================
����ԭ�ͣ�int things(int who,int j,int i);
���ܣ����������������Ʒ����ʵ����ƷЧ����Ȼ�����Ʒ�ӵ�ͼ�ϲ�����
��ڣ��������who���˴������ĸ��������У�j,i��
���ڣ����ñ�ҳ�ĸ����ߺ�����������draw()��ͼ
������ϵ����Ϊ�����ߡ�ģ�����ں���������ctrl.c�е�do_man()�������á�
------------------------------------------------------*/ 
int things(int who,int j,int i){/*who�����������t��Ʒ*/
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


/*����a��b������*/ 
int exchange2nums(int *a , int * b)
{
	int t;
	t=*a;
	*a=*b;
	*b=t;
	return 0;
}

/*=====================================================
����ԭ�ͣ�int random_thing();
���ܣ�
��ڣ��޲���
���ڣ�����һ����������Ҵ�����ĳ�����ߵı��
������ϵ����bomb.c�е�blast()���á�
   blast()��ը��һ��שǽ��ʱ����Ҫ�������һ������
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

 /*  ��������������Ч��*/
int pluslife(int who)
{
	if (life[who]+1>MAX_LIFE) return 0;
	life[who]++;
	write_life(who);
	return 0;
}

/*  ����ը����������Ч��*/
int plusbomb(int who)
{
	if (bomb_num[who]+1>MAX_BOMB_NUM) return 0;
	bomb_num[who]++;
	write_bomb_num(who);
	return 0;
}

/*  ����ը������������Ч��*/
int pluspower(int who){
	if (bomb_power[who]+1>MAX_BOMB_POWER) return 0;
	bomb_power[who]++;
	write_bomb_power(who);
	return 0;
}

/*  ����������ը��������Ч��*/
int blastall(){
	int i;
	for(i=0;i<MAXBOMB;i++)
		if (bomb[i]>0) {
			bomb[i] = newtime + 1;
			bomb_rm[i] = (newtime + 1 + BLAST_TIME) % 30000;
		}
		return 0;
}

/*  ����λ������Ч��*/
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

/*  ������ҩˮ������Ч������û�С���������ֻ����������һ��*/
int deathpotion(int who){
	life[who]--;
	write_life(who);
	check_if_someone_lost();

	return 0;
}