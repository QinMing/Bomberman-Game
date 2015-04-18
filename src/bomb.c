/*===================文件bomb.c=======================
存放与炸弹相关的函数
----------------------------------------------------*/ 

/*=====================================================
函数原型：int set_bomb(int who);
功能：安放炸弹
从属关系：检测到bomb_button等于1时，被GamePlay()调用
------------------------------------------------------*/ 
int set_bomb(int who)
{
	int j , i , p;
	if (bomb_num[who] <= 0) return 0;
	j = xy_to_ij( py[who] ) ;
	i = xy_to_ij( px[who] ) ;
	if ((direction[who]==1) && (( py[who]-10 ) % GRID_SIZE !=0)){/*如果向上，且在两个格子之间，就放在下面那格里*/
		j++;
	}else if ((direction[who]==3) &&  (( px[who]-10 ) % GRID_SIZE !=0)){/*如果向左，且在两个格子之间，就放在右边那格里*/
		i++;
	}/*总之，此if语句的用途是，避免让人在行进过程中把炸弹放在自己前面，挡住自己的路*/

	if((j<0)||(i<0)) return 0;/*此人不在地图内，不能放炸弹。可能是死了*/
	if(a[j][i] >= 30) return 0;/*是物品，就不放炸弹*/
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
函数原型：int serial_blast(int j,int i);
功能：连续引爆其他炸弹。
入口：被别的炸弹炸到的这颗炸弹的格点坐标(j,i)
从属关系：被blast()调用
------------------------------------------------------*/ 
int serial_blast(int j,int i)
{/*一颗炸弹炸到了另一颗炸弹，引发连环爆炸*/
	int p;
	for(p=0;p<MAXBOMB;p++){
		if ((bombj[p] == j)&&(bombi[p] == i)){
			if (bomb[p] == -1) /*它已经引爆过了，不用再引爆*/
				return 0;
			bomb_rm[p] = ( thetime + BLAST_TIME) % 30000;
			blast(p);
			return 0;
		}
	}
}

/*=====================================================
函数原型：int blast(int p);
功能：引爆一个炸弹，绘制爆炸图样
入口：炸弹序号p
出口：调用graph.c中的draw()函数
从属关系：被MAIN.C中的GamePlay()和 本文件中serial_blast()调用
------------------------------------------------------*/ 
int blast(int p)
{
	int d,r,i,j;
	bomb[p] = -1;
	a[ bombj[p] ][ bombi[p] ] = 18;
	draw(bombj[p] , bombi[p] , 18);
	for(d=1;d<=4;d++){/*四个方向*/
		for(r=1;r<=bombrange[p];r++){
			j = bombj[p] + r * fly[d][1];
			i = bombi[p] + r * fly[d][0];
			if( a[j][i] == 1){
				back_a[j][i] = random_thing(); /*砖墙被炸没了，待会爆炸结束的时候，此处变成路面,或物品*/
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
				back_a[j][i] = 0;/*如果这里是爆炸图样，或者是人，待会炸完的时候，这里变成路面*/  /*人这个bug在2011年6月7日21:16:36发现*/
				a[j][i] = 18;
				draw(j,i,18);
				bomb_blastto[p][d] = r;
			}else{
				back_a[j][i] = a[j][i];/*把地上的物品存起来，爆炸完后重新放回来*/
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
函数原型：int remove_blast(int p);
功能：消除爆炸图样
入口：炸弹序号p
出口：调用graph.c中的draw()函数
从属关系：被MAIN.C中的GamePlay()调用
------------------------------------------------------*/ 
int remove_blast(int p)
{
	int d,r,i,j;
	a[ bombj[p] ][ bombi[p] ] = 0;
	draw(bombj[p] , bombi[p] , 0);
	for(d=1;d<=4;d++){/*四个方向*/ 
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
