/*===================�ļ�bomb.c=======================
�����ը����صĺ���
----------------------------------------------------*/ 

/*=====================================================
����ԭ�ͣ�int set_bomb(int who);
���ܣ�����ը��
������ϵ����⵽bomb_button����1ʱ����GamePlay()����
------------------------------------------------------*/ 
int set_bomb(int who)
{
	int j , i , p;
	if (bomb_num[who] <= 0) return 0;
	j = xy_to_ij( py[who] ) ;
	i = xy_to_ij( px[who] ) ;
	if ((direction[who]==1) && (( py[who]-10 ) % GRID_SIZE !=0)){/*������ϣ�������������֮�䣬�ͷ��������Ǹ���*/
		j++;
	}else if ((direction[who]==3) &&  (( px[who]-10 ) % GRID_SIZE !=0)){/*�������������������֮�䣬�ͷ����ұ��Ǹ���*/
		i++;
	}/*��֮����if������;�ǣ������������н������а�ը�������Լ�ǰ�棬��ס�Լ���·*/

	if((j<0)||(i<0)) return 0;/*���˲��ڵ�ͼ�ڣ����ܷ�ը��������������*/
	if(a[j][i] >= 30) return 0;/*����Ʒ���Ͳ���ը��*/
	if(a[j][i] == 5) return 0;

	/*draw_player(who , py[who] , px[who]);*/
	for (p=0;p<MAXBOMB;p++){
		if ((bomb_rm[p] == -1)&&(bomb[p] == -1)){
			bomb[p]=( newtime + BOMB_TIME ) % 30000;
			bomb_rm[p]=( newtime + BOMB_TIME + BLAST_TIME) % 30000;
			bombj[p] = j;
			bombi[p] = i;
			bombrange[p]=bomb_power[who];
			whosebomb[p]=who;
			bomb_num[who]--;
			write_bomb_num(who);

			a[j][i] = 5;
			draw(j,i,5);
			break;
		}
	}
	return 0;
}

/*=====================================================
����ԭ�ͣ�int serial_blast(int j,int i);
���ܣ�������������ը����
��ڣ������ը��ը�������ը���ĸ������(j,i)
������ϵ����blast()����
------------------------------------------------------*/ 
int serial_blast(int j,int i)
{/*һ��ը��ը������һ��ը��������������ը*/
	int p;
	for(p=0;p<MAXBOMB;p++){
		if ((bombj[p] == j)&&(bombi[p] == i)){
			if (bomb[p] == -1) /*���Ѿ��������ˣ�����������*/
				return 0;
			bomb_rm[p] = ( thetime + BLAST_TIME) % 30000;
			blast(p);
			return 0;
		}
	}
}

/*=====================================================
����ԭ�ͣ�int blast(int p);
���ܣ�����һ��ը�������Ʊ�ըͼ��
��ڣ�ը�����p
���ڣ�����graph.c�е�draw()����
������ϵ����MAIN.C�е�GamePlay()�� ���ļ���serial_blast()����
------------------------------------------------------*/ 
int blast(int p)
{
	int d,r,i,j;
	bomb[p] = -1;
	a[ bombj[p] ][ bombi[p] ] = 18;
	draw(bombj[p] , bombi[p] , 18);
	for(d=1;d<=4;d++){/*�ĸ�����*/
		for(r=1;r<=bombrange[p];r++){
			j = bombj[p] + r * fly[d][1];
			i = bombi[p] + r * fly[d][0];
			if( a[j][i] == 1){
				back_a[j][i] = random_thing(); /*שǽ��ըû�ˣ����ᱬը������ʱ�򣬴˴����·��,����Ʒ*/
				a[j][i] = 18;
				draw(j,i,18);
				bomb_blastto[p][d] = r;
				break;
			}else if (a[j][i] == 3){
				bomb_blastto[p][d] = r - 1;
				break;
			}else if (a[j][i] == 5){
				serial_blast(j,i);
				bomb_blastto[p][d] = r;
			}else if (a[j][i] == 18){
				bomb_blastto[p][d] = r;
			}else if ((a[j][i] == 7)||(a[j][i] == 8)){
				back_a[j][i] = 0;/*��������Ǳ�ըͼ�����������ˣ�����ը���ʱ��������·��*/  /*�����bug��2011��6��7��21:16:36����*/
				a[j][i] = 18;
				draw(j,i,18);
				bomb_blastto[p][d] = r;
			}else{
				back_a[j][i] = a[j][i];/*�ѵ��ϵ���Ʒ����������ը������·Ż���*/
				a[j][i] = 18;
				draw(j,i,18);
				bomb_blastto[p][d] = r;
			}
		}
	}

	bomb_num[  whosebomb[p]  ] ++;
	write_bomb_num( whosebomb[p] );
	return 0;
}

/*=====================================================
����ԭ�ͣ�int remove_blast(int p);
���ܣ�������ըͼ��
��ڣ�ը�����p
���ڣ�����graph.c�е�draw()����
������ϵ����MAIN.C�е�GamePlay()����
------------------------------------------------------*/ 
int remove_blast(int p)
{
	int d,r,i,j;
	a[ bombj[p] ][ bombi[p] ] = 0;
	draw(bombj[p] , bombi[p] , 0);
	for(d=1;d<=4;d++){/*�ĸ�����*/ 
		for(r=1;r<=bomb_blastto[p][d];r++){ 
			j = bombj[p] + r * fly[d][1]; 
			i = bombi[p] + r * fly[d][0]; 
			a[j][i] = back_a[j][i]; 
			draw(j , i , a[j][i]); 
		} 
	} 

	bomb_rm[p] = -1;
	return 0;
}
