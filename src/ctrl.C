/*===================�ļ�ctrl.c=======================
������������������صĺ���
----------------------------------------------------*/ 


/*=====================================================
����ԭ�ͣ�int check_if_someone_lost();
���ܣ����˫������ֵ
��ڣ���
���ڣ�ȫ�ֱ���loser
������ϵ����things.c��deathpotion()�� ���ļ���die() ����
ֻҪ�������٣����ͻᱻ����
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
����ԭ�ͣ�int count_ij(int who , int y , int x , int j[] , int i[]);
���ܣ����ݵ�ǰ��������Ļ�ϵ����꣬����������ڵ�ͼ�ϵ�����������꣬
     ˳���ڵ�ͼ����a�а���������Ϊ��Ӧ���֣�player0��Ϊ7��player1��Ϊ8
��ڣ�������who������Ļ�ϵ�������y�ͺ�����x
���ڣ����� int j[] , int i[]
������ϵ����Ҫ�����ļ��� move_man()���á����⣬������ճ���ʱ������born()����
------------------------------------------------------*/ 
int count_ij(int who , int y , int x , int j[] , int i[])
{
	if(a[j[0]][i[0]] == 7+who) a[j[0]][i[0]] = 0;
	j[0]=xy_to_ij(y);
	i[0]=xy_to_ij(x);
	if(a[j[0]][i[0]] == 0) a[j[0]][i[0]] = 7+who;

	/*(j[0],i[0])�ǿ���(����)�ĸ��ӣ������ж��Ƿ�ռ���ң����£����ĸ���*/
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
����ԭ�ͣ�int check_next(int who, int y, int x);
���ܣ���������ƶ������ϣ���һ���Ƿ����ϰ���
��ڣ��������who����Ļ���꣨y��x��
���ڣ�����0��ʾ���ϰ�������1��ʾ���ϰ�
������ϵ�������ļ��� move_man()����
------------------------------------------------------*/ 
int check_next(int who, int y, int x)
{
	int j,i;
	if ((direction[who] == 2)||(direction[who] == 4)){
		y += GRID_SIZE -1;
		x += GRID_SIZE -1;
	}/*�����x,y���������Ͻǵ����ꡣ��������»������ƶ������Ϊ������½��Ǹ��㡣
	 ������������ϻ����ƶ����򱣳ּ�����Ͻǵĵ㲻��*/

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
����ԭ�ͣ�int move_man(int who,int dy,int dx);
���ܣ��������ƶ�һ�ξ��룬ʵ��һ֡����
��ڣ��������who������Ļ�ϵ�����ƫ��������y����x��
���ڣ�����draw_player()�Ȼ�ͼ����
������ϵ�������ļ��� do_man()����
------------------------------------------------------*/ 
int move_man(int who,int dy,int dx)
{
	int res , fix;
	res = check_next(who , py[who] + dy,px[who] + dx);

	/*-----���濪ʼ�Լ�������з���-------*/
	if(res==1){
		direction[who]=0;/*�����ϰ���ǿ��ͣ�¡�ԭ������dx��ôԶ�����ڲ����ˣ�Ҫ��ȥ��*/
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
����ԭ�ͣ�int speed(int time);
���ܣ����������ܹ�ǰ����һ��С���ȣ�Ҳ�����ٶ�
��ڣ������Ѿ����ߵ�ʱ��
���ڣ������ٶ�
������ϵ������������ʱ�������ļ�do_man()���á����뺯��move_man()���ʹ��
------------------------------------------------------*/ 
int speed(int time){
	if(time<13)
		return HI_SPEED;
	else
		return RUN_SPEED;

}

/*=====================================================
����ԭ�ͣ�int count_delta(int who,int xy);
���ܣ��������������һ���ƫ�������Ա����ͷŲ���ʱ���Զ��ص����һ���С�
��ڣ��������who����ǰ��һ������y��x
���ڣ�����ƫ���������ƫ����Ϊ����˵������Ҫ���£����ң��߲��ܹ�λ�����ģ������ϣ�����
������ϵ��������û�в���ʱ�������ļ�do_man()���á�
------------------------------------------------------*/ 
int count_delta(int who,int xy)
{
	int temp;
	temp=(xy-10)%GRID_SIZE;
	if (walk_time[who] < 2)
	{/*��ʼ�ߵ�ʱ�伫�̣���ζ���û��ᴥ��������������ſ�����ʱ�������ߵ���һ������������һ��*/
		if ((direction[who] == 2)||(direction[who] == 4))
		{/*��ǰ�ٶ�Ϊ �»���*/
			return GRID_SIZE-temp;
		}else {/*�ϻ���*/
			return -temp;
		}
	}else{/*��ʼ��·�Ѿ���һ��ʱ�䣬���ʱ��ͣ��������ѡ�����������һ������������������ø�Ĳ��*/
		if(temp>=GRID_SIZE/2)
			return GRID_SIZE-temp;
		else 
			return -temp;
	}
}

/*=====================================================
����ԭ�ͣ�int do_man(int who);
���ܣ�����������Ϊ
��ڣ��������who
���ڣ��޷���ֵ����Ҫ����move_man���в���
������ϵ����18.2Hz��Ƶ�ʣ���MAIN.C��GamePlay()����
------------------------------------------------------*/ 
int do_man(int who)
{
	int sign_of_d,delta;

	things( who , xy_to_ij(py[who]) , xy_to_ij(px[who]) );
	things( who , xy_to_ij(py[who]+GRID_SIZE-1) , xy_to_ij(px[who]+GRID_SIZE-1) );
	if(direction[who]==0){
		if(handle_direction[who]>0){/*�������ٽ��У��������µĲ���*/
			direction[who]=handle_direction[who];
			walk_time[who]=0;
			move_man(who , speed(0) * fly[direction[who]][1] , speed(0) * fly[direction[who]][0]);
		}else{/*�������ڽ��У���û�в���*/

		}
	}
	else
	{/*�������ڽ���*/
		if((handle_direction[who]==direction[who])){/*�������ڽ���,�Ҳ������ı�*/
			walk_time[who]++;
			move_man(who , speed(walk_time[who]) * fly[direction[who]][1] , speed(walk_time[who]) * fly[direction[who]][0]);
		}else{/*�������ڽ���,���û��Ѿ��ſ�����������Ӧ�ûص����������һ����*/
			if((direction[who]==1)||(direction[who]==2)){
				delta = count_delta(who, py[who] );/*��������ƶ�*/
			}else{
				delta = count_delta(who, px[who] );/*����*/
			}

			sign_of_d=(delta>=0)? 1:(-1);
			if ((delta < -LOW_SPEED)||(delta > LOW_SPEED)){ /*��Զ����*/
				if ((direction[who]==1)||(direction[who]==2)){
					move_man(who , sign_of_d * LOW_SPEED * fly[2][1] , sign_of_d * LOW_SPEED * fly[2][0]);
					/*�������ƶ�����*/
				}else{
					move_man(who , sign_of_d * LOW_SPEED * fly[4][1] , sign_of_d * LOW_SPEED * fly[4][0]);
					/*����*/
				}
				delta -= sign_of_d * LOW_SPEED ;        /*�ƶ���LOW_SPEED���룬��Ŀ����ӵľ���������*/
			}else{/*�Ͳ�һ���ˣ������ƶ���Ŀ�����*/
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
����ԭ�ͣ�int if_killed_by_bomb(int who);
���ܣ��ж�һ������û�б�ը��ը��
��ڣ��������who
���ڣ����û���򷵻�0����ը���򷵻�1
������ϵ����MAIN.C�е�GamePlay()���ϵص���
------------------------------------------------------*/ 
int if_killed_by_bomb(int who)
{
	int y,x;
	if( lifetime[who] != -1) return 0;

	y= py[who] + 2;/*���˵������Сһ�㣬û��ô������*/
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
����ԭ�ͣ�int die(int who);
���ܣ�����������
��ڣ��������who
���ڣ��޷���ֵ��
������ϵ����������ʱ���á���Ҫ��MAIN.C�е�GamePlay()����
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

	py[who] = px[who]=-100;/*�Ƴ���ͼ�⡣set_bomb()������Ҫ�õ�*/
	pj[who][0] = pi[who][0] = pj[who][1] = pi[who][1] =-100;
	return 0;
}

/*=====================================================
����ԭ�ͣ�int born(int who);
���ܣ����������
��ڣ��������who
���ڣ��޷���ֵ
������ϵ����MAIN.C�е�GamePlay()����
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